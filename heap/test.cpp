#include <gtest/gtest.h>
#include "heap.hpp"

TEST(HeapTest, InsertGetMinTest) {
  Heap h;
  h.InsertKey(10);
  EXPECT_EQ(h.GetMin(), 10);

  h.InsertKey(5);
  EXPECT_EQ(h.GetMin(), 5);

  h.InsertKey(15);
  EXPECT_EQ(h.GetMin(), 5);
}

TEST(HeapTest, ExtractMinTest) {
  Heap h;
  h.InsertKey(20);
  h.InsertKey(5);
  h.InsertKey(15);

  EXPECT_EQ(h.GetMin(), 5);
  h.ExtractMin();
  EXPECT_EQ(h.GetMin(), 15);
  h.ExtractMin();
  EXPECT_EQ(h.GetMin(), 20);
}

TEST(HeapTest, DecreaseKeyTest) {
  Heap h;
  h.InsertKey(100);  // id 0
  h.InsertKey(200);  // id 1
  h.InsertKey(300);  // id 2

  h.DecreaseKey(2, 250);  // 300 - 250 = 50
  EXPECT_EQ(h.GetMin(), 50);

  h.DecreaseKey(0, 40);  // 100 - 40 = 60
  EXPECT_EQ(h.GetMin(), 50);

  h.DecreaseKey(2, 40);  // 50 - 40 = 10
  EXPECT_EQ(h.GetMin(), 10);
}

TEST(HeapTest, StressTest) {
  Heap h;
  const size_t N = 1'000'000;
  const long long cMod = 1'000'000;
  std::srand(std::time(nullptr));

  for (size_t i = 0; i < N; ++i) {
    h.InsertKey(std::rand() % cMod);
  }

  for (size_t i = 0; i < N; ++i) {
    h.DecreaseKey(i, std::rand() % cMod);
  }

  long long last = LLONG_MIN;
  for (size_t i = 0; i < N; ++i) {
    long long current = h.GetMin();
    EXPECT_GE(current, last);
    last = current;
    h.ExtractMin();
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
