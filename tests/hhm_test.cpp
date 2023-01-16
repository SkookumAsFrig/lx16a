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

    servo1.set_vin_limit(6503, 11060);
    servo3.set_vin_limit(5920, 11590);

    unsigned int low_lim1, high_lim1, low_lim3, high_lim3;

    std::tie(low_lim1, high_lim1) = servo1.read_vin_limit();
    std::tie(low_lim3, high_lim3) = servo3.read_vin_limit();

    printf("servo 1 low limit is %d, high limit is %d\n", low_lim1, high_lim1);
    printf("servo 3 low limit is %d, high limit is %d\n", low_lim3, high_lim3);

    servo1.set_temp_limit(82);
    servo3.set_temp_limit(75);

    std::cout<<servo1.read_temp_limit()<<std::endl;
    std::cout<<servo3.read_temp_limit()<<std::endl;

}