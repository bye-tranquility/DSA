#include <gtest/gtest.h>
#include "treap.hpp"

TEST(TreapTest, InsertGetSumTest) {
  Treap t;
  std::vector<Long> arr{-100, 200, 70, -300, 0};
  for (auto x : arr) {
    t.Insert(x);
  }

  EXPECT_EQ(t.Sum(-300, 200), -130);
}

TEST(TreapTest, EraseTest) {
  Treap t;
  t.Insert(10);
  EXPECT_EQ(t.Sum(10, 10), 10);
  t.Erase(10);
  EXPECT_EQ(t.Sum(10, 10), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}