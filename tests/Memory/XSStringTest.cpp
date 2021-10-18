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
#    include "Memory/XSString.hpp"

#    include "XSGTest.hpp"

using namespace Shift;

template<typename T>
class StringTest : public ::testing::Test
{
public:
    using Type = T;
};

using StringTestTypes = ::testing::Types<char, char8, char16, char32>;

class StringTestNames
{
public:
    template<typename T>
    static std::string GetName(int)
    {
        if (std::is_same<T, char>()) {
            return "char";
        }
        if (std::is_same<T, char8>()) {
            return "char8";
        }
        if (std::is_same<T, char16>()) {
            return "char16";
        }
        if (std::is_same<T, char32>()) {
            return "char32";
        }
        return "def";
    }
};

TYPED_TEST_SUITE(StringTest, StringTestTypes, StringTestNames);

TYPED_TEST_NS2(String, StringTest, Constructor)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test0 = String<TestType>();
    ASSERT_FALSE(test0.isValid());

    String<TestType> test1 = String<TestType>(U"Hello", 5);
    ASSERT_EQ(test1, String<TestType>("Hello"));
    ASSERT_EQ(test1, String<TestType>(u8"Hello"));
    ASSERT_EQ(test1, String<TestType>(u"Hello"));
    ASSERT_EQ(test1, String<TestType>(U"Hello"));

    ASSERT_EQ(test1, String<TestType>("Hello"_s));
    ASSERT_EQ(test1, String<TestType>(u8"Hello"_s));
    ASSERT_EQ(test1, String<TestType>(u"Hello"_s));
    ASSERT_EQ(test1, String<TestType>(U"Hello"_s));

    String<TestType> test2 = String<TestType>(test1);
    ASSERT_EQ(test2, String<TestType>("Hello"));

    String<TestType> test3(test2, 0, 3);
    ASSERT_EQ(test3, String<TestType>("Hel"));

    typename String<TestType>::TypeConstIterator iterator1 = test2.cbegin();
    String<TestType> test4(test2, iterator1 + 3, iterator1 + 5);
    ASSERT_EQ(test4, String<TestType>("lo"));

    String<TestType> test5 = String<TestType>(static_cast<uint16_t>(12587));
    ASSERT_EQ(test5, String<TestType>("12587"));

    test5 = String<TestType>(static_cast<int16_t>(-12587));
    ASSERT_EQ(test5, String<TestType>("-12587"));

    test5 = String<TestType>(232145698U);
    ASSERT_EQ(test5, String<TestType>("232145698"));

    test5 = String<TestType>(-232145698);
    ASSERT_EQ(test5, String<TestType>("-232145698"));

    test5 = String<TestType>(985432514552658892ULL);
    ASSERT_EQ(test5, String<TestType>("985432514552658892"));

    test5 = String<TestType>(-985432514552658892LL);
    ASSERT_EQ(test5, String<TestType>("-985432514552658892"));

    test5 = String<TestType>(451258.03125f);
    ASSERT_EQ(test5, String<TestType>("451258.03125"));

    test5 = String<TestType>(-451258.3125f);
    ASSERT_EQ(test5, String<TestType>("-451258.3125"));

    test5 = String<TestType>(0.32312408f);
    ASSERT_EQ(test5, String<TestType>("3.2312408e-01"));

    test5 = String<TestType>(1.0218521356f);
    ASSERT_EQ(test5, String<TestType>("1.0218521356"));

    test5 = String<TestType>(-1.000218510627f);
    ASSERT_EQ(test5, String<TestType>("-1.000218510627"));

    test5 = String<TestType>(451258.0f);
    ASSERT_EQ(test5, String<TestType>("451258.0"));

    test5 = String<TestType>(-0.00000002f);
    ASSERT_EQ(test5, String<TestType>("-1.99999997e-08"));

    test5 = String<TestType>(451258304.0f);
    ASSERT_EQ(test5, String<TestType>("451258304.0"));

    test5 = String<TestType>(0.00218521244f);
    ASSERT_EQ(test5, String<TestType>("2.18521244e-03"));

    test5 = String<TestType>(751257971000000000000.0f);
    ASSERT_EQ(test5, String<TestType>("7.51257971e20"));

    test5 = String<TestType>(451258.03125);
    ASSERT_EQ(test5, String<TestType>("451258.03125"));

    test5 = String<TestType>(-451258.3125);
    ASSERT_EQ(test5, String<TestType>("-451258.3125"));

    test5 = String<TestType>(1.0218521356);
    ASSERT_EQ(test5, String<TestType>("1.02185213560000009985"));

    test5 = String<TestType>(-1.000218510627);
    ASSERT_EQ(test5, String<TestType>("-1.0002185106270000680691"));

    test5 = String<TestType>(451258.0);
    ASSERT_EQ(test5, String<TestType>("451258.0"));

    test5 = String<TestType>(-0.00000002);
    ASSERT_EQ(test5, String<TestType>("-2.000000000000000036e-008"));

    test5 = String<TestType>(451258304.0);
    ASSERT_EQ(test5, String<TestType>("451258304.0"));

    test5 = String<TestType>(0.00218521244);
    ASSERT_EQ(test5, String<TestType>("2.185212440000000148e-003"));

    test5 = String<TestType>(751257971000000000000.0);
    ASSERT_EQ(test5, String<TestType>("7.51257970999999987e020"));

    test5 = String<TestType>(875421.0003215469187125563618);
    ASSERT_EQ(test5, String<TestType>("875421.0003215469187125563618"));

    test5 = String<TestType>(-9784531246597.0);
    ASSERT_EQ(test5, String<TestType>("-9784531246597.0"));

    test5 = String<TestType>(25160000000000000000000000000000000000.0);
    ASSERT_EQ(test5, String<TestType>("2.515999999999999917e037"));
}

TYPED_TEST_NS2(String, StringTest, Add)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test1 = String<TestType>("Hello");

    test1.add(TestType(' '));
    ASSERT_EQ(test1, String<TestType>("Hello "));

    test1.add(String<TestType>("World", 5));
    ASSERT_EQ(test1, String<TestType>("Hello World"));
}

TYPED_TEST_NS2(String, StringTest, Insert)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test1 = String<TestType>("Hello World");
    String<TestType> test2 = String<TestType>("Hello");

    test1.insert(1, TestType('H'));
    ASSERT_EQ(test1, String<TestType>("HHello World"));

    test1.insert(2, test2);
    ASSERT_EQ(test1, String<TestType>("HHHelloello World"));

    typename String<TestType>::TypeIterator iterator2 = ++test1.begin();
    test1.insert(iterator2, TestType('P'));
    ASSERT_EQ(test1, String<TestType>("HPHHelloello World"));

    iterator2 = ++test1.begin();
    test1.insert(++iterator2, test2);
    ASSERT_EQ(test1, String<TestType>("HPHelloHHelloello World"));
}

TYPED_TEST_NS2(String, StringTest, Remove)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test1 = String<TestType>("HPHelloHHelloello World");

    test1.remove(1);
    ASSERT_EQ(test1, String<TestType>("HHelloHHelloello World"));

    test1.remove(1, 6);
    ASSERT_EQ(test1, String<TestType>("HHHelloello World"));

    auto iterator2 = ++test1.begin();
    test1.remove(++iterator2);
    ASSERT_EQ(test1, String<TestType>("HHelloello World"));

    --iterator2;
    test1.remove(iterator2, iterator2 + 5);
    ASSERT_EQ(test1, String<TestType>("Hello World"));
    test1.removeAll();
    ASSERT_EQ(test1, String<TestType>(""));
}

TYPED_TEST_NS2(String, StringTest, Replace)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test2 = String<TestType>("Hello");

    test2.replace(1, 5, String<TestType>("urts", 4));
    ASSERT_EQ(test2, String<TestType>("Hurts"));

    test2.replace(3, 5, String<TestType>("ricane", 6));
    ASSERT_EQ(test2, String<TestType>("Hurricane"));

    test2.replace(3, 9, String<TestType>("mit", 3));
    ASSERT_EQ(test2, String<TestType>("Hurmit"));

    typename String<TestType>::TypeIterator iterator3 = ++test2.begin();
    typename String<TestType>::TypeIterator iterator4(iterator3 + 2);
    test2.replace(iterator3, iterator4, String<TestType>("el", 2));
    ASSERT_EQ(test2, String<TestType>("Helmit"));

    ++iterator3;
    iterator4 = iterator3 + 3;
    test2.replace(iterator3, iterator4, String<TestType>("ar", 2));
    ASSERT_EQ(test2, String<TestType>("Heart"));

    iterator4 = test2.begin() + 2;
    typename String<TestType>::TypeIterator iterator(test2.begin());
    test2.replace(iterator, iterator4, String<TestType>("P", 1));
    ASSERT_EQ(test2, String<TestType>("Part"));

    test2.replace(1, 3, String<TestType>("lan", 3));
    ASSERT_EQ(test2, String<TestType>("Plant"));

    test2.replace(iterator, iterator4, String<TestType>("Tir", 3));
    ASSERT_EQ(test2, String<TestType>("Tirant"));

    iterator4 = test2.begin() + 5;
    test2.replace(iterator, iterator4, String<TestType>("Helmi", 5));
    ASSERT_EQ(test2, String<TestType>("Helmit"));
}

TYPED_TEST_NS2(String, StringTest, SubString)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test2 = String<TestType>("Helmit");
    String<TestType> test6 = String<TestType>("********************", 20);
    String<TestType> test7 = String<TestType>("********************", 20);
    typename String<TestType>::TypeIterator iterator4 = ++test7.begin();
    ASSERT_EQ(test6, String<TestType>("********************"));

    ASSERT_EQ(test7, String<TestType>("********************"));

    test6.set(1, test2, 0, 3);
    ASSERT_EQ(test6, String<TestType>("*Hel****************"));

    test7.set(iterator4, test2, test2.cbegin(), test2.cbegin() + 3);
    ASSERT_EQ(test7, String<TestType>("*Hel****************"));

    test6.set(3, test2, 1, 6);
    ASSERT_EQ(test6, String<TestType>("*Heelmit************"));

    auto iterator = iterator4 + 2;
    test7.set(iterator, test2, test2.cbegin() + 1, test2.cbegin() + 6);
    ASSERT_EQ(test7, String<TestType>("*Heelmit************"));

    test6.removeAll();
    ASSERT_EQ(test6, String<TestType>(""));

    test6.set(0, test7, 0, 15);
    ASSERT_EQ(test6, String<TestType>("*Heelmit*******"));

    test7.removeAll();
    ASSERT_EQ(test7, String<TestType>(""));

    iterator = test7.begin();
    test7.set(iterator, test6, test6.cbegin(), test6.cend());
    ASSERT_EQ(test7, String<TestType>("*Heelmit*******"));

    test6.set(test2, 1, 6);
    ASSERT_EQ(test6, String<TestType>("elmit"));

    test7.set(test2, test2.cbegin() + 1, test2.cbegin() + 6);
    ASSERT_EQ(test7, String<TestType>("elmit"));
}

TYPED_TEST_NS2(String, StringTest, SetNumber)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test8 = String<TestType>("********************", 20);
    String<TestType> test9 = String<TestType>("********************", 20);
    typename String<TestType>::TypeIterator iterator5 = ++test9.begin();
    ASSERT_EQ(test8, String<TestType>("********************"));

    ASSERT_EQ(test9, String<TestType>("********************"));

    test8.set(1, static_cast<uint16_t>(12587));
    ASSERT_EQ(test8, String<TestType>("*12587**************"));

    test9.set(iterator5, static_cast<uint16_t>(12587));
    ASSERT_EQ(test9, String<TestType>("*12587**************"));

    test8.set(1, static_cast<int16_t>(-12587));
    ASSERT_EQ(test8, String<TestType>("*-12587*************"));

    test9.set(iterator5, static_cast<int16_t>(-12587));
    ASSERT_EQ(test9, String<TestType>("*-12587*************"));

    test8.set(1, 232145698U);
    ASSERT_EQ(test8, String<TestType>("*232145698**********"));

    test9.set(iterator5, 232145698U);
    ASSERT_EQ(test9, String<TestType>("*232145698**********"));

    test8.set(1, -232145698);
    ASSERT_EQ(test8, String<TestType>("*-232145698*********"));

    test9.set(iterator5, -232145698);
    ASSERT_EQ(test9, String<TestType>("*-232145698*********"));

    test8.set(1, 985432514552658892ULL);
    ASSERT_EQ(test8, String<TestType>("*985432514552658892*"));

    test9.set(iterator5, 985432514552658892ULL);
    ASSERT_EQ(test9, String<TestType>("*985432514552658892*"));

    test8.set(1, -985432514552658892LL);
    ASSERT_EQ(test8, String<TestType>("*-985432514552658892"));

    test9.set(iterator5, -985432514552658892LL);
    ASSERT_EQ(test9, String<TestType>("*-985432514552658892"));

    // reset strings
    test8 = String<TestType>("********************", 20);
    test9 = String<TestType>("********************", 20);
    iterator5 = ++test9.begin();
    ASSERT_EQ(test8, String<TestType>("********************"));

    ASSERT_EQ(test9, String<TestType>("********************"));

    test8.set(1, 451258.03125f);
    ASSERT_EQ(test8, String<TestType>("*451258.03125*******"));

    test9.set(iterator5, -451258.03125f);
    ASSERT_EQ(test9, String<TestType>("*-451258.03125******"));

    test8.set(1, -0.00000002f);
    ASSERT_EQ(test8, String<TestType>("*-1.99999997e-08****"));

    test9.set(iterator5, 751257971000000000000.0f);
    ASSERT_EQ(test9, String<TestType>("*7.51257971e20******"));

    test8.set(1, 25160000000000000000000000000000000000.0);
    ASSERT_EQ(test8, String<TestType>("*2.515999999999999917e037"));

    test9.set(iterator5, -25160000000000000000000000000000000000.0);
    ASSERT_EQ(test9, String<TestType>("*-2.515999999999999917e037"));

    test8.set(1, 875421.0003215469187125563618);
    ASSERT_EQ(test8, String<TestType>("*875421.0003215469187125563618"));

    test9.set(iterator5, 875421.0003215469187125563618);
    ASSERT_EQ(test9, String<TestType>("*875421.0003215469187125563618"));

    test8.set(static_cast<uint16_t>(12587));
    ASSERT_EQ(test8, String<TestType>("12587"));

    test8.set(static_cast<int16_t>(-12587));
    ASSERT_EQ(test8, String<TestType>("-12587"));

    test8.set(232145698U);
    ASSERT_EQ(test8, String<TestType>("232145698"));

    test8.set(-232145698);
    ASSERT_EQ(test8, String<TestType>("-232145698"));

    test8.set(985432514552658892ULL);
    ASSERT_EQ(test8, String<TestType>("985432514552658892"));

    test8.set(-985432514552658892LL);
    ASSERT_EQ(test8, String<TestType>("-985432514552658892"));

    test8.set(451258.3125f);
    ASSERT_EQ(test8, String<TestType>("451258.3125"));

    test8.set(-451258.3125f);
    ASSERT_EQ(test8, String<TestType>("-451258.3125"));

    test8.set(875421.0003215469187125563618);
    ASSERT_EQ(test8, String<TestType>("875421.0003215469187125563618"));

    test8.set(-875421.0003215469187125563618);
    ASSERT_EQ(test8, String<TestType>("-875421.0003215469187125563618"));
}

TYPED_TEST_NS2(String, StringTest, SetString)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test8 = String<TestType>("-875421.0003215468975");
    String<TestType> test9;

#    define SET_STRING_TESTS(U)                                     \
        test8.set(U"********************", 20);                     \
        ASSERT_EQ(test8, String<TestType>("********************")); \
                                                                    \
        test9.set(U"********************", 20);                     \
        ASSERT_EQ(test9, String<TestType>("********************")); \
                                                                    \
        test8.set(1, U"ABCD", 4);                                   \
        ASSERT_EQ(test8, String<TestType>("*ABCD***************")); \
                                                                    \
        test8.set(2, U"12587", 5);                                  \
        ASSERT_EQ(test8, String<TestType>("*A12587*************")); \
                                                                    \
        test9.set(++test9.begin(), U"12587", 5);                    \
        ASSERT_EQ(test9, String<TestType>("*12587**************")); \
                                                                    \
        test8.removeAll();                                          \
        ASSERT_EQ(test8, String<TestType>(""));                     \
                                                                    \
        test8.set(0, U"12587", 5);                                  \
        ASSERT_EQ(test8, String<TestType>("12587"));                \
                                                                    \
        test9.removeAll();                                          \
        ASSERT_EQ(test9, String<TestType>(""));                     \
                                                                    \
        auto it = test9.begin();                                    \
        test9.set(it, U"12587", 5);                                 \
        ASSERT_EQ(test9, String<TestType>("12587"));                \
                                                                    \
        test8.set(U"ABCD", 4);                                      \
        ASSERT_EQ(test8, String<TestType>("ABCD"));

    if constexpr (isSame<TestType, char>) {
#    define NOTHING
        SET_STRING_TESTS(NOTHING)
    } else if constexpr (isSame<TestType, char8>) {
#    ifdef __cpp_char8_t
        SET_STRING_TESTS(u8)
#    endif
    } else if constexpr (isSame<TestType, char16>) {
        SET_STRING_TESTS(u)
    } else if constexpr (isSame<TestType, char32>) {
        SET_STRING_TESTS(U)
    }
}

TYPED_TEST_NS2(String, StringTest, CaseConversion)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test2 = String<TestType>("Helmit");

    test2.toUpper();
    ASSERT_EQ(test2, String<TestType>("HELMIT"));

    test2.toLower();
    ASSERT_EQ(test2, String<TestType>("helmit"));
}

TYPED_TEST_NS2(String, StringTest, NumberConversion)
{
    using TestType = typename TestFixture::Type;

    ASSERT_EQ(String<TestType>("12587", 5).template toNumber<uint16>(), 12587);

    ASSERT_EQ(String<TestType>("-12587", 6).template toNumber<int16>(), -12587);

    ASSERT_EQ(String<TestType>("232145698", 9).template toNumber<uint32>(), 232145698);

    ASSERT_EQ(String<TestType>("-232145698", 10).template toNumber<int32>(), -232145698);

    ASSERT_EQ(String<TestType>("985432514552658892", 18).template toNumber<uint64>(), 985432514552658892);

    ASSERT_EQ(String<TestType>("-985432514552658892", 19).template toNumber<int64>(), -985432514552658892);

    // ASSERT_FLOAT_EQ(String<TestType>( "451258.32658741", 15 ).toFloat32(), 451258.32658741);

    // ASSERT_FLOAT_EQ(String<TestType>( "-451258.32658741", 16 ).toFloat32(), -451258.32658741);

    // ASSERT_FLOAT_EQ(String<TestType>( "875421.0003215468975", 20 ).toFloat64(), 875421.0003215468975);

    // ASSERT_FLOAT_EQ(String<TestType>( "-875421.0003215468975", 21 ).toFloat64(),-875421.0003215468975);
}

TYPED_TEST_NS2(String, StringTest, Find)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test2 = String<TestType>("Helmit");

#    define FIND_TESTS(U)                      \
        ASSERT_EQ(test2.findFirst(U'm'), 'm'); \
                                               \
        ASSERT_EQ(test2.findLast(U'e'), 'e');

    if constexpr (isSame<TestType, char>) {
        FIND_TESTS(NOTHING)
    } else if constexpr (isSame<TestType, char8>) {
#    ifdef __cpp_char8_t
        FIND_TESTS(u8)
#    endif
    } else if constexpr (isSame<TestType, char16>) {
        FIND_TESTS(u)
    } else if constexpr (isSame<TestType, char32>) {
        FIND_TESTS(U)
    }

    ASSERT_EQ(test2.findFirst(String<TestType>("elm", 3)), 'e');

    ASSERT_EQ(test2.findLast(String<TestType>("elm", 3)), 'e');
}

TYPED_TEST_NS2(String, StringTest, Index)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test2 = String<TestType>("Helmit");

#    define INDEX_TESTS(U)                      \
        ASSERT_EQ(test2.indexOfFirst(U'm'), 3); \
                                                \
        ASSERT_EQ(test2.indexOfLast(U'e'), 1);

    if constexpr (isSame<TestType, char>) {
        INDEX_TESTS(NOTHING)
    } else if constexpr (isSame<TestType, char8>) {
#    ifdef __cpp_char8_t
        INDEX_TESTS(u8)
#    endif
    } else if constexpr (isSame<TestType, char16>) {
        INDEX_TESTS(u)
    } else if constexpr (isSame<TestType, char32>) {
        INDEX_TESTS(U)
    }

    ASSERT_EQ(test2.indexOfFirst(String<TestType>("elm", 3)), 1);

    ASSERT_EQ(test2.indexOfLast(String<TestType>("mit", 3)), 3);
}

TYPED_TEST_NS2(String, StringTest, Compare)
{
    using TestType = typename TestFixture::Type;

    String<TestType> test2 = String<TestType>("Helmit");

    ASSERT_EQ(test2.compare(String<TestType>("Hermit", 6)), -6);

    ASSERT_EQ(test2.compare(String<TestType>("Helmit", 6)), 0);

    ASSERT_EQ(test2.compare(String<TestType>("Helmet", 6)), 4);
}
#endif
