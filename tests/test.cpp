#include <iostream>
#include "serial/serial.h"
#include <string>

class lx16a_chain
{
    private:
        unsigned int servo_id;
        unsigned int alias;
        unsigned int ser_delay;
        
    public:
        struct ser_port
        {
            unsigned int group_id;
            serial::Serial* port;
        } port_st;

        lx16a_chain(const std::string &portname, unsigned int baud, unsigned int timeout, unsigned int id = 0)
        {
            ser_port port_st;
            port_st.group_id = id;
            serial::Serial new_port(portname, baud, serial::Timeout::simpleTimeout(timeout));
            port_st.port = &new_port;
        }
        // void setmode();
        // void 
        // void unload();
        // int* get_temps();
};

// ser_port(const std::string &portname, unsigned int baud, unsigned int timeout, uint8_t id = 0)
// {
//     group_id = id;
//     serial::Serial port(portname, baud, serial::Timeout::simpleTimeout(timeout));
// }

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

    lx16a_chain testtest("/dev/ttyUSB0", 115200, 1000, 1);
    std::cout<<"Hello World"<<std::endl;
    // serial::Serial news_port("/dev/ttyUSB1", 5400, serial::Timeout::simpleTimeout(500));
    // std::cout<<news_port.getPort()<<std::endl;
    std::cout<<"Hello World"<<std::endl;
    std::cout << "Is the serial port open?";
    if(testtest.port_st.port->isOpen())
        std::cout << " Yes." << std::endl;
    else
        std::cout << " No." << std::endl;
    std::cout<<testtest.port_st.port->getPort()<<std::endl;

    return 0;
}