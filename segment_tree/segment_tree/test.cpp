#include <gtest/gtest.h>
#include "segment_tree.hpp"

TEST(SegmentTreeTest, GetMaxSimpleTest) {
  std::vector<long long> arr{-100, 200, 70, -300, 0};
  SegmentTree tree{arr};
  EXPECT_EQ(tree.GetMax(0, 5), 200);
  EXPECT_EQ(tree.GetMax(4, 5), 0);
  EXPECT_EQ(tree.GetMax(-100, 100), 200);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
