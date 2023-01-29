#include "gtest/gtest.h"
#include "serial/serial.h"
#include "lx16a.hpp"
#include <boost/algorithm/string.hpp>

/*
// Test that googletest is linked correctly and builds
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
*/

TEST(ConstructorTest, SerPortStructWrongPort) {
  // with serial device on ttyUSB0
  // expect to throw IO Exception with default message plus additional message
  EXPECT_THROW({
      try
      {
        testing::internal::CaptureStderr();
        ser_port port_struct("/dev/ttyUSB1", 115200, 50, 1);
      }
      catch (const serial::IOException e)
      {
        std::string output = testing::internal::GetCapturedStderr();

        EXPECT_TRUE(boost::algorithm::contains(output,
          (std::string("Serial port failed to open. Check that it exists with enumerate_ports().\n"))));
        
        EXPECT_TRUE(boost::algorithm::contains(std::string(e.what()),
          (std::string("IO Exception (2): No such file or directory"))));
        
        throw;
      }

      std::string output = testing::internal::GetCapturedStderr(); //incase not thrown

    }, serial::IOException);
}

TEST(ConstructorTest, SerPortStructRightPort) {
  // with serial device on ttyUSB0
  EXPECT_NO_THROW(ser_port port_struct("/dev/ttyUSB0", 115200, 50, 1));
  ser_port port_struct("/dev/ttyUSB0", 115200, 50, 1);
  EXPECT_EQ(port_struct.group_id, 1);
  EXPECT_EQ(port_struct.port.getBaudrate(), 115200);
  EXPECT_EQ(port_struct.port.getPort(), "/dev/ttyUSB0");
  EXPECT_EQ(port_struct.port.getTimeout().read_timeout_constant, 50);
  EXPECT_EQ(port_struct.port.getTimeout().write_timeout_constant, 50);
}

TEST(ConstructorTest, SerPortStructWrongTimeout) {
  testing::internal::CaptureStderr();
  ser_port port_struct("/dev/ttyUSB0", 115200, 20, 1);
  std::string output = testing::internal::GetCapturedStderr();
  EXPECT_EQ(output, "Serial timeout recommended to be greater than 30ms.\n");
}

TEST(ConstructorTest, SerPortStructRightTimeout) {
  testing::internal::CaptureStderr();
  ser_port port_struct("/dev/ttyUSB0", 115200, 30, 1);
  std::string output = testing::internal::GetCapturedStderr();
  EXPECT_EQ(output, "");
}

namespace {
  class lx16aConstructorTests : public ::testing::Test {
    protected:
      void SetUp() override {
        port_struct_ptr = new ser_port ("/dev/ttyUSB0", 115200, 50, 1);
      }

      void TearDown() {
        port_struct_ptr->port.close();
        delete port_struct_ptr;
      }

      ser_port* port_struct_ptr;
  };

  TEST_F(lx16aConstructorTests, ServoConstWorks) {
    EXPECT_NO_THROW(lx16a servo1(port_struct_ptr, 1, 1));

    lx16a servo1(port_struct_ptr, 1, 5);
    unsigned int low_lim1, high_lim1, temp_max;
    std::tie(low_lim1, high_lim1) = servo1.read_vin_limit();
    temp_max = servo1.read_temp_limit();

    EXPECT_EQ(low_lim1, 5000);
    EXPECT_EQ(high_lim1, 9500);
    EXPECT_EQ(temp_max, 80);
    EXPECT_EQ(servo1.get_id(), 1);
    EXPECT_EQ(servo1.get_alias(), 5);
  }

  TEST_F(lx16aConstructorTests, ServoConstFailsIDRange) {
    EXPECT_THROW({
      try
      {
        lx16a servo1(port_struct_ptr, 255, 1);
      }
      catch (const HardwareException e)
      {
        EXPECT_STREQ("Servo ID does not exist on serial bus.", e.what());
        throw;
      }
    }, HardwareException);
  }

  TEST_F(lx16aConstructorTests, ServoConstFailsIDBus) {
    EXPECT_THROW({
      try
      {
        lx16a servo1(port_struct_ptr, 2, 2);
      }
      catch (const HardwareException e)
      {
        EXPECT_STREQ("Servo ID does not exist on serial bus.", e.what());
        throw;
      }
    }, HardwareException);
  }

  TEST_F(lx16aConstructorTests, ServoCopyWorks) {
    lx16a servo1(port_struct_ptr, 1, 1);
    EXPECT_NO_THROW(lx16a servo3(servo1, 3, 3));

    lx16a servo3(servo1, 3, 6);
    unsigned int low_lim1, high_lim1, temp_max;
    std::tie(low_lim1, high_lim1) = servo3.read_vin_limit();
    temp_max = servo3.read_temp_limit();

    EXPECT_EQ(low_lim1, 5000);
    EXPECT_EQ(high_lim1, 9500);
    EXPECT_EQ(temp_max, 80);
    EXPECT_EQ(servo3.get_id(), 3);
    EXPECT_EQ(servo3.get_alias(), 6);
  }

  TEST_F(lx16aConstructorTests, ServoCopyFailsIDRange) {
    EXPECT_THROW({
      try
      {
        lx16a servo1(port_struct_ptr, 1, 1);
        lx16a servo2(servo1, 255, 2);
      }
      catch (const HardwareException e)
      {
        EXPECT_STREQ("Servo ID does not exist on serial bus.", e.what());
        throw;
      }
    }, HardwareException);
  }

  TEST_F(lx16aConstructorTests, ServoCopyFailsIDBus) {
    EXPECT_THROW({
      try
      {
        lx16a servo1(port_struct_ptr, 1, 1);
        lx16a servo2(servo1, 2, 2);
      }
      catch (const HardwareException e)
      {
        EXPECT_STREQ("Servo ID does not exist on serial bus.", e.what());
        throw;
      }
    }, HardwareException);
  }

}

namespace {
  class lx16aFunctionTests : public ::testing::Test {
    protected:
      void SetUp() override {
        port_struct_ptr = new ser_port ("/dev/ttyUSB0", 115200, 50, 1);
        servo1 = new lx16a(port_struct_ptr, 1, 1);
        servo3 = new lx16a(*servo1, 2, 2);
      }

      void TearDown() {
        port_struct_ptr->port.close();
        delete servo1;
        delete servo3;
        delete port_struct_ptr;
      }

      ser_port* port_struct_ptr;
      lx16a* servo1;
      lx16a* servo3;
  };

  // TEST_F(lx16aFunctionTests, ServoCopyFailsIDBus) {

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}