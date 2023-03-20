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
#    include "Memory/XSSortPartition.hpp"

#    include "XSGTest.hpp"
#    include "XSLimits.hpp"

using namespace Shift;

template<typename T>
class PartitionTest : public ::testing::Test
{
public:
    using Type = T;
};

using PartitionTestTypes = ::testing::Types<bool, uint8, uint16, uint32>;

class PartitionTestNames
{
public:
    template<typename T>
    static std::string GetName(int)
    {
        if (std::is_same<T, bool>()) {
            return "bool";
        }
        if (std::is_same<T, uint8>()) {
            return "uint8";
        }
        if (std::is_same<T, uint16>()) {
            return "uint16";
        }
        if (std::is_same<T, uint32>()) {
            return "uint32";
        }
        return "def";
    }
};

constexpr uint0 partitionSize = 129;

TYPED_TEST_SUITE(PartitionTest, PartitionTestTypes, PartitionTestNames);

TYPED_TEST_NS2(Partition, PartitionTest, Partition)
{
    using TestType = typename TestFixture::Type;

    if constexpr (isSame<TestType, bool>) {
        Array<TestType> test1(partitionSize);
        uint0 check = partitionSize;
        for (uint0 i = 0; i < partitionSize; ++i) {
            test1.add((--check & 0x0001) != 0);
        }

        partition<PartitionAlgorithm::NonStable>(test1.begin(), test1.end());

        check = 0;
        for (auto& i : test1) {
            bool valid = ++check <= (test1.getLength() / 2);
            ASSERT_EQ(i, valid);
        }
    } else {
        Array<TestType> test1(partitionSize);
        srand(0x37649723);
        for (uint0 i = 0; i < partitionSize; ++i) {
            test1.add(static_cast<TestType>(rand() * Limits<TestType>::Max()));
        }

        partition<PartitionAlgorithm::NonStable>(
            test1.begin(), test1.end(), [](const TestType& value) { return value < Limits<TestType>::Max() / 2; });

        bool previous = true;
        for (auto& i : test1) {
            // check that the current one is greater or equal to previous
            bool current = i < Limits<TestType>::Max() / 2;
            previous = (!current && previous) ? false : previous;
            ASSERT_EQ(current, previous);
        }
    }
}

TYPED_TEST_NS2(Partition, PartitionTest, PartitionStable)
{
    using TestType = typename TestFixture::Type;

    if constexpr (isSame<TestType, bool>) {
        Array<TestType> test1(partitionSize);
        uint0 check = partitionSize;
        for (uint0 i = 0; i < partitionSize; ++i) {
            test1.add((--check & 0x0001) != 0);
        }

        partition<PartitionAlgorithm::Stable>(test1.begin(), test1.end());

        check = 0;
        for (auto& i : test1) {
            bool valid = ++check <= (test1.getLength() / 2);
            ASSERT_EQ(i, valid);
        }
    } else {
        Array<TestType> test1(partitionSize);
        srand(0x37649723);
        for (uint0 i = 0; i < partitionSize; ++i) {
            test1.add(static_cast<TestType>(rand() * Limits<TestType>::Max()));
        }

        partition<PartitionAlgorithm::Stable>(
            test1.begin(), test1.end(), [](const TestType& value) { return value < Limits<TestType>::Max() / 2; });

        bool previous = true;
        for (auto& i : test1) {
            // check that the current one is greater or equal to previous
            bool current = i < Limits<TestType>::Max() / 2;
            previous = (!current && previous) ? false : previous;
            ASSERT_EQ(current, previous);
        }
    }
}

TYPED_TEST_NS2(Partition, PartitionTest, PartitionStableScratch)
{
    using TestType = typename TestFixture::Type;

    if constexpr (isSame<TestType, bool>) {
        Array<TestType> test1(partitionSize);
        uint0 check = partitionSize;
        for (uint0 i = 0; i < partitionSize; ++i) {
            test1.add((--check & 0x0001) != 0);
        }

        // Create scratch memory and fuzz contents
        Array<TestType> scratch(partitionSize);
        for (uint0 i = 0; i < partitionSize; ++i) {
            scratch.add(false);
        }

        partition(test1.begin(), test1.end(), scratch.begin());

        check = 0;
        for (auto& i : test1) {
            bool valid = ++check <= (test1.getLength() / 2);
            ASSERT_EQ(i, valid);
        }
    } else {
        Array<TestType> test1(partitionSize);
        srand(0x37649723);
        for (uint0 i = 0; i < partitionSize; ++i) {
            test1.add(static_cast<TestType>(rand() * Limits<TestType>::Max()));
        }

        // Create scratch memory and fuzz contents
        Array<TestType> scratch(partitionSize);
        for (uint0 i = 0; i < partitionSize; ++i) {
            scratch.add(static_cast<TestType>(rand() * Limits<TestType>::Max()));
        }

        partition(test1.begin(), test1.end(), scratch.begin(),
            [](const TestType& value) { return value < Limits<TestType>::Max() / 2; });

        bool previous = true;
        for (auto& i : test1) {
            // check that the current one is greater or equal to previous
            bool current = i < Limits<TestType>::Max() / 2;
            previous = (!current && previous) ? false : previous;
            ASSERT_EQ(current, previous);
        }
    }
}

#endif
