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

#include <gtest/gtest.h>
#include <ostream>

#define XS_USE_TEST_NAMESPACE 0

// Need macro indirection to get TESTISA macro to work
#define GTEST_TYPED_TEST(test_suite_name, test_name) TYPED_TEST(test_suite_name, test_name)
#define TYPED_TEST2(test_suite_name, test_name) GTEST_TYPED_TEST(test_suite_name, test_name)
#define GTEST_TYPED_TEST_P(test_suite_name, test_name) TYPED_TEST_P(test_suite_name, test_name)
#define TYPED_TEST_P2(test_suite_name, test_name) GTEST_TYPED_TEST_P(test_suite_name, test_name)
#define GTEST_INSTANTIATE_TYPED_TEST_SUITE_P(Prefix, SuiteName, Types, ...) \
    INSTANTIATE_TYPED_TEST_SUITE_P(Prefix, SuiteName, Types, __VA_ARGS__)
#define INSTANTIATE_TYPED_TEST_SUITE_P2(Prefix, SuiteName, Types, ...) \
    GTEST_INSTANTIATE_TYPED_TEST_SUITE_P(Prefix, SuiteName, Types, __VA_ARGS__)

#if XS_USE_TEST_NAMESPACE
// A copy of GTEST_TEST_CLASS_NAME_, but with handling for namespace name.
#    define NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_case_name, test_name) \
        namespace_name##_##test_case_name##_##test_name##_Test

#    define TYPED_TEST_NS(NameSpace, CaseName, TestName)                                                              \
        static_assert(sizeof(GTEST_STRINGIFY_(TestName)) > 1, "test-name must not be empty");                         \
        template<typename gtest_TypeParam_>                                                                           \
        class NS_GTEST_TEST_CLASS_NAME_(NameSpace, CaseName, TestName)                                                \
            : public CaseName<gtest_TypeParam_>                                                                       \
        {                                                                                                             \
        private:                                                                                                      \
            typedef CaseName<gtest_TypeParam_> TestFixture;                                                           \
            typedef gtest_TypeParam_ TypeParam;                                                                       \
            void TestBody();                                                                                          \
        };                                                                                                            \
        static bool gtest_##NameSpace##_##CaseName##_##TestName##_registered_ GTEST_ATTRIBUTE_UNUSED_ =               \
            ::testing::internal::TypeParameterizedTest<CaseName,                                                      \
                ::testing::internal::TemplateSel<NS_GTEST_TEST_CLASS_NAME_(NameSpace, CaseName, TestName)>,           \
                GTEST_TYPE_PARAMS_(CaseName)>::Register("", ::testing::internal::CodeLocation(__FILE__, __LINE__),    \
                GTEST_STRINGIFY_(NameSpace) "." GTEST_STRINGIFY_(CaseName), GTEST_STRINGIFY_(TestName), 0,            \
                ::testing::internal::GenerateNames<GTEST_NAME_GENERATOR_(CaseName), GTEST_TYPE_PARAMS_(CaseName)>()); \
        template<typename gtest_TypeParam_>                                                                           \
        void NS_GTEST_TEST_CLASS_NAME_(NameSpace, CaseName, TestName)<gtest_TypeParam_>::TestBody()

#    define GTEST_TYPED_TEST_NS(NameSpace, CaseName, TestName) TYPED_TEST_NS(NameSpace, CaseName, TestName)
#else
#    define TYPED_TEST_NS(NameSpace, CaseName, TestName) TYPED_TEST(CaseName, TestName)
#    define GTEST_TYPED_TEST_NS(NameSpace, CaseName, TestName) TYPED_TEST(CaseName, TestName)
#endif
#define TYPED_TEST_NS2(NameSpace, CaseName, TestName) GTEST_TYPED_TEST_NS(NameSpace, CaseName, TestName)

#if XS_USE_TEST_NAMESPACE
#    define GTEST_TEST_NS_(namespace_name, test_suite_name, test_name, parent_class, parent_id)                        \
        static_assert(sizeof(GTEST_STRINGIFY_(test_suite_name)) > 1, "test_suite_name must not be empty");             \
        static_assert(sizeof(GTEST_STRINGIFY_(test_name)) > 1, "test_name must not be empty");                         \
        class NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name)                                    \
            : public parent_class                                                                                      \
        {                                                                                                              \
        public:                                                                                                        \
            NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name)() = default;                         \
            ~NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name)() override = default;               \
            NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name)                                      \
            (const NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name) &) = delete;                  \
            NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name) & operator=(                         \
                const NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name) &) = delete; /* NOLINT */  \
            NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name)                                      \
            (NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name) &&) noexcept = delete;              \
            NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name) & operator=(                         \
                NS_GTEST_TEST_CLASS_NAME_(                                                                             \
                    namespace_name, test_suite_name, test_name) &&) noexcept = delete; /* NOLINT */                    \
                                                                                                                       \
        private:                                                                                                       \
            void TestBody() override;                                                                                  \
            static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;                                      \
        };                                                                                                             \
                                                                                                                       \
        ::testing::TestInfo* const NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name)::test_info_ = \
            ::testing::internal::MakeAndRegisterTestInfo(#namespace_name "." #test_suite_name, #test_name, nullptr,    \
                nullptr, ::testing::internal::CodeLocation(__FILE__, __LINE__), (parent_id),                           \
                ::testing::internal::SuiteApiResolver<parent_class>::GetSetUpCaseOrSuite(__FILE__, __LINE__),          \
                ::testing::internal::SuiteApiResolver<parent_class>::GetTearDownCaseOrSuite(__FILE__, __LINE__),       \
                new ::testing::internal::TestFactoryImpl<NS_GTEST_TEST_CLASS_NAME_(                                    \
                    namespace_name, test_suite_name, test_name)>);                                                     \
        void NS_GTEST_TEST_CLASS_NAME_(namespace_name, test_suite_name, test_name)::TestBody()

#    define TEST_NS(namespace_name, test_suite_name, test_name) \
        GTEST_TEST_NS_(                                         \
            namespace_name, test_suite_name, test_name, ::testing::Test, ::testing::internal::GetTestTypeId())
#    define GTEST_TEST_NS(NameSpace, CaseName, TestName) TEST_NS(NameSpace, CaseName, TestName)
#else
#    define TEST_NS(namespace_name, test_suite_name, test_name) TEST(test_suite_name, test_name)
#    define GTEST_TEST_NS(namespace_name, test_suite_name, test_name) TEST(test_suite_name, test_name)
#endif
#define TEST_NS2(namespace_name, test_suite_name, test_name) GTEST_TEST_NS(namespace_name, test_suite_name, test_name)

template<typename T>
bool testValue(const T val, const T other) noexcept
{
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
#define MAXERROR 0.005
        if (std::isnan(val) || std::isnan(other)) {
            return false;
        }
        if (fabs(1.0 - (val / other)) >= MAXERROR) {
            return false;
        }
        return true;
    } else {
        return val == other;
    }
}

#ifndef GTEST_PRED7_
namespace testing {
template<typename Pred, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
AssertionResult AssertPred7Helper(const char* pred_text, const char* e1, const char* e2, const char* e3, const char* e4,
    const char* e5, const char* e6, const char* e7, Pred pred, const T1& v1, const T2& v2, const T3& v3, const T4& v4,
    const T5& v5, const T6& v6, const T7& v7)
{
    if (pred(v1, v2, v3, v4, v5, v6, v7))
        return AssertionSuccess();

    return AssertionFailure() << pred_text << "(" << e1 << ", " << e2 << ", " << e3 << ", " << e4 << ", " << e5 << ", "
                              << e6 << ", " << e7 << ") evaluates to false, where"
                              << "\n"
                              << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
                              << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
                              << e3 << " evaluates to " << ::testing::PrintToString(v3) << "\n"
                              << e4 << " evaluates to " << ::testing::PrintToString(v4) << "\n"
                              << e5 << " evaluates to " << ::testing::PrintToString(v5) << "\n"
                              << e6 << " evaluates to " << ::testing::PrintToString(v6) << "\n"
                              << e7 << " evaluates to " << ::testing::PrintToString(v7);
}
} // namespace testing
#    define GTEST_PRED7_(pred, v1, v2, v3, v4, v5, v6, v7, on_failure)                                                \
        GTEST_ASSERT_(                                                                                                \
            ::testing::AssertPred7Helper(#pred, #v1, #v2, #v3, #v4, #v5, #v6, #v7, pred, v1, v2, v3, v4, v5, v6, v7), \
            on_failure)
#endif
#ifndef ASSERT_PRED7
#    define ASSERT_PRED7(pred, v1, v2, v3, v4, v5, v6, v7) \
        GTEST_PRED7_(pred, v1, v2, v3, v4, v5, v6, v7, GTEST_FATAL_FAILURE_)
#endif

#ifndef GTEST_PRED9_
namespace testing {
template<typename Pred, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7,
    typename T8, typename T9>
AssertionResult AssertPred9Helper(const char* pred_text, const char* e1, const char* e2, const char* e3, const char* e4,
    const char* e5, const char* e6, const char* e7, const char* e8, const char* e9, Pred pred, const T1& v1,
    const T2& v2, const T3& v3, const T4& v4, const T5& v5, const T6& v6, const T7& v7, const T8& v8, const T9& v9)
{
    if (pred(v1, v2, v3, v4, v5, v6, v7, v8, v9))
        return AssertionSuccess();

    return AssertionFailure() << pred_text << "(" << e1 << ", " << e2 << ", " << e3 << ", " << e4 << ", " << e5 << ", "
                              << e6 << ", " << e7 << ", " << e8 << ", " << e9 << ") evaluates to false, where"
                              << "\n"
                              << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
                              << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
                              << e3 << " evaluates to " << ::testing::PrintToString(v3) << "\n"
                              << e4 << " evaluates to " << ::testing::PrintToString(v4) << "\n"
                              << e5 << " evaluates to " << ::testing::PrintToString(v5) << "\n"
                              << e6 << " evaluates to " << ::testing::PrintToString(v6) << "\n"
                              << e7 << " evaluates to " << ::testing::PrintToString(v7) << "\n"
                              << e8 << " evaluates to " << ::testing::PrintToString(v8) << "\n"
                              << e9 << " evaluates to " << ::testing::PrintToString(v9);
}
} // namespace testing
#    define GTEST_PRED9_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, on_failure)                                       \
        GTEST_ASSERT_(::testing::AssertPred9Helper(#pred, #v1, #v2, #v3, #v4, #v5, #v6, #v7, #v8, #v9, pred, v1, v2, \
                          v3, v4, v5, v6, v7, v8, v9),                                                               \
            on_failure)
#endif
#ifndef ASSERT_PRED9
#    define ASSERT_PRED9(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
        GTEST_PRED9_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, GTEST_FATAL_FAILURE_)
#endif

#ifndef GTEST_PRED10_
namespace testing {
template<typename Pred, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7,
    typename T8, typename T9, typename T10>
AssertionResult AssertPred10Helper(const char* pred_text, const char* e1, const char* e2, const char* e3,
    const char* e4, const char* e5, const char* e6, const char* e7, const char* e8, const char* e9, const char* e10,
    Pred pred, const T1& v1, const T2& v2, const T3& v3, const T4& v4, const T5& v5, const T6& v6, const T7& v7,
    const T8& v8, const T9& v9, const T10& v10)
{
    if (pred(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10))
        return AssertionSuccess();

    return AssertionFailure() << pred_text << "(" << e1 << ", " << e2 << ", " << e3 << ", " << e4 << ", " << e5 << ", "
                              << e6 << ", " << e7 << ", " << e8 << ", " << e9 << ", " << e10
                              << ") evaluates to false, where"
                              << "\n"
                              << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
                              << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
                              << e3 << " evaluates to " << ::testing::PrintToString(v3) << "\n"
                              << e4 << " evaluates to " << ::testing::PrintToString(v4) << "\n"
                              << e5 << " evaluates to " << ::testing::PrintToString(v5) << "\n"
                              << e6 << " evaluates to " << ::testing::PrintToString(v6) << "\n"
                              << e7 << " evaluates to " << ::testing::PrintToString(v7) << "\n"
                              << e8 << " evaluates to " << ::testing::PrintToString(v8) << "\n"
                              << e9 << " evaluates to " << ::testing::PrintToString(v9) << "\n"
                              << e10 << " evaluates to " << ::testing::PrintToString(v10);
}
} // namespace testing
#    define GTEST_PRED10_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, on_failure)                                \
        GTEST_ASSERT_(::testing::AssertPred10Helper(#pred, #v1, #v2, #v3, #v4, #v5, #v6, #v7, #v8, #v9, #v10, pred, \
                          v1, v2, v3, v4, v5, v6, v7, v8, v9, v10),                                                 \
            on_failure)
#endif
#ifndef ASSERT_PRED10
#    define ASSERT_PRED10(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
        GTEST_PRED10_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, GTEST_FATAL_FAILURE_)
#endif

#ifndef GTEST_PRED13_
namespace testing {
template<typename Pred, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7,
    typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
AssertionResult AssertPred13Helper(const char* pred_text, const char* e1, const char* e2, const char* e3,
    const char* e4, const char* e5, const char* e6, const char* e7, const char* e8, const char* e9, const char* e10,
    const char* e11, const char* e12, const char* e13, Pred pred, const T1& v1, const T2& v2, const T3& v3,
    const T4& v4, const T5& v5, const T6& v6, const T7& v7, const T8& v8, const T9& v9, const T10& v10, const T11& v11,
    const T12& v12, const T13& v13)
{
    if (pred(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13))
        return AssertionSuccess();

    return AssertionFailure() << pred_text << "(" << e1 << ", " << e2 << ", " << e3 << ", " << e4 << ", " << e5 << ", "
                              << e6 << ", " << e7 << ", " << e8 << ", " << e9 << ", " << e10 << ", " << e11 << ", "
                              << e12 << ", " << e13 << ") evaluates to false, where"
                              << "\n"
                              << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
                              << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
                              << e3 << " evaluates to " << ::testing::PrintToString(v3) << "\n"
                              << e4 << " evaluates to " << ::testing::PrintToString(v4) << "\n"
                              << e5 << " evaluates to " << ::testing::PrintToString(v5) << "\n"
                              << e6 << " evaluates to " << ::testing::PrintToString(v6) << "\n"
                              << e7 << " evaluates to " << ::testing::PrintToString(v7) << "\n"
                              << e8 << " evaluates to " << ::testing::PrintToString(v8) << "\n"
                              << e9 << " evaluates to " << ::testing::PrintToString(v9) << "\n"
                              << e10 << " evaluates to " << ::testing::PrintToString(v10) << "\n"
                              << e11 << " evaluates to " << ::testing::PrintToString(v11) << "\n"
                              << e12 << " evaluates to " << ::testing::PrintToString(v12) << "\n"
                              << e13 << " evaluates to " << ::testing::PrintToString(v13);
}
} // namespace testing
#    define GTEST_PRED13_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, on_failure)                 \
        GTEST_ASSERT_(::testing::AssertPred13Helper(#pred, #v1, #v2, #v3, #v4, #v5, #v6, #v7, #v8, #v9, #v10, #v11, \
                          #v12, #v13, pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13),                \
            on_failure)
#endif
#ifndef ASSERT_PRED13
#    define ASSERT_PRED13(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) \
        GTEST_PRED13_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, GTEST_FATAL_FAILURE_)
#endif

#ifndef GTEST_PRED17_
namespace testing {
template<typename Pred, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7,
    typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17>
AssertionResult AssertPred17Helper(const char* pred_text, const char* e1, const char* e2, const char* e3,
    const char* e4, const char* e5, const char* e6, const char* e7, const char* e8, const char* e9, const char* e10,
    const char* e11, const char* e12, const char* e13, const char* e14, const char* e15, const char* e16,
    const char* e17, Pred pred, const T1& v1, const T2& v2, const T3& v3, const T4& v4, const T5& v5, const T6& v6,
    const T7& v7, const T8& v8, const T9& v9, const T10& v10, const T11& v11, const T12& v12, const T13& v13,
    const T14& v14, const T15& v15, const T16& v16, const T17& v17)
{
    if (pred(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17))
        return AssertionSuccess();

    return AssertionFailure() << pred_text << "(" << e1 << ", " << e2 << ", " << e3 << ", " << e4 << ", " << e5 << ", "
                              << e6 << ", " << e7 << ", " << e8 << ", " << e9 << ", " << e10 << ", " << e11 << ", "
                              << e12 << ", " << e13 << ", " << e14 << ", " << e15 << ", " << e16 << ", " << e17
                              << ") evaluates to false, where"
                              << "\n"
                              << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
                              << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
                              << e3 << " evaluates to " << ::testing::PrintToString(v3) << "\n"
                              << e4 << " evaluates to " << ::testing::PrintToString(v4) << "\n"
                              << e5 << " evaluates to " << ::testing::PrintToString(v5) << "\n"
                              << e6 << " evaluates to " << ::testing::PrintToString(v6) << "\n"
                              << e7 << " evaluates to " << ::testing::PrintToString(v7) << "\n"
                              << e8 << " evaluates to " << ::testing::PrintToString(v8) << "\n"
                              << e9 << " evaluates to " << ::testing::PrintToString(v9) << "\n"
                              << e10 << " evaluates to " << ::testing::PrintToString(v10) << "\n"
                              << e11 << " evaluates to " << ::testing::PrintToString(v11) << "\n"
                              << e12 << " evaluates to " << ::testing::PrintToString(v12) << "\n"
                              << e13 << " evaluates to " << ::testing::PrintToString(v13) << "\n"
                              << e14 << " evaluates to " << ::testing::PrintToString(v14) << "\n"
                              << e15 << " evaluates to " << ::testing::PrintToString(v15) << "\n"
                              << e16 << " evaluates to " << ::testing::PrintToString(v16) << "\n"
                              << e17 << " evaluates to " << ::testing::PrintToString(v17);
}
} // namespace testing
#    define GTEST_PRED17_(                                                                                             \
        pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, on_failure)                  \
        GTEST_ASSERT_(::testing::AssertPred17Helper(#pred, #v1, #v2, #v3, #v4, #v5, #v6, #v7, #v8, #v9, #v10, #v11,    \
                          #v12, #v13, #v14, #v15, #v16, #v17, pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                          v13, v14, v15, v16, v17),                                                                    \
            on_failure)
#endif
#ifndef ASSERT_PRED17
#    define ASSERT_PRED17(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17) \
        GTEST_PRED17_(                                                                                      \
            pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, GTEST_FATAL_FAILURE_)
#endif

#ifndef GTEST_PRED25_
namespace testing {
template<typename Pred, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7,
    typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23,
    typename T24, typename T25>
AssertionResult AssertPred25Helper(const char* pred_text, const char* e1, const char* e2, const char* e3,
    const char* e4, const char* e5, const char* e6, const char* e7, const char* e8, const char* e9, const char* e10,
    const char* e11, const char* e12, const char* e13, const char* e14, const char* e15, const char* e16,
    const char* e17, const char* e18, const char* e19, const char* e20, const char* e21, const char* e22,
    const char* e23, const char* e24, const char* e25, Pred pred, const T1& v1, const T2& v2, const T3& v3,
    const T4& v4, const T5& v5, const T6& v6, const T7& v7, const T8& v8, const T9& v9, const T10& v10, const T11& v11,
    const T12& v12, const T13& v13, const T14& v14, const T15& v15, const T16& v16, const T17& v17, const T18& v18,
    const T19& v19, const T20& v20, const T21& v21, const T22& v22, const T23& v23, const T24& v24, const T25& v25)
{
    if (pred(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,
            v24, v25))
        return AssertionSuccess();

    return AssertionFailure() << pred_text << "(" << e1 << ", " << e2 << ", " << e3 << ", " << e4 << ", " << e5 << ", "
                              << e6 << ", " << e7 << ", " << e8 << ", " << e9 << ", " << e10 << ", " << e11 << ", "
                              << e12 << ", " << e13 << ", " << e14 << ", " << e15 << ", " << e16 << ", " << e17 << ", "
                              << e18 << ", " << e19 << ", " << e20 << ", " << e21 << ", " << e22 << ", " << e23 << ", "
                              << e24 << ", " << e25 << ") evaluates to false, where"
                              << "\n"
                              << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
                              << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
                              << e3 << " evaluates to " << ::testing::PrintToString(v3) << "\n"
                              << e4 << " evaluates to " << ::testing::PrintToString(v4) << "\n"
                              << e5 << " evaluates to " << ::testing::PrintToString(v5) << "\n"
                              << e6 << " evaluates to " << ::testing::PrintToString(v6) << "\n"
                              << e7 << " evaluates to " << ::testing::PrintToString(v7) << "\n"
                              << e8 << " evaluates to " << ::testing::PrintToString(v8) << "\n"
                              << e9 << " evaluates to " << ::testing::PrintToString(v9) << "\n"
                              << e10 << " evaluates to " << ::testing::PrintToString(v10) << "\n"
                              << e11 << " evaluates to " << ::testing::PrintToString(v11) << "\n"
                              << e12 << " evaluates to " << ::testing::PrintToString(v12) << "\n"
                              << e13 << " evaluates to " << ::testing::PrintToString(v13) << "\n"
                              << e14 << " evaluates to " << ::testing::PrintToString(v14) << "\n"
                              << e15 << " evaluates to " << ::testing::PrintToString(v15) << "\n"
                              << e16 << " evaluates to " << ::testing::PrintToString(v16) << "\n"
                              << e17 << " evaluates to " << ::testing::PrintToString(v17) << "\n"
                              << e18 << " evaluates to " << ::testing::PrintToString(v18) << "\n"
                              << e19 << " evaluates to " << ::testing::PrintToString(v19) << "\n"
                              << e20 << " evaluates to " << ::testing::PrintToString(v20) << "\n"
                              << e21 << " evaluates to " << ::testing::PrintToString(v21) << "\n"
                              << e22 << " evaluates to " << ::testing::PrintToString(v22) << "\n"
                              << e23 << " evaluates to " << ::testing::PrintToString(v23) << "\n"
                              << e24 << " evaluates to " << ::testing::PrintToString(v24) << "\n"
                              << e25 << " evaluates to " << ::testing::PrintToString(v25);
}
} // namespace testing
#    define GTEST_PRED25_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, \
        v20, v21, v22, v23, v24, v25, on_failure)                                                                     \
        GTEST_ASSERT_(                                                                                                \
            ::testing::AssertPred25Helper(#pred, #v1, #v2, #v3, #v4, #v5, #v6, #v7, #v8, #v9, #v10, #v11, #v12, #v13, \
                #v14, #v15, #v16, #v17, #v18, #v19, #v20, #v21, #v22, #v23, #v24, #v25, pred, v1, v2, v3, v4, v5, v6, \
                v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25),          \
            on_failure)
#endif
#ifndef ASSERT_PRED25
#    define ASSERT_PRED25(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,  \
        v20, v21, v22, v23, v24, v25)                                                                                  \
        GTEST_PRED25_(pred, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
            v21, v22, v23, v24, v25, GTEST_FATAL_FAILURE_)
#endif
