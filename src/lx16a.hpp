#include <iostream>
#include "serial/serial.h"

struct ser_port
{
    unsigned int group_id;
    serial::Serial port;
};

class lx16a
{
    private:
        unsigned int servo_id;
        unsigned int alias;
        unsigned int ser_delay;
        ser_port* port_ptr;
        
        size_t servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1, uint16_t part2);
        bool check_id (unsigned int id);
        
    public:
        lx16a (ser_port* struct_ptr, unsigned int id, unsigned int nickname = 0, unsigned int delay = 0);
        lx16a (const lx16a_servo &servo);
        std::string query_port ();
        bool query_open ();
        unsigned int query_timeout ();
        unsigned int check_temp ();
        void unload ();
        void cmd_servo_pos ();
        void motor_power ();
        unsigned int check_position ();
        ~lx16a ();
};

