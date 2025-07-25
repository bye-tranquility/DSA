#include "gtest/gtest.h"
#include "min_queue.hpp"

TEST(MinQueueTest, BasicTest) {
  MinQueue q;
  q.Enqueue(5);
  q.Enqueue(3);
  q.Enqueue(7);

  EXPECT_EQ(q.Size(), 3);
  EXPECT_EQ(q.Front(), 5);
  EXPECT_EQ(q.Min(), 3);
  EXPECT_EQ(q.Dequeue(), 5);
  EXPECT_EQ(q.Min(), 3);
  EXPECT_EQ(q.Dequeue(), 3);
  EXPECT_EQ(q.Min(), 7);
  EXPECT_EQ(q.Dequeue(), 7);
  EXPECT_EQ(q.Size(), 0);
}

TEST(MinQueueTest, DecreasingSequenceTest) {
  MinQueue q;
  q.Enqueue(5);
  q.Enqueue(4);
  q.Enqueue(3);
  q.Enqueue(2);
  q.Enqueue(1);

  EXPECT_EQ(q.Min(), 1);
  EXPECT_EQ(q.Front(), 5);
  q.Dequeue();
  EXPECT_EQ(q.Min(), 1);
  q.Dequeue();
  EXPECT_EQ(q.Min(), 1);
  q.Dequeue();
  EXPECT_EQ(q.Min(), 1);
  q.Dequeue();
  EXPECT_EQ(q.Min(), 1);
  q.Dequeue();
  EXPECT_EQ(q.Size(), 0);
}

TEST(MinQueueTest, IncreasingSequenceTest) {
  MinQueue q;
  q.Enqueue(1);
  q.Enqueue(2);
  q.Enqueue(3);
  q.Enqueue(4);
  q.Enqueue(5);

  EXPECT_EQ(q.Min(), 1);
  EXPECT_EQ(q.Front(), 1);
  q.Dequeue();
  EXPECT_EQ(q.Min(), 2);
  q.Dequeue();
  EXPECT_EQ(q.Min(), 3);
}

TEST(MinQueueTest, ClearTest) {
  MinQueue q;
  q.Enqueue(10);
  q.Enqueue(20);
  q.Clear();

  EXPECT_EQ(q.Size(), 0);
  q.Enqueue(5);
  EXPECT_EQ(q.Front(), 5);
  EXPECT_EQ(q.Min(), 5);
  EXPECT_EQ(q.Dequeue(), 5);
  EXPECT_EQ(q.Size(), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
