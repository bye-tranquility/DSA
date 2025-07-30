#include "sparse_table.hpp"

#include <gtest/gtest.h>

TEST(SparseTableTest, Test) {
  std::vector<long long> arr{-100, 200, 70, -300, 0};
  SparseTable st(arr);
  EXPECT_EQ(st.MinQuery(0, 2), -100);
  EXPECT_EQ(st.PreminQuery(0, 2), 70);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
