#include <iostream>
#include "serial/serial.h"
#include "lx16a.hpp"

void enumerate_ports()
{
    std::vector<serial::PortInfo> devices_found = serial::list_ports();
    std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while( iter != devices_found.end() )
    {
        serial::PortInfo device = *iter++;

        printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
            device.hardware_id.c_str() );
    }
}

size_t lx16a::servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1, uint16_t part2){
    if (port_st->port.isOpen())
    {
        uint8_t buf[10] = {0x55, 0x55};

        buf[2] = id;
        buf[3] = 7;
        buf[4] = cmd;

        buf[5] = 0xFF & part1;
        buf[6] = 0xFF & (part1 >> 8);

        buf[7] = 0xFF & part2;
        buf[8] = 0xFF & (part2 >> 8);

        uint8_t chksm = 0;

        for (int i=2; i<9; i++)
        {
            chksm += buf[i];
        }
        chksm = ~chksm;

        buf[9] = chksm;
        
        return port_st->port.write(buf, 10);
    }

    return 0;
}

bool lx16a::check_id(unsigned int id){
    return 1;
}

lx16a::lx16a(ser_port* struct_ptr, unsigned int id = 256, unsigned int nickname = 256, unsigned int delay = 0)
{
    alias = nickname;
    port_ptr = struct_ptr;

    servo_id = id;

}