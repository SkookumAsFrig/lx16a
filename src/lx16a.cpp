#include <iostream>
#include <algorithm>
#include "serial/serial.h"
#include "lx16a.hpp"

void enumerate_ports ()
{
    // Function borrowed from wjwwood serial library example
    
    std::vector<serial::PortInfo> devices_found = serial::list_ports();
    std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while( iter != devices_found.end() )
    {
        serial::PortInfo device = *iter++;

        printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
            device.hardware_id.c_str() );
    }
}

lx16a::lx16a (ser_port* struct_ptr, unsigned int id = 256, unsigned int nickname = 256)
{
    alias = nickname;
    port_ptr = struct_ptr;

    if (check_id(id)){
        servo_id = id;
    }
    // else throw warning exception id does not exist in hardware
}

lx16a::lx16a (const lx16a_servo &servo, unsigned int id = 256, unsigned int nickname = 256)
{
    // copy constructor + mutator for quickly creating servos in chain
    port_ptr = servo.port_ptr;
    alias = (nickname == 256) ? servo.alias : nickname;
    servo_id = (id == 256) ? servo.servo_id : id;
}

size_t lx16a::servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1, uint16_t part2)
{
    if (port_ptr->port.isOpen())
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

        buf[9] = ~chksm;
        
        return port_ptr->port.write(buf, 10);
    }

    return 0;
}

size_t lx16a::servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1)
{
    if (port_ptr->port.isOpen())
    {
        uint8_t buf[8] = {0x55, 0x55};

        buf[2] = id;
        buf[3] = 5;
        buf[4] = cmd;

        buf[5] = 0xFF & part1;
        buf[6] = 0xFF & (part1 >> 8);

        uint8_t chksm = 0;

        for (int i=2; i<7; i++)
        {
            chksm += buf[i];
        }

        buf[7] = ~chksm;
        
        return port_ptr->port.write(buf, 8);
    }

    return 0;
}

size_t lx16a::servo_write_cmd (uint8_t id, uint8_t cmd)
{
    if (port_ptr->port.isOpen())
    {
        uint8_t buf[6] = {0x55, 0x55};

        buf[2] = id;
        buf[3] = 3;
        buf[4] = cmd;

        uint8_t chksm = 0;

        for (int i=2; i<5; i++)
        {
            chksm += buf[i];
        }

        buf[5] = ~chksm;
        
        return port_ptr->port.write(buf, 6);
    }

    return 0;
}

std::string query_port ()
{
    return port_ptr->port.getPort();
}

bool query_open ()
{
    return port_ptr->port.isOpen();
}

unsigned int query_timeout ()
{
    return port_ptr->port.getTimeout().inter_byte_timeout;
}

bool lx16a::check_id (unsigned int id)
{

    int buffsize = 7;
    uint8_t rcev_buf [buffsize];

    port_ptr->port.flushInput();
    servo_write_cmd(id, ID_READ);

    size_t b_read = port_ptr->port.read(rcev_buf, buffsize);

    if (b_read == 7 && rcev_buf[0] == 0x55 &&
        rcev_buf[0] == 0x55 && && rcev_buf[4] == 0x0E)
    {
        return true;
    }

    return false;
}

unsigned int lx16a::check_temp (unsigned int id)
{

    int buffsize = 7;
    uint8_t rcev_buf [buffsize];

    port_ptr->port.flushInput();
    servo_write_cmd(id, TEMP_READ);

    size_t b_read = port_ptr->port.read(rcev_buf, buffsize);

    if (b_read == 7 && rcev_buf[0] == 0x55 &&
        rcev_buf[0] == 0x55 && && rcev_buf[4] == 0x1A)
    {
        return rcev_buf[5];
    }

    // throw temp read failed warning

    return 256;

}

void lx16a::unload (unsigned int id)
{

    port_ptr->port.flushInput();
    servo_write_cmd(id, LOAD_UNLOAD_WRITE, 0);
    
}

void lx16a::cmd_servo_pos (unsigned int id, unsigned int pos)
{

    port_ptr->port.flushInput();
    servo_write_cmd(id, SERVO_MODE_WRITE, 0);

    pos = std::min(pos, 1000);
    servo_write_cmd(id, MOVE_WRITE, pos);

}

void lx16a::motor_power (unsigned int id, int pwr)
{

    port_ptr->port.flushInput();
    pwr = std::max(std::min(pwr, 1000), -1000);
    servo_write_cmd(id, SERVO_MODE_WRITE, 1, pwr);
    
}

unsigned int lx16a::check_pos (unsigned int id)
{

    int buffsize = 8;
    uint8_t rcev_buf [buffsize];

    port_ptr->port.flushInput();
    servo_write_cmd(id, POS_READ);

    size_t b_read = port_ptr->port.read(rcev_buf, buffsize);

    if (b_read == 8 && rcev_buf[0] == 0x55 &&
        rcev_buf[0] == 0x55 && && rcev_buf[4] == 0x1C)
    {
        return rcev_buf[5] | (0xFF00 & (rcev_buf[6] << 8));
    }

    // throw position read failed warning
    
    return 70000;
}