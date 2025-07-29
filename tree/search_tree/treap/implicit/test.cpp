#include <gtest/gtest.h>
#include "implicit_treap.hpp"

TEST(ImplicitTreapTest, InsertGetSumTest) {
  ImplicitTreap t;
  std::vector<Long> arr{-100, 200, 70, -300, 0};
  for (int i = 0; i < arr.size(); ++i) {
    t.Insert(arr[i], i + 1);
  }
  EXPECT_EQ(t.Sum(2, 4), -230);
}

TEST(ImplicitTreapTest, EraseTest) {
  ImplicitTreap t;
  t.Insert(10, 2);
  EXPECT_EQ(t.Sum(2, 2), 0);
  t.Erase(2);
  EXPECT_EQ(t.Sum(2, 2), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}