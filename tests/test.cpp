#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <chrono>

#include "serial/serial.h"
#include "lx16a.hpp"

int main()
{

    enumerate_ports();
    std::cout<<"List Port Finished"<<std::endl;

    ser_port port_struct("/dev/ttyUSB0", 115200, 20, 1);

    // Alternative way to initialize serial port struct:
    // port_struct.group_id = 1;
    // port_struct.port.setPort("/dev/ttyUSB0");
    // port_struct.port.setBaudrate(115200);
    // serial::Timeout chain_time= serial::Timeout::simpleTimeout(20);
    // port_struct.port.setTimeout(chain_time);
    // port_struct.port.open();

    lx16a servo1(&port_struct, 1, 1);
    lx16a servo3(&port_struct, 3, 3);

    std::cout<<servo1.check_temp()<<std::endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int time = 0;

    while (time < 2)
    {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
        servo1.motor_power(400);
        servo3.motor_power(800);
        std::cout<<"servo1 "<<servo1.check_pos()<<std::endl;
        std::cout<<"servo3 "<<servo3.check_pos()<<std::endl;
        // std::cout<<time<<std::endl;
    }

    // std::cout<<servo.set_hw_id(3)<<std::endl;

    // std::cout<<servo.check_id(3)<<std::endl;

    // std::cout<<"servo_id is "<<servo.get_id()<<std::endl;

    std::cout<<"struct change status is "<<servo3.set_ser_struct(&port_struct)<<std::endl;

    servo1.cmd_servo_pos(500);

    servo3.cmd_servo_pos(0);

    usleep(1000000);

    servo1.unload();
    servo3.unload();

    return 0;
}