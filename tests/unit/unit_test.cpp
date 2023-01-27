#include "gtest/gtest.h"
#include "serial/serial.h"
#include "lx16a.hpp"

int Factorial(int n){
  if(n>1) return n*Factorial(n-1);
  else return n;
}

TEST(FactorialTest, Positive) {
  EXPECT_EQ(1, Factorial(1));
  EXPECT_EQ(2, Factorial(2));
  EXPECT_EQ(6, Factorial(3));
  EXPECT_EQ(40320, Factorial(8));
}

TEST(ConstructorTest, SerPortStruct) {
  EXPECT_THROW({
    try
    {
      // with serial device on ttyUSB0
      ser_port port_struct("/dev/ttyUSB1", 115200, 50, 1);
    }
    catch(const serial::IOException se)
    {
      EXPECT_STREQ("IO Exception (2): No such file or directory, file /home/k5200/lx16a/ext/serial/src/impl/unix.cc, line 151.", se.what());
    }
  }, serial::IOException);
}

namespace {
  class lx16aTests : public ::testing::Test {
    protected:
      void SetUp() override {
        ser_port port_struct("/dev/ttyUSB0", 115200, 50, 1);
        lx16a servo1(&port_struct, 1, 1);
        lx16a servo3(&port_struct, 3, 3);
      }
  };
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}