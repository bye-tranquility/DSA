#include <gtest/gtest.h>
#include "fenwick_tree.hpp"

TEST(FenwickTreeTest, GetSumSimpleTest) {
  std::vector<long long> arr{-100, 200, 70, -300, 0};
  FenwickTree tree{arr};
  EXPECT_EQ(tree.GetPrefixSum(5), -130);
  EXPECT_EQ(tree.GetSum(2, 4), -230);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}