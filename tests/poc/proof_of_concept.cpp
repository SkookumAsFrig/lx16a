#include <iostream>
#include <stdio.h>
#include "serial/serial.h"
#include <unistd.h>
#include <chrono>
#include "lx16a.hpp"

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

        size_t servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1, uint16_t part2){
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

                buf[9] = ~chksm;
                
                return port_st->port.write(buf, 10);
            }

            return 0;
        }

        size_t servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1){
            if (port_st->port.isOpen())
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
                
                return port_st->port.write(buf, 8);
            }

            return 0;
        }

        size_t servo_write_cmd (uint8_t id, uint8_t cmd){
            if (port_st->port.isOpen())
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
                
                return port_st->port.write(buf, 6);
            }

            return 0;
        }

        std::string query_port()
        {
            return port_st->port.getPort();
        }

        bool query_open(){
            return port_st->port.isOpen();
        }

        unsigned int query_timeout(){
            return port_st->port.getTimeout().read_timeout_constant;
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
    serial::Timeout chain_time= serial::Timeout::simpleTimeout(10);
    std::cout<<chain_time.read_timeout_constant<<std::endl;
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
    std::cout<<testtest.query_open()<<std::endl;

    std::cout<<"Print Timeout"<<std::endl;
    std::cout<<testtest.query_timeout()<<std::endl;

    uint8_t buf[8] = {0x55, 0x55};

    uint16_t part1 = 1000;
    uint16_t part2 = 5247;

    buf[4] = 0xFF & part1;
    buf[5] = 0xFF & (part1 >> 8);

    buf[6] = 0xFF & part2;
    buf[7] = 0xFF & (part2 >> 8);

    for(int i=0; i<8; i++){
        printf("%X, ", buf[i]);
        if (i==7) printf("\n");
    }

    printf("%X", SERVO_MOVE_STOP);

    size_t sz = 10;

    std::cout<<testtest.servo_write_cmd(1, SERVO_MODE_WRITE, 1, 500)<<std::endl;
    usleep(1000000);
    std::cout<<testtest.servo_write_cmd(1, SERVO_MODE_WRITE, 1, 0)<<std::endl;

    int buffsize = 10;
    uint8_t rcev_buf [buffsize];


    auto begin = std::chrono::high_resolution_clock::now();

    // for(int i=0; i<500; i++){

        testtest.port_st->port.flushInput();

        // testtest.servo_write_cmd(1, TEMP_READ);
        // testtest.servo_write_cmd(1, POS_READ);
        testtest.servo_write_cmd(3, ERROR_READ);

        auto end = std::chrono::high_resolution_clock::now();

        std::cout <<testtest.port_st->port.read(rcev_buf, buffsize)<< std::endl;

        // printf("%d Position\n", rcev_buf[5] | (0xFF00 & (rcev_buf[6] << 8)));
        for(int i=0; i<buffsize; i++){
            printf("%X, ", rcev_buf[i]);
            if (i==buffsize-1) printf("\n");
        }
        // }

    auto end2 = std::chrono::high_resolution_clock::now();

    std::cout << "Send Time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Recv Time = " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - end).count() << "[µs]" << std::endl;

    float time_f = std::chrono::duration<float, std::milli>(end2 - end).count();

    printf("time in float is %.5fms \n", time_f);
    

    for(int i=0; i<9; i++){
        std::cout <<led_err.at(i)<< std::endl;
    }

    return 0;
}