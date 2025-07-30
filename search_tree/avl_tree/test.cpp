#include <gtest/gtest.h>
#include "avl_tree.hpp"

TEST(AVLTest, InsertLowerBoundTest) {
  AVLTree t;
  t.Insert(10);
  EXPECT_EQ(t.LowerBound(10), 10);

  t.Insert(5);
  EXPECT_EQ(t.LowerBound(7), 10);

  t.Insert(15);
  EXPECT_EQ(t.LowerBound(12), 15);
  EXPECT_EQ(t.LowerBound(-100'000), 5);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
