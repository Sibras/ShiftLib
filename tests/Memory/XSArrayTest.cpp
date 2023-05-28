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

#    include "Memory/XSArray.hpp"

#    include "Memory/XSAllocatorStack.hpp"
#    include "XSGTest.hpp"

using namespace Shift;

template<typename T>
class ArrayTest : public ::testing::Test
{
public:
    using Type = T;
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

    bool operator<(const size_t in) const noexcept
    {
        return data[0] < in;
    }

    D1024 operator++() noexcept
    {
        ++data[0];
        return *this;
    }

    D1024 operator--() noexcept
    {
        --data[0];
        return *this;
    }
};

using MemoryTestTypes = ::testing::Types<uint8, uint16, uint32, uint64, D1024>;

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
        if (std::is_same<T, D1024>()) {
            return "D1024";
        }
        return "def";
    }
};

TYPED_TEST_SUITE(ArrayTest, MemoryTestTypes, MemoryTestNames);

TYPED_TEST_NS2(Array, ArrayTest, Constructor)
{
    using TestType = typename TestFixture::Type;

    auto test1 = Array<TestType>(8);
    ASSERT_TRUE(test1.isValid());
    ASSERT_EQ(reinterpret_cast<uint0>(test1.getData()) % alignof(TestType), 0);
    ASSERT_TRUE(test1.getReservedSize() >= 8 * sizeof(TestType));
    ASSERT_TRUE(test1.getReservedLength() >= 8);

    auto test2 = Array<TestType, AllocRegionStack<TestType, 8>>();
    ASSERT_TRUE(test2.isValid());
    ASSERT_EQ(reinterpret_cast<uint0>(test1.getData()) % alignof(TestType), 0);
    ASSERT_TRUE(test2.getReservedSize() == 8 * sizeof(TestType));
    ASSERT_TRUE(test2.getReservedLength() == 8);

    Array<TestType> test3;
    ASSERT_FALSE(test3.isValid());

    Array<TestType> test4(test1);
    ASSERT_EQ(test4.getSize(), test1.getSize());
}

TYPED_TEST_NS2(Array, ArrayTest, AddElement)
{
    using TestType = typename TestFixture::Type;
    auto test1 = Array<TestType>(8);
    ASSERT_TRUE(test1.isValid());

    // Add some data
    for (TestType i = TestType(0); i < 8; ++i) {
        test1.add(i);
    }

    // Check sizing
    ASSERT_EQ(test1.getSize(), 8 * sizeof(TestType));
    ASSERT_EQ(test1.getLength(), 8);

    // Check if contents are valid
    TestType check(0);
    for (auto& i : test1) {
        ASSERT_EQ(i, check);
        ++check;
    }

    Array<TestType> test2(test1);
    test2.setReservedLength(test2.getReservedLength() * 2 + 1);
    test2.add(test1);
    test2.add(check);
    ASSERT_EQ(test2.getLength(), (2 * test1.getLength()) + 1);
    ASSERT_EQ(test2.atBack(), check);

    auto test3 = test1 + test1;
    ASSERT_EQ(test3.getLength(), (2 * test1.getLength()));
    ASSERT_EQ(test3.atBack(), --check);
}

#endif
