#include <iostream>
#include <vector>
#include "serial/serial.h"

struct ser_port
{
    uint8_t group_id;
    serial::Serial port;
};

class lx16a_servo
{
    private:
        uint8_t servo_id;
        ser_port port_struct;
        uint8_t alias;
        uint32_t ser_delay;

        bool check_id(uint8_t id);
        size_t servo_write_cmd(uint8_t id, uint8_t cmd, uint8_t part1 = NULL, uint8_t part2 = NULL);
        
    public:
        lx16a_servo (uint8_t id, ser_port p_str, uint8_t nickname = NULL, uint32_t delay = 0);
        lx16a_servo (const lx16a_servo &servo);
        uint8_t check_temp();
        void unload();
        void cmd_servo_pos();
        void motor_power();
        uint16_t check_position();
};

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

int main()
{
    std::cout<<"Hello World"<<std::endl;
    enumerate_ports();

    std::vector<int> n{6, 7, 8, 9, 10};

    lx16a_chain first_chain(n);
    std::vector<int> haha = first_chain.get_temps();

    for (int x : haha)
    {
        std::cout<<x<<std::endl;
    }

    std::cout<<&haha<<std::endl;
    std::cout<<&n<<std::endl;

    return 0;
}