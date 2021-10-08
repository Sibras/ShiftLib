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

#if !defined(XSTESTMAIN)
#    include "XSCompilerOptions.h"
#    if !(XS_ARCH_AVX && !XS_ARCH_AVX2)
#        include "XSGTest.hpp"

#        define XS_OVERRIDE_SHIFT_NS TESTISA(MemoryTest)

namespace XS_OVERRIDE_SHIFT_NS {
#        include "Memory/XSMemory.hpp"
}

using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

constexpr size_t testMaxElements = 2048;

template<typename T>
class TESTISA(Memory)
    : public ::testing::Test
{
public:
    using Type = T;

protected:
    static T* input;
    static T* output;

    static void SetUpTestSuite()
    {
        input = new T[testMaxElements + 1];
        output = new T[testMaxElements + 2];
        // Initialise data
        for (size_t i = 0; i < testMaxElements; ++i) {
            input[i] = T(i);
        }
        constexpr size_t max = 0xFFFFFFFFFFFFFFFF;
        input[testMaxElements] = static_cast<T>(max);
    }

    static void TearDownTestSuite()
    {
        delete input;
        input = nullptr;
        delete output;
        output = nullptr;
    }
};

template<typename T>
T* TESTISA(Memory)<T>::input = nullptr;
template<typename T>
T* TESTISA(Memory)<T>::output = nullptr;

class alignas(16) D128
{
public:
    size_t data[16 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    D128() noexcept = default;

    explicit D128(const size_t in) noexcept
    {
        data[0] = in;
    }

    bool operator==(const D128& in) const noexcept
    {
        return data[0] == in.data[0];
    }
};

class alignas(32) D256
{
public:
    size_t data[32 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    D256() noexcept = default;

    explicit D256(const size_t in) noexcept
    {
        data[0] = in;
    }

    bool operator==(const D256& in) const noexcept
    {
        return data[0] == in.data[0];
    }
};

class alignas(64) D512
{
public:
    size_t data[64 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    D512() noexcept = default;

    explicit D512(const size_t in) noexcept
    {
        data[0] = in;
    }

    bool operator==(const D512& in) const noexcept
    {
        return data[0] == in.data[0];
    }
};

class alignas(128) D1024
{
public:
    size_t data[128 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    D1024() noexcept = default;

    explicit D1024(const size_t in) noexcept
    {
        data[0] = in;
    }

    bool operator==(const D1024& in) const noexcept
    {
        return data[0] == in.data[0];
    }
};

class alignas(256) D2048
{
public:
    size_t data[256 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    D2048() noexcept = default;

    explicit D2048(const size_t in) noexcept
    {
        data[0] = in;
    }

    bool operator==(const D2048& in) const noexcept
    {
        return data[0] == in.data[0];
    }
};

using MemoryTestTypes = ::testing::Types<uint8, uint16, uint32, uint64, D128, D256, D512, D1024, D2048>;

class MemoryTestNames
{
public:
    template<typename T>
    static std::string GetName(int)
    {
        if (std::is_same<T, uint8>()) {
            return "uint8";
        }
        if (std::is_same<T, uint16>()) {
            return "uint16";
        }
        if (std::is_same<T, uint32>()) {
            return "uint32";
        }
        if (std::is_same<T, uint64>()) {
            return "uint64";
        }
        if (std::is_same<T, D128>()) {
            return "D128";
        }
        if (std::is_same<T, D256>()) {
            return "D256";
        }
        if (std::is_same<T, D512>()) {
            return "D512";
        }
        if (std::is_same<T, D1024>()) {
            return "D1024";
        }
        if (std::is_same<T, D2048>()) {
            return "D2048";
        }
        return "def";
    }
};

TYPED_TEST_SUITE(TESTISA(Memory), MemoryTestTypes, MemoryTestNames);

TYPED_TEST_NS2(Memory, TESTISA(Memory), MemMove)
{
    using TestType = typename TestFixture::Type;

    // Check 0 copy
    const TestType comp2 = TestType(1);
    TestFixture::output[0] = comp2;
    memMove<TestType>(TestFixture::output, TestFixture::input, 0);
    ASSERT_EQ(TestFixture::output[0], comp2);

    for (size_t testElements = 1; testElements < testMaxElements; testElements <<= 2) {
        const size_t testSize = testElements * sizeof(TestType);

        // Zero output data
        for (size_t i = 0; i < testElements + 2; ++i) {
            TestFixture::output[i] = TestType(0);
        }

        // Perform Copy
        memMove<TestType>(TestFixture::output, TestFixture::input, testSize);

        // Check data
        for (uint32 i = 0; i < testElements; ++i) {
            const TestType comp = TestType(i);
            ASSERT_EQ(TestFixture::output[i], comp);
        }
        // Check for overwrite
        ASSERT_EQ(TestFixture::output[testElements], TestType(0));
        ASSERT_EQ(TestFixture::output[testElements + 1], TestType(0));

        if (testElements > 1) {
            // Zero output data
            for (size_t i = 0; i < testElements + 2; ++i) {
                TestFixture::output[i] = TestType(0);
            }

            // Check handling of offset data
            memMove(TestFixture::output, &TestFixture::input[1], testSize - sizeof(TestType));

            // Check data
            for (uint32 i = 0; i < testElements - 1; ++i) {
                const TestType comp = TestType(i + 1);
                ASSERT_EQ(TestFixture::output[i], comp);
            }
            // Check for overwrite
            ASSERT_EQ(TestFixture::output[testElements], TestType(0));
            ASSERT_EQ(TestFixture::output[testElements + 1], TestType(0));

            // Zero output data
            for (size_t i = 0; i < testElements + 2; ++i) {
                TestFixture::output[i] = TestType(0);
            }

            // Check handling of offset data at end
            memMove(&TestFixture::output[1], TestFixture::input, testSize - sizeof(TestType));

            // Check data
            for (uint32 i = 0; i < testElements - 1; ++i) {
                const TestType comp = TestType(i);
                ASSERT_EQ(TestFixture::output[i + 1], comp);
            }
            // Check for overwrite
            ASSERT_EQ(TestFixture::output[testElements], TestType(0));
            ASSERT_EQ(TestFixture::output[testElements + 1], TestType(0));

            // Zero output data
            for (size_t i = 0; i < testElements + 2; ++i) {
                TestFixture::output[i] = TestType(0);
            }

            // Check handling of offset data at both ends
            memMove(&TestFixture::output[1], &TestFixture::input[1], testSize - sizeof(TestType));

            // Check data
            for (uint32 i = 1; i < testElements - 1; ++i) {
                const TestType comp = TestType(i);
                ASSERT_EQ(TestFixture::output[i], comp);
            }
            // Check for overwrite
            ASSERT_EQ(TestFixture::output[testElements], TestType(0));
            ASSERT_EQ(TestFixture::output[testElements + 1], TestType(0));
        }
    }
}

TYPED_TEST_NS2(Memory, TESTISA(Memory), MemMoveBack)
{
    using TestType = typename TestFixture::Type;

    // Check 0 copy
    const TestType comp2 = TestType(1);
    TestFixture::output[0] = comp2;
    memMoveBackwards<TestType>(TestFixture::output, TestFixture::input, 0);
    ASSERT_EQ(TestFixture::output[0], comp2);

    for (size_t testElements = 1; testElements < testMaxElements; testElements <<= 2) {
        const size_t testSize = testElements * sizeof(TestType);

        // Zero output data
        for (size_t i = 0; i < testElements + 2; ++i) {
            TestFixture::output[i] = TestType(0);
        }

        // Perform Copy
        memMoveBackwards<TestType>(TestFixture::output, TestFixture::input, testSize);

        // Check data
        for (uint32 i = 0; i < testElements; ++i) {
            const TestType comp = TestType(i);
            ASSERT_EQ(TestFixture::output[i], comp);
        }
        // Check for overwrite
        ASSERT_EQ(TestFixture::output[testElements], TestType(0));
        ASSERT_EQ(TestFixture::output[testElements + 1], TestType(0));

        if (testElements > 1) {
            // Zero output data
            for (size_t i = 0; i < testElements + 2; ++i) {
                TestFixture::output[i] = TestType(0);
            }

            // Check handling of offset data
            memMoveBackwards(TestFixture::output, &TestFixture::input[1], testSize - sizeof(TestType));

            // Check data
            for (uint32 i = 0; i < testElements - 1; ++i) {
                const TestType comp = TestType(i + 1);
                ASSERT_EQ(TestFixture::output[i], comp);
            }
            // Check for overwrite
            ASSERT_EQ(TestFixture::output[testElements], TestType(0));
            ASSERT_EQ(TestFixture::output[testElements + 1], TestType(0));

            // Zero output data
            TestFixture::input[testElements] = TestType(0);
            TestFixture::input[testElements + 1] = TestType(0);

            // Check handling of offset data at end
            // This also checks for correct handling of overlapping region in copy
            memMoveBackwards(&TestFixture::input[1], TestFixture::input, testSize - sizeof(TestType));

            // Check data
            for (uint32 i = 0; i < testElements - 1; ++i) {
                const TestType comp = TestType(i);
                ASSERT_EQ(TestFixture::input[i + 1], comp);
            }
            // Check for overwrite
            ASSERT_EQ(TestFixture::input[testElements], TestType(0));

            // Re-Initialise data
            for (size_t i = 0; i < testMaxElements; ++i) {
                TestFixture::input[i] = TestType(i);
            }
            constexpr size_t max = 0xFFFFFFFFFFFFFFFF;
            TestFixture::input[testMaxElements] = static_cast<TestType>(max);

            // Zero output data
            for (size_t i = 0; i < testElements + 2; ++i) {
                TestFixture::output[i] = TestType(0);
            }

            // Check handling of offset data at both ends
            memMoveBackwards(&TestFixture::output[1], &TestFixture::input[1], testSize - sizeof(TestType));

            // Check data
            for (uint32 i = 1; i < testElements - 1; ++i) {
                const TestType comp = TestType(i);
                ASSERT_EQ(TestFixture::output[i], comp);
            }
            // Check for overwrite
            ASSERT_EQ(TestFixture::output[testElements], TestType(0));
            ASSERT_EQ(TestFixture::output[testElements + 1], TestType(0));
        }
    }
}

TYPED_TEST_NS2(Memory, TESTISA(Memory), MemSwap)
{
    using TestType = typename TestFixture::Type;

    TestType in = TestType(0xFF);
    TestType out = TestType(1);

    // Perform reverse
    memSwap(&in, &out);

    // Check data
    ASSERT_EQ(in, TestType(1));
    ASSERT_EQ(out, TestType(0xFF));
}

TYPED_TEST_NS2(Memory, TESTISA(Memory), MemReverse)
{
    using TestType = typename TestFixture::Type;

    TestType* inputR = new TestType[testMaxElements];
    for (size_t testElements = 1; testElements < testMaxElements; testElements <<= 2) {
        // Initialise data
        for (size_t i = 0; i < testElements; ++i) {
            inputR[i] = TestType(i);
        }

        // Perform reverse
        memReverse(inputR, &inputR[testElements]);

        // Check data
        size_t val = testElements;
        for (size_t i = 0; i < testElements; ++i) {
            --val;
            const TestType comp = TestType(val);
            ASSERT_EQ(inputR[i], comp);
        }

        if (testElements > 1) {
            // Check handling of offset data
            memReverse(&inputR[1], &inputR[testElements]);

            // Check data
            for (size_t i = 0; i < testElements - 1; ++i) {
                const TestType comp = TestType(i);
                ASSERT_EQ(inputR[i + 1], comp);
            }

            // Re-Initialise data
            for (size_t i = 0; i < testElements; ++i) {
                inputR[i] = TestType(i);
            }

            // Check handling of offset data at end
            memReverse(inputR, &inputR[testElements - 1]);

            // Check data
            val = testElements - 1;
            for (size_t i = 0; i < testElements - 1; ++i) {
                --val;
                const TestType comp = TestType(val);
                ASSERT_EQ(inputR[i], comp);
            }

            // Re-Initialise data
            for (size_t i = 0; i < testElements; ++i) {
                inputR[i] = TestType(i);
            }

            // Check handling of offset data at both ends
            memReverse(&inputR[1], &inputR[testElements - 1]);

            // Check data
            val = testElements - 1;
            for (size_t i = 1; i < testElements - 2; ++i) {
                --val;
                const TestType comp = TestType(val);
                ASSERT_EQ(inputR[i], comp);
            }
        }
    }
}

#    endif
#endif
