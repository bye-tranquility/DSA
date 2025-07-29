#include <gtest/gtest.h>
#include <string>
#include "hash_map.hpp"

TEST(HashMapTest, InsertAndGet) {
  HashMap<std::string, int> map;
  map.Insert("banana", 10);
  map.Insert("apple", 20);
  map.Insert("carrot", 30);

  int val;
  EXPECT_TRUE(map.GetValByKey("banana", val));
  EXPECT_EQ(val, 10);

  EXPECT_TRUE(map.GetValByKey("apple", val));
  EXPECT_EQ(val, 20);

  EXPECT_TRUE(map.GetValByKey("carrot", val));
  EXPECT_EQ(val, 30);
}

TEST(HashMapTest, OverwriteValue) {
  HashMap<int, int> map;
  map.Insert(1, 10);
  map.Insert(1, 42);

  int val;
  EXPECT_TRUE(map.GetValByKey(1, val));
  EXPECT_EQ(val, 42);
}

TEST(HashMapTest, EraseKey) {
  HashMap<int, int> map;
  map.Insert(1, 10);
  int val;

  EXPECT_TRUE(map.GetValByKey(1, val));
  map.Erase(1);
  EXPECT_FALSE(map.GetValByKey(1, val));
}

TEST(HashMapTest, MissingKey) {
  HashMap<int, int> map;
  int val;
  for (int i = 0; i < 1000; ++i) {
    EXPECT_FALSE(map.GetValByKey(i, val));
  }
}

TEST(HashMapTest, RehashingStress) {
  HashMap<int, int> map(2);
  for (int i = 0; i < 1000; ++i) {
    map.Insert(i, i * 2);
  }

  int val;
  for (int i = 0; i < 1000; ++i) {
    EXPECT_TRUE(map.GetValByKey(i, val));
    EXPECT_EQ(val, i * 2);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
