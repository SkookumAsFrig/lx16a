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
        lx16a (ser_port* struct_ptr, unsigned int id, unsigned int nickname = 0);
        lx16a (const lx16a_servo &servo, unsigned int id = 256, unsigned int nickname = 256);
        std::string query_port ();
        bool query_open ();
        unsigned int query_timeout ();
        unsigned int check_temp (unsigned int id);
        void unload (unsigned int id);
        void cmd_servo_pos (unsigned int id, unsigned int pos);
        void motor_power (unsigned int id, int pwr);
        unsigned int check_pos (unsigned int id);
};

