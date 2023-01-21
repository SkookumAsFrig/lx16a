#include <iostream>
#include <algorithm>
#include <tuple>
#include <array>
#include "serial/serial.h"

// See docs/lx16a_protocol.pdf for servo command reference
constexpr uint8_t MOVE_WRITE = 1,
POS_READ = 28,
SERVO_MODE_WRITE = 29,
LOAD_UNLOAD_WRITE = 31,
SERVO_MOVE_STOP = 12,
TEMP_READ = 26,
ID_READ = 14,
ID_WRITE = 13,
VIN_LIMIT_WRITE = 22,
VIN_LIMIT_READ = 23,
TEMP_MAX_LIMIT_WRITE = 24,
TEMP_MAX_LIMIT_READ = 25,
ERROR_READ = 36;
// End of servo command constants

// Helper function from serial library example to list available serial ports
// Prints all serial ports to commandline
void enumerate_ports ();

// Decoding dictionary of servo LED faults for read_faults()
static const std::array<std::string, 9> led_err =
    {"No Alarm",
    "Over Temperature",
    "Over Voltage",
    "Over Temp and Voltage",
    "Locked Rotor",
    "Over Temp and Stalled",
    "Over Voltage and Stalled",
    "Over Temp, Voltage and Stalled",
    "Read Fault Failed"};

// Serial port struct to group together port object and associated data
struct ser_port
{
    // Number id for serial port group (for identifying servo chain on a particular board)
    unsigned int group_id;
    // Serial port object from serial class: https://github.com/wjwwood/serial
    serial::Serial port;

    // Initializable with default constructor, and using serial port set functions
    ser_port () = default;

    // Recommended constructor assigns serial port parameters in one step.
    /*!
    \param portname: string of serial device port, in unix may be something like "/dev/ttyUSB0".
    \param baud: int baudrate, lx16a datasheet calls for 115200.
    \param timeout: int serial port timeout in milliseconds, in testing lx16a shows it
        needs at least 5ms to receive and send message to host, setting to 0 will cause
        read messages to fail.
    \param g_id: int id for serial port group.
    */
    ser_port (const std::string &portname, unsigned int baud = 115200, unsigned int timeout = 5, unsigned int g_id = 0);
};

// lx16a servo class
class lx16a
{
    private:
        // Hardware servo id, range 0-253, settable and can be queried
        unsigned int servo_id;
        // User defined id for servo
        unsigned int alias;
        // ser_port struct pointer, for storing servo serial port and group id information
        ser_port* port_ptr;
        
        // Overloaded servo write command functions, for different command variations
        /*!
        \param id: unsigned char specifying servo id for command
        \param cmd: unsigned char specifying command type, see top of file for list
        \param part1: unsigned short for first command parameter
        \param part2: unsigned short for second command parameter
        \return A size_t of the number of bytes written, should expect 10, 8 or 6
        */
        size_t 
        servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1, uint16_t part2);
        size_t 
        servo_write_cmd (uint8_t id, uint8_t cmd, uint16_t part1);
        size_t 
        servo_write_cmd (uint8_t id, uint8_t cmd);
        
    public:
        // Constructors
        /*!
        \param struct_ptr: pointer to serial port struct for communications
        \param int id: servo hardware id
        \param int nickname: servo user defined id
        \param bool new_safe_lim: set safety limits for new hardware (writes in persistent memory, should be set
        to false for old servos). Vin range: 5-9.5V, max temp: 80C.
        */
        lx16a (ser_port* struct_ptr, unsigned int id = 256, unsigned int nickname = 256, bool new_safe_lim = false);

        // copy and modify constructor for generating servos on chain
        /*!
        \param servo: base lx16a object to copy from
        \param id: new int id for copied servo object (i.e. next one on chain)
        \param nickname: new int alias for copied servo object
        \param bool new_safe_lim: set safety limits for new hardware (writes in persistent memory, should be set
        to false for old servos)
        */
        lx16a (const lx16a& servo, unsigned int id = 256, unsigned int nickname = 256, bool new_safe_lim = false);

        ///////////////////////////////////////////////////////////////////////////////////////////////

        // Getter functions
        // get servo_id of current lx16a object
        /*!
        \return: int representing servo_id parameter of object, corresponding to the hardware servo id
        */
        unsigned int 
        get_id();

        // get alias of current lx16a object
        /*!
        \return: int representing alias parameter of object
        */
        unsigned int 
        get_alias();

        // get serial port struct pointer of current lx16a object
        /*!
        \return: ser_port pointer representing associated ser_port struct of object
        */
        ser_port* 
        get_ser_struct();

        ///////////////////////////////////////////////////////////////////////////////////////////////

        // Setter functions
        // set servo_id of current lx16a object. Note this is not setting servo hardware id, just modifying
        // software object id.
        /*!
        \param id: new int id for servo object
        \return: bool representing if servo_id change is successful. Will fail if id does not exist on chain
        */
        bool 
        set_obj_id(unsigned int id);

        // set alias of current lx16a object.
        /*!
        \param nickname: new int alias for servo object
        */
        void 
        set_alias(unsigned int nickname);

        // set ser_port pointer of current lx16a object.
        /*!
        \param new_struct_ptr: new ser_port struct pointer for servo object
        \return: bool representing if new serial port is open, and the current servo_id exists on chain
        */
        bool 
        set_ser_struct(ser_port* new_struct_ptr);

        ///////////////////////////////////////////////////////////////////////////////////////////////

        // Serial Port Tools
        // helper function, returns servo serial port as string
        std::string 
        query_port ();
        
        // helper function, returns boolean if servo serial port is open
        bool 
        query_open ();

        // helper function, returns unsigned int of serial timeout object value field
        unsigned int 
        query_timeout ();

        ///////////////////////////////////////////////////////////////////////////////////////////////

        // Hardware Safety Tools
        // sets voltage input safety limits in hardware, in milliVolts. Should be used sparingly due to
        // writing in servo persistent memory
        /*!
        \param low_lim: int voltage lower limit in mV, range 4500-12000
        \param high_lim: int voltage higher limit in mV, range 4500-12000
        high_lim should be greater than low_lim, function enforces it too.
        */
        void 
        set_vin_limit(unsigned int low_lim, unsigned int high_lim);

        // read the voltage input safety limits from persistent memory
        /*!
        \return: std::tuple of low and high vin limits, use std::tie(low_lim, high_lim) to decode results
        */
        std::tuple<unsigned int, unsigned int> 
        read_vin_limit();

        // sets max temperature limit in hardware, in degrees Celcius. Should be used sparingly due to
        // writing in servo persistent memory
        /*!
        \param max_temp: int max temperature limit in degrees Celcius
        */
        void 
        set_temp_limit(unsigned int max_temp);

        // read the max temperature limit from persistent memory
        /*!
        \return: int max temperature limit in degrees Celcius
        */
        unsigned int 
        read_temp_limit();

        // read the current fault
        /*!
        \return: std::string specifying fault according to API
        */
        std::string 
        read_faults();

        ///////////////////////////////////////////////////////////////////////////////////////////////

        // Utility Functions
        // query temperature of servo
        /*!
        \return: int representing celcius temperature of current servo
        */
        unsigned int 
        check_temp ();

        // stops and unloads servo, regardless of servo or motor mode
        void 
        unload ();

        // commands servo to move to desired position in servo mode
        /*!
        \param pos: int ranging 0 to 1000 position command corresponding to 0-240 degree
            range of motion. Function automatically limits input if out of range.
        */
        void 
        cmd_servo_pos (unsigned int pos);

        // commands servo in motor mode with desired power
        /*!
        \param pwr: signed int ranging -1000 to 1000 corresponding to direction and 1000
            divisions of power. Function automatically limits input if out of range.
        */
        void 
        motor_power (int pwr);

        // query position of servo
        /*!
        \return: short representing position of current servo, between 0-1000. Can return negative,
            or above 1000 in the other direction. Out of range behaviors necessitate the 16 bit
            short data type to decode the sign correctly.
        */
        short 
        check_pos ();

        // Helper function to ensure assigned id exists in hardware on serial port servo chain
        /*!
        \param id: int id that needs to be verified if it exists
        \return A boolean value if the id is valid and exists in hardware
        */
        bool 
        check_id (unsigned int new_id);

        // Change servo hardware id
        /*!
        \param new_id: new int id that the servo hardware will be assigned
        \return A boolean value if the id is valid in hardware after assignment
        */
        bool 
        set_hw_id (unsigned int new_id);

};

class SerialReadException : public std::exception
{
    // Code referenced from tune2fs/OverShifted stackedoverflow
    public:
        /** Constructor (C strings).
         *  @param message C-style string error message.
         *                 The string contents are copied upon construction.
         *                 Hence, responsibility for deleting the char* lies
         *                 with the caller. 
         */
        explicit SerialReadException(const char* message)
            : msg_(message) {}

        /** Constructor (C++ STL strings).
         *  @param message The error message.
         */
        explicit SerialReadException(const std::string& message)
            : msg_(message) {}

        /** Destructor.
         * Virtual to allow for subclassing.
         */
        virtual ~SerialReadException() noexcept {}

        /** Returns a pointer to the (constant) error description.
         *  @return A pointer to a const char*. The underlying memory
         *          is in posession of the Exception object. Callers must
         *          not attempt to free the memory.
         */
        virtual const char* what() const noexcept {
        return msg_.c_str();
        }

    protected:
        /** Error message.
         */
        std::string msg_;
};

class HardwareException : public std::exception
{
    // Code referenced from tune2fs/OverShifted stackedoverflow
    public:
        /** Constructor (C strings).
         *  @param message C-style string error message.
         *                 The string contents are copied upon construction.
         *                 Hence, responsibility for deleting the char* lies
         *                 with the caller. 
         */
        explicit HardwareException(const char* message)
            : msg_(message) {}

        /** Constructor (C++ STL strings).
         *  @param message The error message.
         */
        explicit HardwareException(const std::string& message)
            : msg_(message) {}

        /** Destructor.
         * Virtual to allow for subclassing.
         */
        virtual ~HardwareException() noexcept {}

        /** Returns a pointer to the (constant) error description.
         *  @return A pointer to a const char*. The underlying memory
         *          is in posession of the Exception object. Callers must
         *          not attempt to free the memory.
         */
        virtual const char* what() const noexcept {
        return msg_.c_str();
        }

    protected:
        /** Error message.
         */
        std::string msg_;
};
