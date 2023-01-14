#include <iostream>
#include <algorithm>
#include "serial/serial.h"

static const uint8_t MOVE_WRITE = 1;
static const uint8_t POS_READ = 28;
static const uint8_t SERVO_MODE_WRITE = 29;
static const uint8_t LOAD_UNLOAD_WRITE = 31;
static const uint8_t SERVO_MOVE_STOP = 12;
static const uint8_t TEMP_READ = 26;
static const uint8_t ID_READ = 14;

void enumerate_ports ();

struct ser_port
{
    unsigned int group_id;
    serial::Serial port;
};
// need to write comments explaning how and why to use each function
class lx16a
{
    private:
        unsigned int servo_id;
        unsigned int alias;
        ser_port* port_ptr;
        
        size_t servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1, uint16_t part2);
        size_t servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1);
        size_t servo_write_cmd (uint8_t id, uint8_t cmd);
        bool check_id (unsigned int id);
        
    public:
        // constructors
        lx16a (ser_port* struct_ptr, unsigned int id = 256, unsigned int nickname = 256);
        lx16a (const lx16a& servo, unsigned int id = 256, unsigned int nickname = 256);
        // serial port tools
        std::string query_port ();
        bool query_open ();
        unsigned int query_timeout ();
        // utility functions
        unsigned int check_temp ();
        void unload ();
        void cmd_servo_pos (unsigned int pos);
        void motor_power (int pwr);
        unsigned int check_pos ();
};

