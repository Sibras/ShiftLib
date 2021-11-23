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

#ifdef XSTESTMAIN
#    include "XSRational.hpp"

#    include "XSGTest.hpp"
#    include "XSInt128.hpp"
#    include "XSUInt128.hpp"

using namespace Shift;

namespace Shift {
template<typename T>
void PrintTo(Rational<T> param, std::ostream* os)
{
    if constexpr (isSameAny<T, UInt128, Int128>) {
        if constexpr (isSigned<T>) {
            *os << "(" << static_cast<int64>(param.numerator) << "/" << static_cast<int64>(param.denominator) << ")";
        } else {
            *os << "(" << static_cast<uint64>(param.numerator) << "/" << static_cast<uint64>(param.denominator) << ")";
        }
    } else {
        *os << "(" << param.numerator << "/" << param.denominator << ")";
    }
}
} // namespace Shift

template<typename T>
::testing::AssertionResult assertRational(Rational<T> param, T v0, T v1)
{
    if (testValue(param.numerator, v0) && testValue(param.denominator, v1)) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}

template<typename T>
class Rat : public ::testing::Test
{
public:
    using Type = T;
    using TypeInt = typename T::Type; // requested type
};

using RationalTestTypes = ::testing::Types<Rational<uint8>, Rational<int8>, Rational<uint16>, Rational<int16>,
    Rational<uint32>, Rational<int32>, Rational<uint64>, Rational<int64>, Rational<UInt128>, Rational<Int128>>;
TYPED_TEST_SUITE(Rat, RationalTestTypes);

TYPED_TEST_NS2(Rational, Rat, Rational)
{
    using TestType = typename TestFixture::Type;
    using TestTypeInt = typename TestFixture::TypeInt;

    //  Constructor Test
    TestType test1 = TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(64));
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test1, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(64));

    TestType test2 = TestType(4.0f / 128.0f);
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test2, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(32));

    TestType test3 = TestType(4.0 / 64.0);
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test3, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(16));

    TestType test4 = TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(4));
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test4, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(4));

    //  Get Test
    ASSERT_FLOAT_EQ(test2.getFloat(), 0.03125f);

    ASSERT_DOUBLE_EQ(test1.getDouble(), 0.015625);

    //  Load/Store Test
    RationalData<TestTypeInt> data(test1);
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), data.load(), static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(64));

    data.store(test1);
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), data.load(), static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(64));

    //  Math Test
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test1 + test1, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(32));

    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test2 - test1, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(64));

    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test1 - test2, static_cast<TestTypeInt>(-1),
        static_cast<TestTypeInt>(64));

    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test3 * test4, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(64));

    TestType test15 = test3 * test4;
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test15 / test4, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(16));

    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test1 += test1, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(32));

    test1 = TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(64));
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test2 -= test1, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(64));

    test2 = TestType(4.0f / 128.0f);
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test1 -= test2, static_cast<TestTypeInt>(-1),
        static_cast<TestTypeInt>(64));

    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test3 *= test4, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(64));

    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test3 /= test4, static_cast<TestTypeInt>(1),
        static_cast<TestTypeInt>(16));

    //  Comparison Test
    test1 = TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(64));
    ASSERT_TRUE(test1 == TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(64)));

    ASSERT_FALSE(test1 == TestType(static_cast<TestTypeInt>(-1), static_cast<TestTypeInt>(64)));

    ASSERT_TRUE(test1 <= TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(16)));

    ASSERT_TRUE(test1 <= TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(63)));

    if constexpr (isSigned<TestTypeInt>) {
        ASSERT_FALSE(test1 <= TestType(static_cast<TestTypeInt>(-1), static_cast<TestTypeInt>(63)));
    }

    ASSERT_TRUE(test1 <= TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(64)));

    if constexpr (isSigned<TestTypeInt>) {
        ASSERT_FALSE(test1 <= TestType(static_cast<TestTypeInt>(-1), static_cast<TestTypeInt>(64)));
    }

    ASSERT_TRUE(test1 < TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(16)));

    ASSERT_TRUE(test1 < TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(63)));

    if constexpr (isSigned<TestTypeInt>) {
        ASSERT_FALSE(test1 < TestType(static_cast<TestTypeInt>(-1), static_cast<TestTypeInt>(63)));
    }

    ASSERT_FALSE(test1 < TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(64)));

    if constexpr (isSigned<TestTypeInt>) {
        ASSERT_FALSE(test1 < TestType(static_cast<TestTypeInt>(-1), static_cast<TestTypeInt>(64)));
    }

    ASSERT_TRUE(test1 != TestType(static_cast<TestTypeInt>(-1), static_cast<TestTypeInt>(64)));

    ASSERT_FALSE(test1 != TestType(static_cast<TestTypeInt>(1), static_cast<TestTypeInt>(64)));

    //  Support Function Test
    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), -test3, static_cast<TestTypeInt>(-1),
        static_cast<TestTypeInt>(16));

    ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), test3.reciprocal(), static_cast<TestTypeInt>(16),
        static_cast<TestTypeInt>(1));

    if constexpr (isSigned<TestTypeInt>) {
        ASSERT_PRED3((assertRational<typename TestFixture::TypeInt>), (-test3).reciprocal(),
            static_cast<TestTypeInt>(-16), static_cast<TestTypeInt>(1));
    }
}
#endif
