cmake_minimum_required(VERSION 3.14)

# Detect vcpkg ports
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" ENV_VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${ENV_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(ShiftLib
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "An optimised utility library for modern C++"
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Default to a release build if desired configuration is not specified
    if(NOT CMAKE_CONFIGURATION_TYPES)
        if(NOT CMAKE_BUILD_TYPE)
            message(STATUS "Setting build type to 'Release' as none was specified.")
            set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
        endif()
    endif()
    
    # Set the install location to the source location if not currently specified
    if("${CMAKE_INSTALL_PREFIX}" STREQUAL "")
        message("Installing into source folder")
        set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "..." FORCE)
    endif()

    # Use folder structure for arranging files within IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    
    # Setup basic testing parameters
    include(CTest)
    
    # Build documentation if available
    find_package(Doxygen)
    if(Doxygen_FOUND)
        set(DOXYGEN_EXTRACT_ALL YES)
        set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

        include(GNUInstallDirs)

        doxygen_add_docs(docs
            "${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include"
            DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_DOCDIR}"
        )

        # TODO: install docs to ${CMAKE_INSTALL_DOCDIR}
    endif()
endif()

# Add in the library code
add_library(ShiftLib INTERFACE)

target_sources(ShiftLib INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSArchitecture.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSConfig.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSTraits.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSMath.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSInt128.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSUInt128.hpp>"
    
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSTraits.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSMath.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSInt128.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSUInt128.inl>"
)

target_compile_features(ShiftLib
    INTERFACE cxx_std_17
)

target_include_directories(ShiftLib
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib>"
    "$<INSTALL_INTERFACE:include/ShiftLib>"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" REGULAR_EXPRESSION "*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "Source Files" REGULAR_EXPRESSION "*.cpp")

include(GNUInstallDirs)
install(TARGETS ShiftLib
    EXPORT ShiftLibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ShiftLibTargets
    FILE ShiftLibTargets.cmake
    NAMESPACE ShiftLib::
    DESTINATION lib/cmake/ShiftLib
)
add_library(ShiftLib::ShiftLib ALIAS ShiftLib)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ShiftLibConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ShiftLibConfig.cmake.in" ShiftLibConfig.cmake @ONLY)
install(FILES ShiftLibConfig.cmake ShiftLibConfigVersion.cmake
    DESTINATION lib/cmake/ShiftLib
)

# Add tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    add_executable(XSTest
        tests/XSTraitsTest.cpp
        tests/XSMathTest.cpp
        tests/XSUInt128Test.cpp   
        tests/XSInt128Test.cpp    
    )

    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_FOUND)
    if(IPO_FOUND)
        set_target_properties(XSTest PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

    target_link_libraries(XSTest
        PRIVATE GTest::GTest
        PRIVATE GTest::Main
        PRIVATE ShiftLib
    )

    set_target_properties(
        XSTest PROPERTIES
        EXCLUDE_FROM_ALL true
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    
    # Enable all compile warnings
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(XSTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror>)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        target_compile_options(XSTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror -wd1011>)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options(XSTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4 /WX /experimental:external /external:anglebrackets /external:W3>)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
            target_compile_options(XSTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4 /WX>)
        else()
            target_compile_options(XSTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror>)
        endif()
    endif()

    gtest_discover_tests(XSTest
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# Packaging support
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CPACK_PACKAGE_VENDOR "Matt Oliver")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    
    set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
    set(CPACK_SOURCE_IGNORE_FILES
        /.git*
        /dist
        /.*build.*
        /\\\\.DS_Store
    )
    include(CPack)
endif()
