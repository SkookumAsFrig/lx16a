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

    lx16a servo(&port_struct, 1, 1);

    std::cout<<servo.check_temp()<<std::endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int time = 0;

    while (time < 4)
    {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
        servo.motor_power(400);
        std::cout<<servo.check_pos()<<std::endl;
        // std::cout<<time<<std::endl;
    }

    servo.unload();

    // servo.cmd_servo_pos(500);

    return 0;
}