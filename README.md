ShiftLib
=============
[![GitHub issues](https://img.shields.io/github/issues/Sibras/ShiftIntrinsicGuide.svg)](https://github.com/Sibras/ShiftIntrinsicGuide/issues)
[![license](https://img.shields.io/github/license/Sibras/ShiftIntrinsicGuide.svg)](https://github.com/Sibras/ShiftIntrinsicGuide)
## ShiftLib

ShiftLib is a general purpose cross platform C++ library providing multiple mathematics, memory and utility functions

## Features

- Optimised general purpose math functions
- Optimised bit arithmetic operations
- Signed and Unsigned 128 bit integers
- Rational number type
- Type traits (alternate to std::)
- Compiler and architecture detection
- Optimised SIMD template types
    - Configurable platform optimisations (SSE3, SSE4.1, AVX, AVX2, AVX512)
    - Scalar fallback (CUDA compatible)
    - Multiple SIMD type sizes:
        - SIMD2
        - SIMD3
        - SIMD4
        - SIMD8
        - SIMD16
        - SIMD12
        - SIMD3x2
        - SIMD3x3
        - SIMD3x4
    - Optimised trigonometric functions (float/double types only)
    - General arithmetic operations, load-store, horizontal, shuffle functions
- Optimised Geometry template types
    - Matrix3x3
    - Matrix4x3
    - Matrix4x4
    - Quaternion
    - Point3D + Vector3D
    - Packed Point3Dx2 + Vector3Dx2
    - Packed Point3Dx4 + Vector3Dx4
    - AABoundingBox (axis aligned)
    - Packed AABoundingBox2, AABoundingBox4
    - Ray
    - Packed Ray2, Ray4
    - Range
    - Packed Range2, Range4
- Custom memory allocators and containers
    - OS based heap allocator
    - Stack allocator
    - Dynamic array template type
    - Fixed array template type
    - Optimised memory copy functions
    - CUDA memory allocation and transfer support

## Issues

Any issues related to ShiftLib should be sent to the [issues](https://github.com/Sibras/ShiftLib/issues) page for the repository.

## License

ShiftLib original code is released under [Apache 2.0](https://opensource.org/licenses/Apache-2.0).

## Copyright

For a complete copyright list please checkout the source code to examine license headers. Unless expressly stated otherwise all code submitted to ShiftLib (in any form) is licensed under [Apache 2.0](https://opensource.org/licenses/Apache-2.0) and copyright is donated to ShiftLib. If you submit code that is not your own work it is your responsibility to place a header stating the copyright.

## Contributing

Patches related to ShiftLib should be sent as pull requests to the main repository.