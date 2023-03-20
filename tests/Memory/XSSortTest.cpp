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
#    include "Memory/XSSort.hpp"

#    include "XSGTest.hpp"

using namespace Shift;

template<typename T>
class SortTest : public ::testing::Test
{
public:
    using Type = T;
};

class alignas(128) D1024
{
public:
    size_t data[128 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    D1024() noexcept = default;

    D1024(const size_t in) noexcept
    {
        data[0] = in;
    }

    bool operator==(const D1024& in) const noexcept
    {
        return data[0] == in.data[0];
    }

    bool operator<(const int in) const
    {
        return data[0] < in;
    }

    bool operator<(const D1024& in) const
    {
        return data[0] < in.data[0];
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

    D1024 operator++(int) noexcept
    {
        const D1024 ret = *this;
        ++data[0];
        return ret;
    }

    D1024 operator--(int) noexcept
    {
        const D1024 ret = *this;
        --data[0];
        return ret;
    }
};

using SortTestTypes = ::testing::Types<uint8, uint16, uint32, D1024>;

class SortTestNames
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
        if (std::is_same<T, D1024>()) {
            return "D1024";
        }
        return "def";
    }
};

constexpr uint0 sortSize = 129;

TYPED_TEST_SUITE(SortTest, SortTestTypes, SortTestNames);

TYPED_TEST_NS2(Sort, SortTest, SortInsertion)
{
    using TestType = typename TestFixture::Type;

    Array<TestType> test1(sortSize);
    TestType check = TestType(sortSize - 1);
    for (uint0 i = 0; i < sortSize; ++i) {
        test1.add(check--);
    }

    sort<SortAlgorithm::Insertion>(test1.begin(), test1.end());

    check = 0;
    for (auto& i : test1) {
        ASSERT_EQ(i, check);
        ++check;
    }

    using PackedTestType = Pair<Pair<D1024, uint32>, TestType>;
    Array<PackedTestType> test2(sortSize);
    check = TestType(sortSize - 1);
    for (uint0 i = 0; i < sortSize; ++i) {
        test2.add();
        test2.atBack().second = check--;
    }

    sort<SortAlgorithm::Insertion>(test2.begin(), test2.end(),
        [](const PackedTestType& first, const PackedTestType& second) { return first.second < second.second; });

    check = 0;
    for (auto& i : test2) {
        ASSERT_EQ(i.second, check);
        ++check;
    }
}

TYPED_TEST_NS2(Sort, SortTest, SortQuick)
{
    using TestType = typename TestFixture::Type;

    Array<TestType> test1(sortSize);
    TestType check = TestType(sortSize - 1);
    for (uint0 i = 0; i < sortSize; ++i) {
        test1.add(check--);
    }

    sort<SortAlgorithm::Quick>(test1.begin(), test1.end());

    check = 0;
    for (auto& i : test1) {
        ASSERT_EQ(i, check);
        ++check;
    }

    using PackedTestType = Pair<Pair<D1024, uint32>, TestType>;
    Array<PackedTestType> test2(sortSize);
    check = TestType(sortSize - 1);
    for (uint0 i = 0; i < sortSize; ++i) {
        test2.add();
        test2.atBack().second = check--;
    }

    sort<SortAlgorithm::Quick>(test2.begin(), test2.end(),
        [](const PackedTestType& first, const PackedTestType& second) { return first.second < second.second; });

    check = 0;
    for (auto& i : test2) {
        ASSERT_EQ(i.second, check);
        ++check;
    }
}

#endif
