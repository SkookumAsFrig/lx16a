#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <tuple>
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

ser_port::ser_port (const std::string &portname, unsigned int baud, unsigned int timeout, unsigned int g_id)
{
    group_id = g_id;

    if (timeout<30)
    {
        std::cout<<"Serial timeout recommended to be greater than 30ms for servo to respond to longer messages"<<std::endl;
        std::cout<<"Current timeout: "<<timeout<<"ms, may encounter command errors"<<std::endl;
        // Replace with warning down the line
    }

    serial::Timeout chain_time= serial::Timeout::simpleTimeout(timeout);
    port.setPort(portname);
    port.setBaudrate(baud);
    port.setTimeout(chain_time);

    port.open();
}

lx16a::lx16a (ser_port* struct_ptr, unsigned int id, unsigned int nickname)
{
    alias = nickname;
    port_ptr = struct_ptr;

    if (check_id(id)){
        servo_id = id;
    }
    else{
        std::cout<<"servo_id does not exist"<<std::endl;
        servo_id = 256;
    }
    // else throw warning exception id does not exist in hardware
}

lx16a::lx16a (const lx16a& servo, unsigned int id, unsigned int nickname)
{
    // copy constructor + mutator for quickly creating servos in chain
    port_ptr = servo.port_ptr;
    alias = (nickname == 256) ? servo.alias : nickname;
    servo_id = (id != 256 && check_id(id)) ? id : servo.servo_id;
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

unsigned int lx16a::get_id()
{
    return servo_id;
}

unsigned int lx16a::get_alias()
{
    return alias;
}

ser_port* lx16a::get_ser_struct()
{
    return port_ptr;
}

bool lx16a::set_obj_id(unsigned int id)
{
    if (check_id(id))
    {
        servo_id = id;
        return true;
    }

    return false;
}

void lx16a::set_alias(unsigned int nickname)
{
    alias = nickname;
}

bool lx16a::set_ser_struct(ser_port* new_struct_ptr)
{
    port_ptr = new_struct_ptr;

    if (query_open() && check_id(servo_id)) return true;

    return false;
}

std::string lx16a::query_port ()
{
    return port_ptr->port.getPort();
}

bool lx16a::query_open ()
{
    return port_ptr->port.isOpen();
}

unsigned int lx16a::query_timeout ()
{
    return port_ptr->port.getTimeout().inter_byte_timeout;
}

void lx16a::set_vin_limit(unsigned int low_lim, unsigned int high_lim)
{
    low_lim = std::max(std::min((int) low_lim, 11999), 4500);
    high_lim = std::max(std::min((int) high_lim, 12000), 4501);

    if (low_lim>= high_lim) low_lim = high_lim-1;

    port_ptr->port.flushInput();
    servo_write_cmd(servo_id, VIN_LIMIT_WRITE, low_lim, high_lim);
}

std::tuple<unsigned int, unsigned int> lx16a::read_vin_limit()
{
    int buffsize = 10;
    uint8_t rcev_buf [buffsize];

    port_ptr->port.flushInput();
    servo_write_cmd(servo_id, VIN_LIMIT_READ);

    size_t b_read = port_ptr->port.read(rcev_buf, buffsize);

    unsigned int low_lim = 0, high_lim = 0;

    if (b_read == 10 && rcev_buf[0] == 0x55 &&
        rcev_buf[1] == 0x55 && rcev_buf[4] == 0x17)
    {
        low_lim = rcev_buf[5] | (0xFF00 & (rcev_buf[6] << 8));
        high_lim = rcev_buf[7] | (0xFF00 & (rcev_buf[8] << 8));
    }

    // throw vin limit read failed warning
    
    return std::make_tuple(low_lim, high_lim);
}

void lx16a::set_temp_limit(unsigned int max_temp)
{
    max_temp = std::max(std::min((int) max_temp, 100), 50);

    port_ptr->port.flushInput();
    servo_write_cmd(servo_id, TEMP_MAX_LIMIT_WRITE, max_temp);
}

unsigned int lx16a::read_temp_limit()
{
    int buffsize = 7;
    uint8_t rcev_buf [buffsize];

    port_ptr->port.flushInput();
    servo_write_cmd(servo_id, TEMP_MAX_LIMIT_READ);

    size_t b_read = port_ptr->port.read(rcev_buf, buffsize);

    if (b_read == 7 && rcev_buf[0] == 0x55 &&
        rcev_buf[1] == 0x55 && rcev_buf[4] == 0x19)
    {
        return rcev_buf[5];
    }

    // throw temp read failed warning

    return 256;
}

bool lx16a::check_id (unsigned int new_id)
{
    if (new_id < 254)
    {
        int buffsize = 7;
        uint8_t rcev_buf [buffsize];

        port_ptr->port.flushInput();
        servo_write_cmd(new_id, ID_READ);

        size_t b_read = port_ptr->port.read(rcev_buf, buffsize);

        if (b_read == 7 && rcev_buf[0] == 0x55 &&
            rcev_buf[1] == 0x55 && rcev_buf[4] == 0x0E)
        {
            return true;
        }
    }

    return false;
}

unsigned int lx16a::check_temp ()
{
    int buffsize = 7;
    uint8_t rcev_buf [buffsize];

    port_ptr->port.flushInput();
    servo_write_cmd(servo_id, TEMP_READ);

    size_t b_read = port_ptr->port.read(rcev_buf, buffsize);

    if (b_read == 7 && rcev_buf[0] == 0x55 &&
        rcev_buf[1] == 0x55 && rcev_buf[4] == 0x1A)
    {
        return rcev_buf[5];
    }

    // throw temp read failed warning

    return 256;

}

void lx16a::unload ()
{

    port_ptr->port.flushInput();
    servo_write_cmd(servo_id, SERVO_MODE_WRITE, 0);
    servo_write_cmd(servo_id, LOAD_UNLOAD_WRITE, 0);
    
}

void lx16a::cmd_servo_pos (unsigned int pos)
{

    port_ptr->port.flushInput();
    servo_write_cmd(servo_id, SERVO_MODE_WRITE, 0);

    unsigned int pos_max = 1000;
    pos = std::min(pos, pos_max);

    servo_write_cmd(servo_id, MOVE_WRITE, pos, 0);
    // Need to set last param move time to 0 to tell servo
    // to move to pos at max velocity

}

void lx16a::motor_power (int pwr)
{

    port_ptr->port.flushInput();
    pwr = std::max(std::min(pwr, 1000), -1000);

    servo_write_cmd(servo_id, SERVO_MODE_WRITE, 1, pwr);
    
}

short lx16a::check_pos ()
{

    int buffsize = 8;
    uint8_t rcev_buf [buffsize];

    port_ptr->port.flushInput();
    servo_write_cmd(servo_id, POS_READ);

    size_t b_read = port_ptr->port.read(rcev_buf, buffsize);

    if (b_read == 8 && rcev_buf[0] == 0x55 &&
        rcev_buf[1] == 0x55 && rcev_buf[4] == 0x1C)
    {
        return rcev_buf[5] | (0xFF00 & (rcev_buf[6] << 8));
    }

    // throw position read failed warning
    
    return 3000;
}

bool lx16a::set_hw_id (unsigned int new_id)
{
    if (new_id < 254)
    {
        port_ptr->port.flushInput();
        servo_write_cmd(servo_id, ID_WRITE, new_id);
        
        // command to delay until servo finishes id update, usleep also works, result not used
        check_id(new_id);

        if (check_id(new_id))
        {
            servo_id = new_id;
            return true;
        }
    }

    return false;
}