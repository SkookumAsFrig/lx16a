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

    ser_port port_struct("/dev/ttyUSB0", 115200, 50, 1);

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
    std::cout<<servo3.check_temp()<<std::endl;

    // for (int i=0; i<5; i++)
    // {
    // servo1.set_vin_limit(6503+i, 11060+i);
    // servo3.set_vin_limit(5920+i, 11590+i);
    // }

    unsigned int low_lim1, high_lim1, low_lim3, high_lim3;

    for (int i=0; i<5; i++)
    {
    std::tie(low_lim1, high_lim1) = servo1.read_vin_limit();
    std::tie(low_lim3, high_lim3) = servo3.read_vin_limit();

    printf("servo 1 low limit is %d, high limit is %d\n", low_lim1, high_lim1);
    printf("servo 3 low limit is %d, high limit is %d\n", low_lim3, high_lim3);

    // servo1.set_temp_limit(82);
    // servo3.set_temp_limit(75);

    std::cout<<servo1.read_temp_limit()<<std::endl;
    std::cout<<servo3.read_temp_limit()<<std::endl;
    }

    std::cout<<servo1.read_faults()<<std::endl;
    std::cout<<servo3.read_faults()<<std::endl;

    float f_time, f_time2;
    unsigned int temp;
    unsigned int ret_pos;

    std::tie(f_time, temp) = servo1.check_time(&lx16a::check_temp);

    f_time2 = servo1.check_time(&lx16a::motor_power, 300);

    usleep(500000);

    auto begin = std::chrono::high_resolution_clock::now();
    servo1.check_temp();
    auto end = std::chrono::high_resolution_clock::now();

    float manual_time = std::chrono::duration<float, std::milli>(end - begin).count();

    std::cout<<f_time<<", "<<temp<<std::endl;

    std::cout<<"f_time2: "<<f_time2<<std::endl;

    std::tie(f_time, ret_pos) = servo1.check_time(&lx16a::motor_power, &lx16a::check_pos, 500);

    std::cout<<"manual time: "<<manual_time<<std::endl;

    std::cout<<"motor power and check pos time: "<<f_time<<std::endl;

    usleep(500000);

    servo1.motor_power(0);

}