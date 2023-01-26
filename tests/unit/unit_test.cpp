#include "gtest/gtest.h"
#include "lx16a.hpp"

TEST(FuncUnitTests, UnloadTest){
    EXPECT_EQ(1, 2) << "1 and 2 should return failed test";
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}