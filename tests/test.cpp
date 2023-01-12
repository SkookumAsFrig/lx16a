#include <iostream>
#include "serial/serial.h"

struct ser_port
{
    unsigned int group_id;
    serial::Serial port;
};

class lx16a_chain
{
    private:
        unsigned int servo_id;
        unsigned int alias;
        unsigned int ser_delay;
        
    public:
        ser_port* port_st;
        lx16a_chain(ser_port* port_add)
        {
            port_st = port_add;
        }

        std::string query_port()
        {
            return port_st->port.getPort();
        }

        void query_open(){
            if(port_st->port.isOpen())
                std::cout << " Yes." << std::endl;
            else
                std::cout << " No." << std::endl;
        }

        unsigned int query_timeout(){
            return port_st->port.getTimeout().inter_byte_timeout;
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

    // serial::Serial haha("/dev/ttyUSB0", 115200, serial::Timeout::simpleTimeout(500));
    // serial::Serial* news_port = &haha;
    // std::cout<<news_port->getPort()<<std::endl;

    // std::cout<<"Hello World"<<std::endl;
    // std::cout << "Is the serial port open?"<<std::endl;
    // // if(testtest.port_st.port->isOpen())
    // if(news_port->isOpen())
    //     std::cout << " Yes." << std::endl;
    // else
    //     std::cout << " No." << std::endl;

    // std::cout<<"Hello World"<<std::endl;

    

    ser_port port_struct;
    port_struct.group_id = 1;
    port_struct.port.setPort("/dev/ttyUSB0");
    port_struct.port.setBaudrate(115200);
    serial::Timeout chain_time= serial::Timeout::simpleTimeout(1000);
    std::cout<<chain_time.inter_byte_timeout<<std::endl;
    port_struct.port.setTimeout(chain_time);
    port_struct.port.open();

    lx16a_chain testtest(&port_struct);

    std::cout<<"Hello World"<<std::endl;
    // std::cout<<testtest.port_st.port<<std::endl;
    std::cout << "Is the serial port open?"<<std::endl;
    if(testtest.port_st->port.isOpen())
        std::cout << " Yes." << std::endl;
    else
        std::cout << " No." << std::endl;

    std::cout<<"Print Baud"<<std::endl;
    std::cout<<testtest.port_st->port.getBaudrate()<<std::endl;

    std::cout<<"Print Private Port"<<std::endl;
    std::cout<<testtest.query_port()<<std::endl;

    std::cout<<"Check Open Port"<<std::endl;
    testtest.query_open();

    std::cout<<"Print Timeout"<<std::endl;
    std::cout<<testtest.query_timeout()<<std::endl;

    return 0;
}