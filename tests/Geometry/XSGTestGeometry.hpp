#pragma once
/**
 * Copyright Matthew Oliver
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "XSGTest.hpp"

#ifdef XS_OVERRIDE_SHIFT_NS
namespace XS_OVERRIDE_SHIFT_NS {
#endif
#ifdef XS_TESTING_AABB2
#    ifndef XS_TESTING_POINT3D2
#        define XS_TESTING_POINT3D2 // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSAABoundingBox2.hpp"
#endif
#ifdef XS_TESTING_AABB4
#    ifndef XS_TESTING_POINT3D4
#        define XS_TESTING_POINT3D4 // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSAABoundingBox4.hpp"
#endif
#ifdef XS_TESTING_AABB
#    ifndef XS_TESTING_POINT3D
#        define XS_TESTING_POINT3D // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSAABoundingBox.hpp"
#endif
#ifdef XS_TESTING_RAY
#    ifndef XS_TESTING_POINT3D
#        define XS_TESTING_POINT3D // Needed for PrinTo(row)
#    endif
#    ifndef XS_TESTING_VECTOR3D
#        define XS_TESTING_VECTOR3D
#    endif
#    include "Geometry/XSRay.hpp"
#endif
#ifdef XS_TESTING_RAY2
#    ifndef XS_TESTING_POINT3D2
#        define XS_TESTING_POINT3D2 // Needed for PrinTo(row)
#    endif
#    ifndef XS_TESTING_VECTOR3D2
#        define XS_TESTING_VECTOR3D2
#    endif
#    include "Geometry/XSRay2.hpp"
#endif
#ifdef XS_TESTING_RAY4
#    ifndef XS_TESTING_POINT3D4
#        define XS_TESTING_POINT3D4 // Needed for PrinTo(row)
#    endif
#    ifndef XS_TESTING_VECTOR3D2
#        define XS_TESTING_VECTOR3D2
#    endif
#    include "Geometry/XSRay4.hpp"
#endif
#ifdef XS_TESTING_MATRIX3X3
#    ifndef XS_TESTING_SIMD3
#        define XS_TESTING_SIMD3 // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSMatrix3x3.hpp"
#endif
#ifdef XS_TESTING_MATRIX3X4
#    ifndef XS_TESTING_SIMD4
#        define XS_TESTING_SIMD4 // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSMatrix3x4.hpp"
#endif
#ifdef XS_TESTING_MATRIX4X4
#    ifndef XS_TESTING_SIMD4
#        define XS_TESTING_SIMD4 // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSMatrix4x4.hpp"
#endif
#ifdef XS_TESTING_POINT3D2
#    ifndef XS_TESTING_POINT3D
#        define XS_TESTING_POINT3D // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSPoint3D2.hpp"
#endif
#ifdef XS_TESTING_POINT3D4
#    ifndef XS_TESTING_POINT3D
#        define XS_TESTING_POINT3D // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSPoint3D4.hpp"
#endif
#ifdef XS_TESTING_POINT3D
#    include "Geometry/XSPoint3D.hpp"
#endif
#ifdef XS_TESTING_QUATERNION
#    include "Geometry/XSQuaternion.hpp"
#endif
#ifdef XS_TESTING_RANGE
#    include "Geometry/XSRange.hpp"
#endif
#ifdef XS_TESTING_RANGE2
#    include "Geometry/XSRange2.hpp"
#endif
#ifdef XS_TESTING_RANGE4
#    include "Geometry/XSRange4.hpp"
#endif
#ifdef XS_TESTING_TRANSFORM
#    include "Geometry/XSTransform.hpp"
#endif
#ifdef XS_TESTING_VECTOR3D2
#    ifndef XS_TESTING_VECTOR3D
#        define XS_TESTING_VECTOR3D // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSVector3D2.hpp"
#endif
#ifdef XS_TESTING_VECTOR3D4
#    ifndef XS_TESTING_VECTOR3D
#        define XS_TESTING_VECTOR3D // Needed for PrinTo(row)
#    endif
#    include "Geometry/XSVector3D4.hpp"
#endif
#ifdef XS_TESTING_VECTOR3D
#    include "Geometry/XSVector3D.hpp"
#endif

#ifndef XS_TESTING_INBASE
#    define XS_TESTING_INBASE // Needed for getValueInBase
#endif

namespace Shift {
#ifdef XS_TESTING_AABB2
template<typename T, SIMDWidth Width, bool Packed>
void PrintTo(const AABoundingBox2<T, Width, Packed>& param, std::ostream* os)
{
    auto val0 = param.minPoints.template getPoint<0>();
    auto val1 = param.minPoints.template getPoint<1>();
    auto val2 = param.maxPoints.template getPoint<0>();
    auto val3 = param.maxPoints.template getPoint<1>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val2, os);
    PrintTo(val1, os);
    PrintTo(val3, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_AABB4
template<typename T, SIMDWidth Width, bool Packed>
void PrintTo(const AABoundingBox4<T, Width, Packed>& param, std::ostream* os)
{
    auto val0 = param.minPoints.template getPoint<0>();
    auto val1 = param.minPoints.template getPoint<1>();
    auto val2 = param.minPoints.template getPoint<2>();
    auto val3 = param.minPoints.template getPoint<3>();
    auto val4 = param.maxPoints.template getPoint<0>();
    auto val5 = param.maxPoints.template getPoint<1>();
    auto val6 = param.maxPoints.template getPoint<2>();
    auto val7 = param.maxPoints.template getPoint<3>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val4, os);
    PrintTo(val1, os);
    PrintTo(val5, os);
    PrintTo(val2, os);
    PrintTo(val6, os);
    PrintTo(val3, os);
    PrintTo(val7, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_AABB
template<typename T, SIMDWidth Width>
void PrintTo(const AABoundingBox<T, Width>& param, std::ostream* os)
{
    *os << "(";
    PrintTo(param.minPoint, os);
    PrintTo(param.maxPoint, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_POINT3D2
template<typename T, SIMDWidth Width, bool Packed>
void PrintTo(const Point3D2<T, Width, Packed>& param, std::ostream* os)
{
    auto val0 = param.template getPoint<0>();
    auto val1 = param.template getPoint<1>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_POINT3D4
template<typename T, SIMDWidth Width, bool Packed>
void PrintTo(const Point3D4<T, Width, Packed>& param, std::ostream* os)
{
    auto val0 = param.template getPoint<0>();
    auto val1 = param.template getPoint<1>();
    auto val2 = param.template getPoint<2>();
    auto val3 = param.template getPoint<3>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    PrintTo(val2, os);
    PrintTo(val3, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_POINT3D
template<typename T, SIMDWidth Width>
void PrintTo(const Point3D<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_QUATERNION
template<typename T, SIMDWidth Width>
void PrintTo(const Quaternion<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ", "
        << param.template getValueInBase<3>().getValue() << ")";
}
#endif

#ifdef XS_TESTING_MATRIX3X3
template<typename T, SIMDWidth Width>
void PrintTo(const Matrix3x3<T, Width>& param, std::ostream* os)
{
    auto val0 = param.template getRow<0>();
    auto val1 = param.template getRow<1>();
    auto val2 = param.template getRow<2>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    PrintTo(val2, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_MATRIX3X4
template<typename T, SIMDWidth Width>
void PrintTo(const Matrix3x4<T, Width>& param, std::ostream* os)
{
    auto val0 = param.template getRow<0>();
    auto val1 = param.template getRow<1>();
    auto val2 = param.template getRow<2>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    PrintTo(val2, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_MATRIX4X4
template<typename T, SIMDWidth Width>
void PrintTo(const Matrix4x4<T, Width>& param, std::ostream* os)
{
    auto val0 = param.template getRow<0>();
    auto val1 = param.template getRow<1>();
    auto val2 = param.template getRow<2>();
    auto val3 = param.template getRow<3>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    PrintTo(val2, os);
    PrintTo(val3, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_RANGE2
template<typename T, SIMDWidth Width>
void PrintTo(const Range2<T, Width>& param, std::ostream* os)
{
    auto val0 = param.template getMinInBase<0>();
    auto val1 = param.template getMaxInBase<0>();
    auto val2 = param.template getMinInBase<1>();
    auto val3 = param.template getMaxInBase<1>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    PrintTo(val2, os);
    PrintTo(val3, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_RANGE4
template<typename T, SIMDWidth Width>
void PrintTo(const Range4<T, Width>& param, std::ostream* os)
{
    auto val0 = param.template getMinInBase<0>();
    auto val1 = param.template getMaxInBase<0>();
    auto val2 = param.template getMinInBase<1>();
    auto val3 = param.template getMaxInBase<1>();
    auto val4 = param.template getMinInBase<2>();
    auto val5 = param.template getMaxInBase<2>();
    auto val6 = param.template getMinInBase<3>();
    auto val7 = param.template getMaxInBase<3>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    PrintTo(val2, os);
    PrintTo(val3, os);
    PrintTo(val4, os);
    PrintTo(val5, os);
    PrintTo(val6, os);
    PrintTo(val7, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_RANGE
template<typename T, SIMDWidth Width>
void PrintTo(const Range<T, Width>& param, std::ostream* os)
{
    auto val0 = param.template getMinInBase<0>();
    auto val1 = param.template getMaxInBase<0>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_RAY2
template<typename T, SIMDWidth Width, bool Packed>
void PrintTo(const Ray2<T, Width, Packed>& param, std::ostream* os)
{
    *os << "(";
    PrintTo(param.origins, os);
    PrintTo(param.directions, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_RAY4
template<typename T, SIMDWidth Width, bool Packed>
void PrintTo(const Ray4<T, Width, Packed>& param, std::ostream* os)
{
    *os << "(";
    PrintTo(param.origins, os);
    PrintTo(param.directions, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_RAY
template<typename T, SIMDWidth Width>
void PrintTo(const Ray<T, Width>& param, std::ostream* os)
{
    *os << "(";
    PrintTo(param.origin, os);
    PrintTo(param.direction, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_VECTOR3D2
template<typename T, SIMDWidth Width, bool Packed>
void PrintTo(const Vector3D2<T, Width, Packed>& param, std::ostream* os)
{
    auto val0 = param.template getVector<0>();
    auto val1 = param.template getVector<1>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_VECTOR3D4
template<typename T, SIMDWidth Width, bool Packed>
void PrintTo(const Vector3D4<T, Width, Packed>& param, std::ostream* os)
{
    auto val0 = param.template getVector<0>();
    auto val1 = param.template getVector<1>();
    auto val2 = param.template getVector<2>();
    auto val3 = param.template getVector<3>();
    *os << "(";
    PrintTo(val0, os);
    PrintTo(val1, os);
    PrintTo(val2, os);
    PrintTo(val3, os);
    *os << ")";
}
#endif

#ifdef XS_TESTING_VECTOR3D
template<typename T, SIMDWidth Width>
void PrintTo(const Vector3D<T, Width>& param, std::ostream* os)
{
    *os << "(" << param.template getValueInBase<0>().getValue() << ", " << param.template getValueInBase<1>().getValue()
        << ", " << param.template getValueInBase<2>().getValue() << ")";
}
#endif
} // namespace Shift
#ifdef XS_OVERRIDE_SHIFT_NS
} // namespace XS_OVERRIDE_SHIFT_NS
#endif

#include "SIMD/XSGTestSIMD.hpp"

#ifdef XS_TESTING_AABB
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertAABB(
    const XS_OVERRIDE_SHIFT_NS::Shift::AABoundingBox<T, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5)
{
    if (testValue(param.minPoint.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.minPoint.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.minPoint.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.maxPoint.template getValueInBase<0>().getValue(), v3) &&
        testValue(param.maxPoint.template getValueInBase<1>().getValue(), v4) &&
        testValue(param.maxPoint.template getValueInBase<2>().getValue(), v5)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_AABB2
template<typename T, bool Packed, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertAABB2(const XS_OVERRIDE_SHIFT_NS::Shift::AABoundingBox2<T, Packed, Width>& param, T v0,
    T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11)
{
    auto val0 = param.minPoints.template getPoint<0>();
    auto val1 = param.maxPoints.template getPoint<0>();
    auto val2 = param.minPoints.template getPoint<1>();
    auto val3 = param.maxPoints.template getPoint<1>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val1.template getValueInBase<0>().getValue(), v3) &&
        testValue(val1.template getValueInBase<1>().getValue(), v4) &&
        testValue(val1.template getValueInBase<2>().getValue(), v5) &&
        testValue(val2.template getValueInBase<0>().getValue(), v6) &&
        testValue(val2.template getValueInBase<1>().getValue(), v7) &&
        testValue(val2.template getValueInBase<2>().getValue(), v8) &&
        testValue(val3.template getValueInBase<0>().getValue(), v9) &&
        testValue(val3.template getValueInBase<1>().getValue(), v10) &&
        testValue(val3.template getValueInBase<2>().getValue(), v11)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_AABB4
template<typename T, bool Packed, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertAABB4(const XS_OVERRIDE_SHIFT_NS::Shift::AABoundingBox4<T, Packed, Width>& param, T v0,
    T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15, T v16, T v17, T v18,
    T v19, T v20, T v21, T v22, T v23)
{
    auto val0 = param.minPoints.template getPoint<0>();
    auto val1 = param.maxPoints.template getPoint<0>();
    auto val2 = param.minPoints.template getPoint<1>();
    auto val3 = param.maxPoints.template getPoint<1>();
    auto val4 = param.minPoints.template getPoint<2>();
    auto val5 = param.maxPoints.template getPoint<2>();
    auto val6 = param.minPoints.template getPoint<3>();
    auto val7 = param.maxPoints.template getPoint<3>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val1.template getValueInBase<0>().getValue(), v3) &&
        testValue(val1.template getValueInBase<1>().getValue(), v4) &&
        testValue(val1.template getValueInBase<2>().getValue(), v5) &&
        testValue(val2.template getValueInBase<0>().getValue(), v6) &&
        testValue(val2.template getValueInBase<1>().getValue(), v7) &&
        testValue(val2.template getValueInBase<2>().getValue(), v8) &&
        testValue(val3.template getValueInBase<0>().getValue(), v9) &&
        testValue(val3.template getValueInBase<1>().getValue(), v10) &&
        testValue(val3.template getValueInBase<2>().getValue(), v11) &&
        testValue(val4.template getValueInBase<0>().getValue(), v12) &&
        testValue(val4.template getValueInBase<1>().getValue(), v13) &&
        testValue(val4.template getValueInBase<2>().getValue(), v14) &&
        testValue(val5.template getValueInBase<0>().getValue(), v15) &&
        testValue(val5.template getValueInBase<1>().getValue(), v16) &&
        testValue(val5.template getValueInBase<2>().getValue(), v17) &&
        testValue(val6.template getValueInBase<0>().getValue(), v18) &&
        testValue(val6.template getValueInBase<1>().getValue(), v19) &&
        testValue(val6.template getValueInBase<2>().getValue(), v20) &&
        testValue(val7.template getValueInBase<0>().getValue(), v21) &&
        testValue(val7.template getValueInBase<1>().getValue(), v22) &&
        testValue(val7.template getValueInBase<2>().getValue(), v23)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_POINT3D
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertPoint3D(const XS_OVERRIDE_SHIFT_NS::Shift::Point3D<T, Width>& param, T v0, T v1, T v2)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_POINT3D2
template<typename T, bool Packed, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertPoint3D2(
    const XS_OVERRIDE_SHIFT_NS::Shift::Point3D2<T, Packed, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5)
{
    auto val0 = param.template getPoint<0>();
    auto val1 = param.template getPoint<1>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val1.template getValueInBase<0>().getValue(), v3) &&
        testValue(val1.template getValueInBase<1>().getValue(), v4) &&
        testValue(val1.template getValueInBase<2>().getValue(), v5)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_POINT3D4
template<typename T, bool Packed, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertPoint3D4(const XS_OVERRIDE_SHIFT_NS::Shift::Point3D4<T, Packed, Width>& param, T v0,
    T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11)
{
    auto val0 = param.template getPoint<0>();
    auto val1 = param.template getPoint<1>();
    auto val2 = param.template getPoint<2>();
    auto val3 = param.template getPoint<3>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val1.template getValueInBase<0>().getValue(), v3) &&
        testValue(val1.template getValueInBase<1>().getValue(), v4) &&
        testValue(val1.template getValueInBase<2>().getValue(), v5) &&
        testValue(val2.template getValueInBase<0>().getValue(), v6) &&
        testValue(val2.template getValueInBase<1>().getValue(), v7) &&
        testValue(val2.template getValueInBase<2>().getValue(), v8) &&
        testValue(val3.template getValueInBase<0>().getValue(), v9) &&
        testValue(val3.template getValueInBase<1>().getValue(), v10) &&
        testValue(val3.template getValueInBase<2>().getValue(), v11)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_QUATERNION
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertQuaternion(
    const XS_OVERRIDE_SHIFT_NS::Shift::Quaternion<T, Width>& param, T v0, T v1, T v2, T v3)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2) &&
        testValue(param.template getValueInBase<3>().getValue(), v3)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_MATRIX3X3
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertMatrix3x3(
    const XS_OVERRIDE_SHIFT_NS::Shift::Matrix3x3<T, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8)
{
    auto val0 = param.template getRow<0>();
    auto val1 = param.template getRow<1>();
    auto val2 = param.template getRow<2>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val1.template getValueInBase<0>().getValue(), v3) &&
        testValue(val1.template getValueInBase<1>().getValue(), v4) &&
        testValue(val1.template getValueInBase<2>().getValue(), v5) &&
        testValue(val2.template getValueInBase<0>().getValue(), v6) &&
        testValue(val2.template getValueInBase<1>().getValue(), v7) &&
        testValue(val2.template getValueInBase<2>().getValue(), v8)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_MATRIX3X4
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertMatrix3x4(const XS_OVERRIDE_SHIFT_NS::Shift::Matrix3x4<T, Width>& param, T v0, T v1,
    T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11)
{
    auto val0 = param.template getRow<0>();
    auto val1 = param.template getRow<1>();
    auto val2 = param.template getRow<2>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val0.template getValueInBase<3>().getValue(), v3) &&
        testValue(val1.template getValueInBase<0>().getValue(), v4) &&
        testValue(val1.template getValueInBase<1>().getValue(), v5) &&
        testValue(val1.template getValueInBase<2>().getValue(), v6) &&
        testValue(val1.template getValueInBase<3>().getValue(), v7) &&
        testValue(val2.template getValueInBase<0>().getValue(), v8) &&
        testValue(val2.template getValueInBase<1>().getValue(), v9) &&
        testValue(val2.template getValueInBase<2>().getValue(), v10) &&
        testValue(val2.template getValueInBase<3>().getValue(), v11)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_MATRIX4X4
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertMatrix4x4(const XS_OVERRIDE_SHIFT_NS::Shift::Matrix4x4<T, Width>& param, T v0, T v1,
    T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15)
{
    auto val0 = param.template getRow<0>();
    auto val1 = param.template getRow<1>();
    auto val2 = param.template getRow<2>();
    auto val3 = param.template getRow<3>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val0.template getValueInBase<3>().getValue(), v3) &&
        testValue(val1.template getValueInBase<0>().getValue(), v4) &&
        testValue(val1.template getValueInBase<1>().getValue(), v5) &&
        testValue(val1.template getValueInBase<2>().getValue(), v6) &&
        testValue(val1.template getValueInBase<3>().getValue(), v7) &&
        testValue(val2.template getValueInBase<0>().getValue(), v8) &&
        testValue(val2.template getValueInBase<1>().getValue(), v9) &&
        testValue(val2.template getValueInBase<2>().getValue(), v10) &&
        testValue(val2.template getValueInBase<3>().getValue(), v11) &&
        testValue(val3.template getValueInBase<0>().getValue(), v12) &&
        testValue(val3.template getValueInBase<1>().getValue(), v13) &&
        testValue(val3.template getValueInBase<2>().getValue(), v14) &&
        testValue(val3.template getValueInBase<3>().getValue(), v15)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_RANGE
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertRange(const XS_OVERRIDE_SHIFT_NS::Shift::Range<T, Width>& param, T v0, T v1)
{
    if (testValue(param.template getMinInBase().getValue(), v0) &&
        testValue(param.template getMaxInBase().getValue(), v1)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_RANGE2
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertRange2(
    const XS_OVERRIDE_SHIFT_NS::Shift::Range2<T, Width>& param, T v0, T v1, T v2, T v3)
{
    if (testValue(param.template getMinInBase<0>().getValue(), v0) &&
        testValue(param.template getMinInBase<1>().getValue(), v1) &&
        testValue(param.template getMaxInBase<0>().getValue(), v2) &&
        testValue(param.template getMaxInBase<1>().getValue(), v3)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_RANGE4
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertRange4(
    const XS_OVERRIDE_SHIFT_NS::Shift::Range4<T, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
{
    if (testValue(param.template getMinInBase<0>().getValue(), v0) &&
        testValue(param.template getMinInBase<1>().getValue(), v1) &&
        testValue(param.template getMinInBase<2>().getValue(), v2) &&
        testValue(param.template getMinInBase<3>().getValue(), v3) &&
        testValue(param.template getMaxInBase<0>().getValue(), v4) &&
        testValue(param.template getMaxInBase<1>().getValue(), v5) &&
        testValue(param.template getMaxInBase<2>().getValue(), v6) &&
        testValue(param.template getMaxInBase<3>().getValue(), v7)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_RAY
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertRay(
    const XS_OVERRIDE_SHIFT_NS::Shift::Ray<T, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5)
{
    if (testValue(param.origin.template getMinInBase<0>().getValue(), v0) &&
        testValue(param.origin.template getMinInBase<1>().getValue(), v1) &&
        testValue(param.origin.template getMinInBase<2>().getValue(), v2) &&
        testValue(param.direction.template getMinInBase<3>().getValue(), v3) &&
        testValue(param.direction.template getMaxInBase<0>().getValue(), v4) &&
        testValue(param.direction.template getMaxInBase<1>().getValue(), v5)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_RAY2
template<typename T, bool Packed, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertRay2(const XS_OVERRIDE_SHIFT_NS::Shift::Ray2<T, Packed, Width>& param, T v0, T v1,
    T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11)
{
    auto val0 = param.origins.template getPoint<0>();
    auto val1 = param.origins.template getPoint<1>();
    auto val2 = param.directions.template getVector<0>();
    auto val3 = param.directions.template getVector<1>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val1.template getValueInBase<0>().getValue(), v3) &&
        testValue(val1.template getValueInBase<1>().getValue(), v4) &&
        testValue(val1.template getValueInBase<2>().getValue(), v5) &&
        testValue(val2.template getValueInBase<0>().getValue(), v6) &&
        testValue(val2.template getValueInBase<1>().getValue(), v7) &&
        testValue(val2.template getValueInBase<2>().getValue(), v8) &&
        testValue(val3.template getValueInBase<0>().getValue(), v9) &&
        testValue(val3.template getValueInBase<1>().getValue(), v10) &&
        testValue(val3.template getValueInBase<2>().getValue(), v11)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_RAY4
// Requires to many arguments, instead test by testing each point individually
#endif

#ifdef XS_TESTING_VECTOR3D
template<typename T, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertVector3D(
    const XS_OVERRIDE_SHIFT_NS::Shift::Vector3D<T, Width>& param, T v0, T v1, T v2)
{
    if (testValue(param.template getValueInBase<0>().getValue(), v0) &&
        testValue(param.template getValueInBase<1>().getValue(), v1) &&
        testValue(param.template getValueInBase<2>().getValue(), v2)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_VECTOR3D2
template<typename T, bool Packed, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertVector3D2(
    const XS_OVERRIDE_SHIFT_NS::Shift::Vector3D2<T, Packed, Width>& param, T v0, T v1, T v2, T v3, T v4, T v5)
{
    auto val0 = param.template getVector<0>();
    auto val1 = param.template getVector<1>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val1.template getValueInBase<0>().getValue(), v3) &&
        testValue(val1.template getValueInBase<1>().getValue(), v4) &&
        testValue(val1.template getValueInBase<2>().getValue(), v5)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif

#ifdef XS_TESTING_VECTOR3D4
template<typename T, bool Packed, XS_OVERRIDE_SHIFT_NS::Shift::SIMDWidth Width>
::testing::AssertionResult assertVector3D4(const XS_OVERRIDE_SHIFT_NS::Shift::Vector3D4<T, Packed, Width>& param, T v0,
    T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11)
{
    auto val0 = param.template getVector<0>();
    auto val1 = param.template getVector<1>();
    auto val2 = param.template getVector<2>();
    auto val3 = param.template getVector<3>();
    if (testValue(val0.template getValueInBase<0>().getValue(), v0) &&
        testValue(val0.template getValueInBase<1>().getValue(), v1) &&
        testValue(val0.template getValueInBase<2>().getValue(), v2) &&
        testValue(val1.template getValueInBase<0>().getValue(), v3) &&
        testValue(val1.template getValueInBase<1>().getValue(), v4) &&
        testValue(val1.template getValueInBase<2>().getValue(), v5) &&
        testValue(val2.template getValueInBase<0>().getValue(), v6) &&
        testValue(val2.template getValueInBase<1>().getValue(), v7) &&
        testValue(val2.template getValueInBase<2>().getValue(), v8) &&
        testValue(val3.template getValueInBase<0>().getValue(), v9) &&
        testValue(val3.template getValueInBase<1>().getValue(), v10) &&
        testValue(val3.template getValueInBase<2>().getValue(), v11)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}
#endif
