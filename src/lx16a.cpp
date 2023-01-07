#include <iostream>
#include "serial/serial.h"

class lx16a_chain
{
    private:
        unsigned char *servo_id;
        port;
        alias;
        char *temp;
        int *position;
        int *motor_power;
        
    public:
        lx16a_chain ( );
        void setmode();
        void 
        void unload();
        int* get_temps();
}

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

    return 0;
}