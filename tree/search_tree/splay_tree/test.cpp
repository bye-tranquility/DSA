#include <gtest/gtest.h>
#include "splay_tree.hpp"

TEST(SplayTest, InsertExistsTest) {
  SplayTree t;
  t.Insert(10);
  EXPECT_EQ(t.Exists(10), true);

  t.Insert(5);
  EXPECT_EQ(t.Exists(7), false);
}

TEST(SplayTest, EraseTest) {
  SplayTree t;
  t.Insert(10);
  EXPECT_EQ(t.Exists(10), true);

  t.Erase(10);
  EXPECT_EQ(t.Exists(10), false);
}

TEST(SplayTest, NextPrevTest) {
  SplayTree t;
  t.Insert(10);
  t.Insert(5);
  t.Insert(7);

  EXPECT_EQ(t.Next(7).value(), 10);
  EXPECT_EQ(t.Prev(7).value(), 5);
}

TEST(SplayTest, KthTest) {
  SplayTree t;
  t.Insert(3);
  t.Insert(1);
  t.Insert(4);
  t.Insert(5);
  t.Insert(2);
  t.Insert(0);
  t.Insert(100);

  EXPECT_EQ(t.Kth(3).value(), 3);
  EXPECT_EQ(t.Kth(0).value(), 0);
  EXPECT_EQ(t.Kth(t.Size() - 1).value(), 100);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}