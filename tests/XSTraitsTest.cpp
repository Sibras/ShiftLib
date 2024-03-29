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

// Must be included before traits for some reason
inline void swap(float&, float&) noexcept
{}

#include "XSTraits.hpp"

#include "XSCompilerOptions.h"
#include "XSGTest.hpp"
#include "XSInt128.hpp"

using namespace Shift;

class Test1;
class Test2;

#ifdef XSTESTMAIN
TEST_NS(Traits, Traits, RemoveConst)
{
    static_assert(isSameCV<removeConst<int32>, int32> == true);
    static_assert(isSameCV<removeConst<const int32>, int32> == true);
    static_assert(isSameCV<removeConst<const int32>, const int32> == false);
    static_assert(isSameCV<removeConst<const volatile int32>, volatile int32> == true);
}

TEST_NS(Traits, Traits, AddConst)
{
    static_assert(isSameCV<addConst<int32>, int32> == false);
    static_assert(isSameCV<addConst<const int32>, const int32> == true);
    static_assert(isSameCV<addConst<int32>, const int32> == true);
    static_assert(isSameCV<addConst<const int32>, volatile int32> == false);
    static_assert(isSameCV<addConst<volatile int32>, const volatile int32> == true);
}

TEST_NS(Traits, Traits, RemoveVolatile)
{
    static_assert(isSameCV<removeVolatile<int32>, int32> == true);
    static_assert(isSameCV<removeVolatile<volatile int32>, int32> == true);
    static_assert(isSameCV<removeVolatile<volatile int32>, volatile int32> == false);
    static_assert(isSameCV<removeVolatile<const volatile int32>, const int32> == true);
}

TEST_NS(Traits, Traits, AddVolatile)
{
    static_assert(isSameCV<addVolatile<int32>, int32> == false);
    static_assert(isSameCV<addVolatile<volatile int32>, volatile int32> == true);
    static_assert(isSameCV<addVolatile<int32>, volatile int32> == true);
    static_assert(isSameCV<addVolatile<volatile int32>, const int32> == false);
    static_assert(isSameCV<addVolatile<const int32>, const volatile int32> == true);
}

TEST_NS(Traits, Traits, RemoveCV)
{
    static_assert(isSameCV<removeCV<int32>, int32> == true);
    static_assert(isSameCV<removeCV<volatile int32>, int32> == true);
    static_assert(isSameCV<removeCV<volatile int32>, volatile int32> == false);
    static_assert(isSameCV<removeCV<const volatile int32>, const int32> == false);
    static_assert(isSameCV<removeCV<const volatile int32>, volatile int32> == false);
    static_assert(isSameCV<removeCV<const volatile int32>, int32> == true);
}

TEST_NS(Traits, Traits, AddCV)
{
    static_assert(isSameCV<addCV<int32>, int32> == false);
    static_assert(isSameCV<addCV<volatile int32>, volatile int32> == false);
    static_assert(isSameCV<addCV<int32>, volatile int32> == false);
    static_assert(isSameCV<addCV<int32>, const int32> == false);
    static_assert(isSameCV<addCV<volatile int32>, const int32> == false);
    static_assert(isSameCV<addCV<const int32>, const volatile int32> == true);
    static_assert(isSameCV<addCV<volatile int32>, const volatile int32> == true);
    static_assert(isSameCV<addCV<int32>, const volatile int32> == true);
}

TEST_NS(Traits, Traits, CopyCV)
{
    static_assert(isSameCV<copyCV<int32, const int32>, int32> == false);
    static_assert(isSameCV<copyCV<volatile int32, const int32>, volatile int32> == false);
    static_assert(isSameCV<copyCV<int32, const int32>, volatile int32> == false);
    static_assert(isSameCV<copyCV<int32, const int32>, const int32> == true);
    static_assert(isSameCV<copyCV<volatile int32, const int32>, const int32> == true);
    static_assert(isSameCV<copyCV<const int32, volatile int32>, const volatile int32> == false);
    static_assert(isSameCV<copyCV<volatile int32, volatile int32>, volatile int32> == true);
    static_assert(isSameCV<copyCV<int32, const volatile int32>, const volatile int32> == true);
}

TEST_NS(Traits, Traits, AddLRef)
{
    static_assert(isSameCV<addLRef<int32>, int32> == false);
    static_assert(isSameCV<addLRef<int32&>, int32&> == true);
    static_assert(isSameCV<addLRef<int32&&>, int32&> == true);
    static_assert(isSameCV<addLRef<int32>, int32&> == true);
    static_assert(isSameCV<addLRef<const int32>, const int32&> == true);
}

TEST_NS(Traits, Traits, AddRRef)
{
    static_assert(isSameCV<addRRef<int32>, int32> == false);
    static_assert(isSameCV<addRRef<int32&&>, int32&&> == true);
    static_assert(isSameCV<addRRef<int32&>, int32&> == true);
    static_assert(isSameCV<addRRef<int32>, int32&&> == true);
    static_assert(isSameCV<addRRef<const int32>, const int32&&> == true);
}

TEST_NS(Traits, Traits, RemovePointer)
{
    static_assert(isSameCV<removePointer<int32>, int32> == true);
    static_assert(isSameCV<removePointer<int32*>, int32> == true);
    static_assert(isSameCV<removePointer<int32**>, int32*> == true);
    static_assert(isSameCV<removePointer<int32* volatile>, int32> == true);
    static_assert(isSameCV<removePointer<int32* const>, int32> == true);
    static_assert(isSameCV<removePointer<int32* const volatile>, int32> == true);
}

TEST_NS(Traits, Traits, AddPointer)
{
    static_assert(isSameCV<addPointer<int32>, int32*> == true);
    static_assert(isSameCV<addPointer<int32*>, int32**> == true);
    static_assert(isSameCV<addPointer<int32**>, int32***> == true);
    static_assert(isSameCV<addPointer<int32* volatile>, int32* volatile*> == true);
}

TEST_NS(Traits, Traits, IsConst)
{
    static_assert(isConst<int32> == false);
    static_assert(isConst<volatile int32> == false);
    static_assert(isConst<const int32> == true);
    static_assert(isConst<const volatile int32> == true);
}

TEST_NS(Traits, Traits, IsVolatile)
{
    static_assert(isVolatile<int32> == false);
    static_assert(isVolatile<volatile int32> == true);
    static_assert(isVolatile<const int32> == false);
    static_assert(isVolatile<const volatile int32> == true);
}

TEST_NS(Traits, Traits, IsCV)
{
    static_assert(isCV<int32> == false);
    static_assert(isCV<volatile int32> == false);
    static_assert(isCV<const int32> == false);
    static_assert(isCV<const volatile int32> == true);
}

TEST_NS(Traits, Traits, IsCOrV)
{
    static_assert(isCOrV<int32> == false);
    static_assert(isCOrV<volatile int32> == true);
    static_assert(isCOrV<const int32> == true);
    static_assert(isCOrV<const volatile int32> == true);
}

TEST_NS(Traits, Traits, IsSame)
{
    static_assert(isSame<int32, uint32> == false);
    static_assert(isSame<int32, int32> == true);
    static_assert(isSame<uint32, int32> == false);
    static_assert(isSame<float32, int32> == false);
    static_assert(isSame<float32, float64> == false);
    static_assert(isSame<Test1, Test1> == true);
    static_assert(isSame<Test1, Test2> == false);
    static_assert(isSame<void, int32> == false);

    static_assert(isSameCV<int32, const int32> == false);
    static_assert(isSameCV<const int32, int32> == false);
    static_assert(isSameCV<int32, const volatile int32> == false);
    static_assert(isSameCV<const volatile int32, int32> == false);
    static_assert(isSameCV<int32, volatile int32> == false);
    static_assert(isSameCV<volatile int32, int32> == false);
    static_assert(isSameCV<volatile int32, volatile int32> == true);
    static_assert(isSameCV<const int32, const int32> == true);
    static_assert(isSameCV<const volatile int32, const volatile int32> == true);

    static_assert(isSame<int32, uint32> == false);
    static_assert(isSame<int32, int32> == true);
    static_assert(isSame<uint32, int32> == false);
    static_assert(isSame<float32, int32> == false);
    static_assert(isSame<float32, float64> == false);
    static_assert(isSame<Test1, Test1> == true);
    static_assert(isSame<Test1, Test2> == false);
    static_assert(isSame<void, int32> == false);

    static_assert(isSame<int32, const int32> == true);
    static_assert(isSame<const int32, int32> == true);
    static_assert(isSame<int32, const volatile int32> == true);
    static_assert(isSame<const volatile int32, int32> == true);
    static_assert(isSame<int32, volatile int32> == true);
    static_assert(isSame<volatile int32, int32> == true);
}

TEST_NS(Traits, Traits, IsSameAny)
{
    static_assert(isSameAny<int32, int32> == true);
    static_assert(isSameAny<int32, float32> == false);
    static_assert(isSameAny<int32, float32, int32> == true);
    static_assert(isSameAny<int32, float32, float64, uint32, int32> == true);
    static_assert(isSameAny<int32, float32, float64, uint32, int64> == false);

    static_assert(isSameAny<int32, float32, float64, const int32> == true);
    static_assert(isSameAny<const int32, float32, float64, int32> == true);
    static_assert(isSameAny<int32, float32, float64, const volatile int32> == true);
    static_assert(isSameAny<const volatile int32, float32, float64, int32> == true);
    static_assert(isSameAny<int32, float32, float64, volatile int32> == true);
    static_assert(isSameAny<volatile int32, float32, float64, int32> == true);

    static_assert(isSameAnyCV<int32, const int32, float32, float64> == false);
    static_assert(isSameAnyCV<const int32, int32, float32, float64> == false);
    static_assert(isSameAnyCV<int32, const volatile int32, float32, float64> == false);
    static_assert(isSameAnyCV<const volatile int32, int32, float32, float64> == false);
    static_assert(isSameAnyCV<int32, volatile int32, float32, float64> == false);
    static_assert(isSameAnyCV<volatile int32, int32, float32, float64> == false);
    static_assert(isSameAnyCV<volatile int32, float32, float64, volatile int32> == true);
    static_assert(isSameAnyCV<const int32, float32, float64, const int32> == true);
    static_assert(isSameAnyCV<const volatile int32, float32, float64, const volatile int32> == true);
}

TEST_NS(Traits, Traits, IsVoid)
{
    static_assert(isVoid<void> == true);
    static_assert(isVoid<const void> == true);
    static_assert(isVoid<volatile void> == true);
    static_assert(isVoid<char> == false);
    static_assert(isVoid<int16> == false);
}

TEST_NS(Traits, Traits, IsNullPtr)
{
    static_assert(isNullPtr<decltype(nullptr)> == true);
    static_assert(isNullPtr<const decltype(nullptr)> == true);
    static_assert(isNullPtr<volatile decltype(nullptr)> == true);
    static_assert(isNullPtr<char> == false);
    static_assert(isNullPtr<int16> == false);
}

TEST_NS(Traits, Traits, IsArray)
{
    static_assert(isArray<decltype(nullptr)> == false);
    static_assert(isArray<const decltype(nullptr)> == false);
    static_assert(isArray<uint32*> == false);
    static_assert(isArray<uint32[]> == true);
    static_assert(isArray<uint32[5]> == true);
}

TEST_NS(Traits, Traits, IsPointer)
{
    static_assert(isPointer<decltype(nullptr)> == false);
    static_assert(isPointer<uint32> == false);
    static_assert(isPointer<uint32*> == true);
    static_assert(isPointer<uint32**> == true);
    static_assert(isPointer<uint32[5]> == false);
}

TEST_NS(Traits, Traits, IsLRef)
{
    static_assert(isLRef<int32> == false);
    static_assert(isLRef<int32&&> == false);
    static_assert(isLRef<int32&> == true);
    static_assert(isLRef<int32*> == false);
}

TEST_NS(Traits, Traits, IsRRef)
{
    static_assert(isRRef<int32> == false);
    static_assert(isRRef<int32&&> == true);
    static_assert(isRRef<int32&> == false);
    static_assert(isRRef<int32*> == false);
}

TEST_NS(Traits, Traits, IsRef)
{
    static_assert(isRef<int32> == false);
    static_assert(isRef<int32&&> == true);
    static_assert(isRef<int32&> == true);
    static_assert(isRef<int32*> == false);
}

void test2(uint32) noexcept
{}

TEST_NS(Traits, Traits, IsFunction)
{
    class Test
    {
    public:
        static float32 test(const float32& val) noexcept
        {
            return val;
        }
    };
    static_assert(isFunction<decltype(Test::test)> == true);
    static_assert(isFunction<decltype(test2)> == true);
    static_assert(isFunction<int32> == false);
    static_assert(isFunction<int32*> == false);
    static_assert(isFunction<int32&&> == false);
}

TEST_NS(Traits, Traits, IsInteger)
{
    static_assert(isInteger<int8> == true);
    static_assert(isInteger<const int8> == true);
    static_assert(isInteger<volatile int8> == true);
    static_assert(isInteger<uint8> == true);
    static_assert(isInteger<char> == false);
    static_assert(isInteger<int16> == true);
    static_assert(isInteger<uint16> == true);
    static_assert(isInteger<int32> == true);
    static_assert(isInteger<uint32> == true);
    static_assert(isInteger<int64> == true);
    static_assert(isInteger<uint64> == true);
    static_assert(isInteger<Int128> == true);
    static_assert(isInteger<UInt128> == true);
    static_assert(isInteger<float32> == false);
    static_assert(isInteger<float64> == false);
    static_assert(isInteger<Test1> == false);
}

TEST_NS(Traits, Traits, IsFloat)
{
    static_assert(isFloat<float32> == true);
    static_assert(isFloat<const float32> == true);
    static_assert(isFloat<volatile float32> == true);
    static_assert(isFloat<float64> == true);
    static_assert(isFloat<int32> == false);
    static_assert(isFloat<uint32> == false);
    static_assert(isFloat<Test1> == false);
}

TEST_NS(Traits, Traits, IsArithmetic)
{
    static_assert(isArithmetic<int8> == true);
    static_assert(isArithmetic<const int8> == true);
    static_assert(isArithmetic<volatile int8> == true);
    static_assert(isArithmetic<uint8> == true);
    static_assert(isArithmetic<char> == false);
    static_assert(isArithmetic<int16> == true);
    static_assert(isArithmetic<uint16> == true);
    static_assert(isArithmetic<int32> == true);
    static_assert(isArithmetic<uint32> == true);
    static_assert(isArithmetic<int64> == true);
    static_assert(isArithmetic<uint64> == true);
    static_assert(isArithmetic<Int128> == true);
    static_assert(isArithmetic<UInt128> == true);
    static_assert(isArithmetic<Test1> == false);
    static_assert(isArithmetic<float32> == true);
    static_assert(isArithmetic<float64> == true);
}

TEST_NS(Traits, Traits, IsSigned)
{
    static_assert(isSigned<int8> == true);
    static_assert(isSigned<const int8> == true);
    static_assert(isSigned<volatile int8> == true);
    static_assert(isSigned<uint8> == false);
    static_assert(isSigned<int16> == true);
    static_assert(isSigned<uint16> == false);
    static_assert(isSigned<int32> == true);
    static_assert(isSigned<uint32> == false);
    static_assert(isSigned<int64> == true);
    static_assert(isSigned<uint64> == false);
    static_assert(isSigned<Int128> == true);
    static_assert(isSigned<UInt128> == false);
    static_assert(isSigned<Test1> == false);
    static_assert(isSigned<float32> == true);
    static_assert(isSigned<float64> == true);
}

TEST_NS(Traits, Traits, IsUnSigned)
{
    static_assert(isUnsigned<int8> == false);
    static_assert(isUnsigned<const int8> == false);
    static_assert(isUnsigned<volatile int8> == false);
    static_assert(isUnsigned<uint8> == true);
    static_assert(isUnsigned<int16> == false);
    static_assert(isUnsigned<uint16> == true);
    static_assert(isUnsigned<int32> == false);
    static_assert(isUnsigned<uint32> == true);
    static_assert(isUnsigned<int64> == false);
    static_assert(isUnsigned<uint64> == true);
    static_assert(isUnsigned<Int128> == false);
    static_assert(isUnsigned<UInt128> == true);
    static_assert(isUnsigned<Test1> == false);
    static_assert(isUnsigned<float32> == false);
    static_assert(isUnsigned<float64> == false);
}

TEST_NS(Traits, Traits, IsNative)
{
    static_assert(isNative<int8> == true);
    static_assert(isNative<const int8> == true);
    static_assert(isNative<volatile int8> == true);
    static_assert(isNative<uint8> == true);
    static_assert(isNative<int16> == true);
    static_assert(isNative<uint16> == true);
    static_assert(isNative<int32> == true);
    static_assert(isNative<uint32> == true);
    static_assert(isNative<int64> == true);
    static_assert(isNative<uint64> == true);
    static_assert(isNative<Int128> == false);
    static_assert(isNative<UInt128> == false);
    static_assert(isNative<Test1> == false);
    static_assert(isNative<float32> == true);
    static_assert(isNative<float64> == true);
}

TEST_NS(Traits, Traits, IsSwappable)
{
    static_assert(isSwappable<float> == true);
    static_assert(isSwappable<int16> == false);
    class SwapTest
    {
    public:
        void swap(SwapTest&) noexcept
        {}
    };
    static_assert(isSwappableMember<int8> == false);
    static_assert(isSwappableMember<SwapTest> == true);
}

TEST_NS(Traits, Traits, IsComparable)
{
    static_assert(isComparable<int8, int8> == true);
    static_assert(isComparable<int16, int8> == true);
    static_assert(isComparable<Int128, Int128> == true);
    class CompareTest
    {};
    static_assert(isComparable<float32, CompareTest> == false);
}

TEST_NS(Traits, Traits, Conditional)
{
    static_assert(isSame<conditional<true, int8, float32>, int8> == true);
    static_assert(isSame<conditional<false, int8, float32>, float32> == true);
}

TEST_NS(Traits, Traits, InvokableResult)
{
    class Test
    {
    public:
        static float32 test(const float32& val) noexcept
        {
            return val;
        }
    };
    using type = invokeResult<decltype(Test::test), float32>;
    static_assert(isSame<type, float32> == true);
    static_assert(isSame<invokeResult<decltype(Test::test), uint32>, uint32> == false);
}

TEST_NS(Traits, Traits, Invokable)
{
    class Test
    {
    public:
        static float test(const float& val) noexcept
        {
            return val;
        }
    };
    static_assert(isInvokable<decltype(Test::test), float32> == true);
    static_assert(isInvokable<decltype(test2), uint32> == true);
    static_assert(isInvokable<decltype(test2), uint32, bool> == false);
    static_assert(isInvokable<decltype(test2), uint32, bool, float32> == false);
    static_assert(isInvokable<decltype(test2), Test> == false);
}

TEST_NS(Traits, Traits, Promote)
{
    static_assert(isSame<promote<int8>, int16> == true);
    static_assert(isSame<promote<uint8>, uint16> == true);
    static_assert(isSame<promote<int16>, int32> == true);
    static_assert(isSame<promote<uint16>, uint32> == true);
    static_assert(isSame<promote<int32>, int64> == true);
    static_assert(isSame<promote<uint32>, uint64> == true);
    static_assert(isSame<promote<int64>, Int128> == true);
    static_assert(isSame<promote<uint64>, UInt128> == true);
    static_assert(isSame<promote<UInt128>, UInt128> == true);
    static_assert(isSame<promote<Int128>, Int128> == true);
    static_assert(isSame<promote<Int128>, UInt128> == false);

    static_assert(isSameCV<promote<const int8>, int16> == false);
    static_assert(isSameCV<promote<int8>, const int16> == false);
    static_assert(isSameCV<promote<const int8>, const int16> == true);
    static_assert(isSameCV<promote<const uint8>, uint16> == false);
    static_assert(isSameCV<promote<uint8>, const uint16> == false);
    static_assert(isSameCV<promote<const uint8>, const uint16> == true);
    static_assert(isSameCV<promote<const int16>, int32> == false);
    static_assert(isSameCV<promote<int16>, const int32> == false);
    static_assert(isSameCV<promote<const int16>, const int32> == true);
    static_assert(isSameCV<promote<const uint16>, uint32> == false);
    static_assert(isSameCV<promote<uint16>, const uint32> == false);
    static_assert(isSameCV<promote<const uint16>, const uint32> == true);
    static_assert(isSameCV<promote<const int64>, Int128> == false);
    static_assert(isSameCV<promote<int64>, const Int128> == false);
    static_assert(isSameCV<promote<const int64>, const Int128> == true);
    static_assert(isSameCV<promote<const uint64>, UInt128> == false);
    static_assert(isSameCV<promote<uint64>, const UInt128> == false);
    static_assert(isSameCV<promote<const uint64>, const UInt128> == true);
    static_assert(isSameCV<promote<const UInt128>, UInt128> == false);
    static_assert(isSameCV<promote<UInt128>, const UInt128> == false);
    static_assert(isSameCV<promote<const UInt128>, const UInt128> == true);
    static_assert(isSameCV<promote<const Int128>, Int128> == false);
    static_assert(isSameCV<promote<Int128>, const Int128> == false);
    static_assert(isSameCV<promote<const Int128>, const Int128> == true);
    static_assert(isSameCV<promote<const Int128>, const UInt128> == false);
}

TEST_NS(Traits, Traits, Demote)
{
    static_assert(isSame<demote<int8>, int8> == true);
    static_assert(isSame<demote<uint8>, uint8> == true);
    static_assert(isSame<demote<int16>, int8> == true);
    static_assert(isSame<demote<uint16>, uint8> == true);
    static_assert(isSame<demote<int32>, int16> == true);
    static_assert(isSame<demote<uint32>, uint16> == true);
    static_assert(isSame<demote<int64>, int32> == true);
    static_assert(isSame<demote<uint64>, uint32> == true);
    static_assert(isSame<demote<UInt128>, uint64> == true);
    static_assert(isSame<demote<Int128>, int64> == true);
    static_assert(isSame<demote<Int128>, UInt128> == false);
    static_assert(isSame<demote<uint32>, uint32> == false);
}
#endif

#ifndef XSTESTMAIN
TEST_NS(Traits, Traits, TESTISA(hasSIMD))
{
    static_assert(hasSIMD<float32> == (XS_ARCH_SSE == 1));
    static_assert(hasSIMD<uint32> == (XS_ARCH_SSE2 == 1));
    static_assert(hasSIMD<float64> == false);
    static_assert(hasSIMD<Int128> == false);

    static_assert(hasFMA<float32> == (XS_ARCH_AVX2 == 1));
    static_assert(hasFMA<uint32> == false);
    static_assert(hasFMA<float64> == false);
    static_assert(hasFMA<Int128> == false);

    static_assert(hasFMS<float32> == (XS_ARCH_AVX2 == 1));
    static_assert(hasFMS<uint32> == false);
    static_assert(hasFMS<float64> == false);
    static_assert(hasFMS<Int128> == false);

    static_assert(hasFMAFree<float32> == (XS_ARCH_AVX512F == 1));
    static_assert(hasFMAFree<uint32> == false);
    static_assert(hasFMAFree<float64> == false);
    static_assert(hasFMAFree<Int128> == false);
}
#endif
