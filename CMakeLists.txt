cmake_minimum_required(VERSION 3.14)

# Detect vcpkg ports
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" ENV_VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${ENV_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
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

    #Setup basic benchmarking parameters
    if(BUILD_TESTING)
        option(BUILD_BENCHMARKING "Create benchmarks using CMake" ON)
    endif()
    
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
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSConfig.h>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSCompiler.h>"

    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSTraits.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSMath.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSBit.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSInt128.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSUInt128.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSRational.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/XSUtility.hpp>"
    
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMDData.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMDTraits.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMDBool.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMDBase.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMDInBase.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD2.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD3.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD4.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD6.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD3x2.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD8.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD8.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD3x3.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD3x3.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD12.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD12.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD3x4.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD3x4.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMD16.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMDx86.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/SIMD/XSSIMDx86Functions.hpp>"    
    
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSAABoundingBox.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSAABoundingBox.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSAABoundingBox2.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSAABoundingBox2.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSAABoundingBox4.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSAABoundingBox4.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSMatrix3x3.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSMatrix3x3.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSMatrix4x3.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSMatrix4x3.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSMatrix4x4.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSMatrix4x4.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSPoint3D.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSPoint3D.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSPoint3D2.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSPoint3D2.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSPoint3D4.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSPoint3D4.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSQuaternion.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSQuaternion.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRange.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRange.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRange2.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRange2.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRange4.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRange4.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRay.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRay.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRay2.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRay2.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRay4.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSRay4.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSVector3D.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSVector3D.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSVector3D2.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSVector3D2.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSVector3D4.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSVector3D4.inl>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSTransform.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Geometry/XSTransform.inl>"
    
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSAllocatorHeap.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSAllocatorStack.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSMemory.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSIterator.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSIteratorOffset.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSArray.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSArrayView.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSDArray.hpp>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib/Memory/XSString.hpp>"
)

target_compile_features(ShiftLib
    INTERFACE cxx_std_20
)

target_include_directories(ShiftLib
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ShiftLib>"
    "$<INSTALL_INTERFACE:include/ShiftLib>"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" REGULAR_EXPRESSION "*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" REGULAR_EXPRESSION "*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" REGULAR_EXPRESSION "*.cpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" REGULAR_EXPRESSION "*.inl")

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
    
    #Source files are included in main executable and in dependent ISA specific static libs
    set(SHIFTLIBTESTFILES
        tests/XSCompilerOptions.h
        tests/XSGTest.hpp
        tests/XSUInt128Test.cpp
        tests/XSInt128Test.cpp
        tests/XSMathTest.cpp
        tests/XSTraitsTest.cpp
        tests/XSBitTest.cpp
        tests/XSRationalTest.cpp
        
        tests/SIMD/XSGTestSIMD.hpp
        tests/SIMD/XSSIMDTraitsTest.cpp
        tests/SIMD/XSSIMDBoolTest.cpp
        tests/SIMD/XSSIMDBaseTest.cpp
        tests/SIMD/XSSIMDInBaseTest.cpp
        tests/SIMD/XSSIMD2Test.cpp
        tests/SIMD/XSSIMD3Test.cpp
        tests/SIMD/XSSIMD4Test.cpp
        tests/SIMD/XSSIMD6Test.cpp
        tests/SIMD/XSSIMD3x2Test.cpp
        tests/SIMD/XSSIMD8Test.cpp
        tests/SIMD/XSSIMD3x3Test.cpp
        tests/SIMD/XSSIMD12Test.cpp
        tests/SIMD/XSSIMD3x4Test.cpp
        tests/SIMD/XSSIMD16Test.cpp
        
        tests/Geometry/XSGTestGeometry.hpp
        tests/Geometry/XSAABoundingBoxTest.cpp
        tests/Geometry/XSAABoundingBox2Test.cpp
        tests/Geometry/XSAABoundingBox4Test.cpp
        tests/Geometry/XSMatrix3x3Test.cpp
        tests/Geometry/XSMatrix4x3Test.cpp
        tests/Geometry/XSMatrix4x4Test.cpp
        tests/Geometry/XSPoint3DTest.cpp
        tests/Geometry/XSPoint3D2Test.cpp
        tests/Geometry/XSPoint3D4Test.cpp
        tests/Geometry/XSQuaternionTest.cpp
        tests/Geometry/XSRayTest.cpp
        tests/Geometry/XSRay2Test.cpp
        tests/Geometry/XSRay4Test.cpp
        tests/Geometry/XSRangeTest.cpp
        tests/Geometry/XSRange2Test.cpp
        tests/Geometry/XSRange4Test.cpp
        tests/Geometry/XSVector3DTest.cpp
        tests/Geometry/XSVector3D2Test.cpp
        tests/Geometry/XSVector3D4Test.cpp
        
        tests/Memory/XSMemoryTest.cpp
        tests/Memory/XSArrayTest.cpp
        tests/Memory/XSDArrayTest.cpp
        tests/Memory/XSStringTest.cpp
    )
    
    add_executable(ShiftLibTest)

    target_sources(ShiftLibTest PRIVATE ${SHIFTLIBTESTFILES})

    # XSTESTMAIN is used to signal if in main executable or ISA library
    target_compile_definitions(ShiftLibTest PRIVATE XSTESTMAIN)
    
    set_target_properties(
        ShiftLibTest PROPERTIES
        EXCLUDE_FROM_ALL true
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    target_include_directories(ShiftLibTest PRIVATE "tests/")

    target_link_libraries(ShiftLibTest
        PRIVATE GTest::GTest
        PRIVATE GTest::Main
        PRIVATE ShiftLib
    )
    
    # Enable all compile warnings
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror>)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
            target_compile_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4 /WX /experimental:external /external:anglebrackets /external:W0 /analyze:external- /std:c++20 /MP>)
        else()
            target_compile_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror -wd1011>)
        endif()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4 /WX /experimental:external /external:anglebrackets /external:W0 /analyze:external- /std:c++20 /MP /wd6326>)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
            target_compile_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W3 /WX /std:c++20>)
        else()
            target_compile_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror>)
        endif()
    endif()

    # Create a list of tested compiler options
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list(APPEND SHIFTLIBTESTOPTIONSTAG IA32 SSE3 SSE4 AVX AVX2 AVX512)
        list(APPEND SHIFTLIBTESTOPTIONS -march=i586 -march=core2 -march=nehalem -march=sandybridge -march=haswell -march=skylake-avx512)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
            list(APPEND SHIFTLIBTESTOPTIONSTAG IA32 SSE3 SSE4 AVX AVX2 AVX512)
            list(APPEND SHIFTLIBTESTOPTIONS "/arch:IA32 /DXS_IGNORE_ISA_OPT" /arch:SSE2 /arch:SSE4.2 /arch:CORE-AVX-I /arch:CORE-AVX2 /arch:SKYLAKE-AVX512)
        else()
            list(APPEND SHIFTLIBTESTOPTIONSTAG IA32 SSE3 SSE4 AVX AVX2 AVX512)
            list(APPEND SHIFTLIBTESTOPTIONS march=i586 -march=core2 -march=nehalem -march=sandybridge -march=haswell -march=skylake-avx512)
        endif()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        list(APPEND SHIFTLIBTESTOPTIONSTAG IA32 SSE3 AVX AVX2 AVX512)
        list(APPEND SHIFTLIBTESTOPTIONS "/arch:IA32 /DXS_IGNORE_ISA_OPT" /arch:SSE2 /arch:AVX /arch:AVX2 /arch:AVX512)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
            list(APPEND SHIFTLIBTESTOPTIONSTAG IA32 SSE3 AVX AVX2 AVX512)
            list(APPEND SHIFTLIBTESTOPTIONS /arch:IA32 /W4 /arch:AVX /arch:AVX2 /arch:AVX512)
        else()
            list(APPEND SHIFTLIBTESTOPTIONSTAG IA32 SSE3 SSE4 AVX AVX2 AVX512)
            list(APPEND SHIFTLIBTESTOPTIONS march=i586 -march=core2 -march=nehalem -march=sandybridge -march=haswell -march=skylake-avx512)
        endif()
    endif()

    #Add an library for each tested set of compiler options
    foreach(SHIFTLIBCOMPILE ${SHIFTLIBTESTOPTIONSTAG})
        add_library(ShiftLibTest_${SHIFTLIBCOMPILE} STATIC)        
        
        target_sources(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE ${SHIFTLIBTESTFILES})

        set_target_properties(
            ShiftLibTest_${SHIFTLIBCOMPILE} PROPERTIES
            EXCLUDE_FROM_ALL true
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )

        target_include_directories(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE "tests/")

        target_link_libraries(ShiftLibTest_${SHIFTLIBCOMPILE}
            PRIVATE GTest::GTest
            PRIVATE ShiftLib
        )
    
        # Enable all compile warnings and add any extra required linker flags to ensure that the static libs are correctly included in final program
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            target_compile_options(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror>)
            target_link_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:--whole-archive ShiftLibTest_${SHIFTLIBCOMPILE} --no-whole-archive>)
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
            if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
                target_compile_options(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4 /WX /experimental:external /external:anglebrackets /external:W0 /analyze:external- /std:c++20 /bigobj /MP>)
                target_link_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/WHOLEARCHIVE:ShiftLibTest_${SHIFTLIBCOMPILE}>)
            else()
                target_compile_options(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror -wd1011>)
                target_link_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:--whole-archive ShiftLibTest_${SHIFTLIBCOMPILE} --no-whole-archive>)
            endif()
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            target_compile_options(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4 /WX /experimental:external /external:anglebrackets /external:W0 /analyze:external- /std:c++20 /bigobj /MP /wd6326>)
            target_link_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/WHOLEARCHIVE:ShiftLibTest_${SHIFTLIBCOMPILE}>)
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
                target_compile_options(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W3 /WX /std:c++20 /bigobj>)
                target_link_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/WHOLEARCHIVE:ShiftLibTest_${SHIFTLIBCOMPILE}>)
            else()
                target_compile_options(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic -Werror>)
                target_link_options(ShiftLibTest PRIVATE $<$<COMPILE_LANGUAGE:CXX>:--whole-archive ShiftLibTest_${SHIFTLIBCOMPILE} --no-whole-archive>)
            endif()
        endif()

        # Add compiler options from list to lib
        list(GET SHIFTLIBTESTOPTIONS 0 TESTOPTIONSTAG)
        separate_arguments(TESTOPTIONSTAG)
        foreach(TESTOPTIONS ${TESTOPTIONSTAG})
            target_compile_options(ShiftLibTest_${SHIFTLIBCOMPILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${TESTOPTIONS}>)
        endforeach()
        list(REMOVE_AT SHIFTLIBTESTOPTIONS 0)        
        
        # Add the ISA dependent lib as dependency of main test program
        target_link_libraries(ShiftLibTest PRIVATE ShiftLibTest_${SHIFTLIBCOMPILE})
    endforeach()
    
    gtest_discover_tests(ShiftLibTest
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()


# Add benchmarks
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_BENCHMARKING)
    find_package(benchmark REQUIRED)
    
    #Source files are included in main executable and in dependent ISA specific static libs
    set(SHIFTLIBBENCHFILES
        benchmarks/XSBenchConfig.h
        benchmarks/Memory/XSMemoryBench.cpp
    )
    
    add_executable(ShiftLibBench)

    target_sources(ShiftLibBench PRIVATE ${SHIFTLIBBENCHFILES})

    # XSTESTMAIN is used to signal if in main executable or ISA library
    target_compile_definitions(ShiftLibBench PRIVATE XSBENCHMAIN)
    
    set_target_properties(
        ShiftLibBench PROPERTIES
        EXCLUDE_FROM_ALL true
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    target_include_directories(ShiftLibBench PRIVATE "benchmarks/")

    target_link_libraries(ShiftLibBench
        PRIVATE benchmark::benchmark
        PRIVATE benchmark::benchmark_main
        PRIVATE ShiftLib
    )

    # Create a list of benched compiler options
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list(APPEND SHIFTLIBBENCHOPTIONSTAG IA32 SSE3 SSE4 AVX AVX2 AVX512)
        list(APPEND SHIFTLIBBENCHOPTIONS -march=i586 -march=core2 -march=nehalem -march=sandybridge -march=haswell -march=skylake-avx512)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
            list(APPEND SHIFTLIBBENCHOPTIONSTAG IA32 SSE3 SSE4 AVX AVX2 AVX512)
            list(APPEND SHIFTLIBBENCHOPTIONS "/arch:IA32 /DXS_IGNORE_ISA_OPT" /arch:SSE2 /arch:SSE4.2 /arch:CORE-AVX-I /arch:CORE-AVX2 /arch:SKYLAKE-AVX512)
        else()
            list(APPEND SHIFTLIBBENCHOPTIONSTAG IA32 SSE3 SSE4 AVX AVX2 AVX512)
            list(APPEND SHIFTLIBBENCHOPTIONS march=i586 -march=core2 -march=nehalem -march=sandybridge -march=haswell -march=skylake-avx512)
        endif()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        list(APPEND SHIFTLIBBENCHOPTIONSTAG IA32 SSE3 AVX AVX2 AVX512)
        list(APPEND SHIFTLIBBENCHOPTIONS "/arch:IA32 /DXS_IGNORE_ISA_OPT" /arch:SSE2 /arch:AVX /arch:AVX2 /arch:AVX512)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
            list(APPEND SHIFTLIBBENCHOPTIONSTAG IA32 SSE3 AVX AVX2 AVX512)
            list(APPEND SHIFTLIBBENCHOPTIONS /arch:IA32 /W3 /arch:AVX /arch:AVX2 /arch:AVX512)
        else()
            list(APPEND SHIFTLIBBENCHOPTIONSTAG IA32 SSE3 SSE4 AVX AVX2 AVX512)
            list(APPEND SHIFTLIBBENCHOPTIONS march=i586 -march=core2 -march=nehalem -march=sandybridge -march=haswell -march=skylake-avx512)
        endif()
    endif()

    #Add an library for each set of compiler options
    foreach(SHIFTLIBCOMPILE ${SHIFTLIBBENCHOPTIONSTAG})
        add_library(ShiftLibBench_${SHIFTLIBCOMPILE} STATIC)        
        
        target_sources(ShiftLibBench_${SHIFTLIBCOMPILE} PRIVATE ${SHIFTLIBBENCHFILES})

        set_target_properties(
            ShiftLibBench_${SHIFTLIBCOMPILE} PROPERTIES
            EXCLUDE_FROM_ALL true
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )

        target_include_directories(ShiftLibBench_${SHIFTLIBCOMPILE} PRIVATE "benchmarks/")

        target_link_libraries(ShiftLibBench_${SHIFTLIBCOMPILE}
            PRIVATE benchmark::benchmark
            PRIVATE ShiftLib
        )
    
        # Add any extra required linker flags to ensure that the static libs are correctly included in final program
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            target_link_options(ShiftLibBench PRIVATE $<$<COMPILE_LANGUAGE:CXX>:--whole-archive ShiftLibBench_${SHIFTLIBCOMPILE} --no-whole-archive>)
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
            if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
                target_link_options(ShiftLibBench PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/WHOLEARCHIVE:ShiftLibBench_${SHIFTLIBCOMPILE}>)
            else()
                target_link_options(ShiftLibBench PRIVATE $<$<COMPILE_LANGUAGE:CXX>:--whole-archive ShiftLibBench_${SHIFTLIBCOMPILE} --no-whole-archive>)
            endif()
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            target_link_options(ShiftLibBench PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/WHOLEARCHIVE:ShiftLibBench_${SHIFTLIBCOMPILE}>)
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            if ("${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
                target_link_options(ShiftLibBench PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/WHOLEARCHIVE:ShiftLibBench_${SHIFTLIBCOMPILE}>)
            else()
                target_link_options(ShiftLibBench PRIVATE $<$<COMPILE_LANGUAGE:CXX>:--whole-archive ShiftLibBench_${SHIFTLIBCOMPILE} --no-whole-archive>)
            endif()
        endif()

        # Add compiler options from list to lib
        list(GET SHIFTLIBBENCHOPTIONS 0 BENCHOPTIONSTAG)
        separate_arguments(BENCHOPTIONSTAG)
        foreach(BENCHOPTIONS ${BENCHOPTIONSTAG})
            target_compile_options(ShiftLibBench_${SHIFTLIBCOMPILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${BENCHOPTIONS}>)
        endforeach()
        list(REMOVE_AT SHIFTLIBBENCHOPTIONS 0)        
        
        # Add the ISA dependent lib as dependency of main bench program
        target_link_libraries(ShiftLibBench PRIVATE ShiftLibBench_${SHIFTLIBCOMPILE})
    endforeach()
endif()
