#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sstream>

void register_pin(int pin);
void drop_pin(int pin);
void set_direction(int pin, std::string direction);

int get_value(int pin);
void set_value(int pin, int value);

int main()
{
    int pin1 = 42; // lock sensor (0: unlocked, 1:locked)
    int pin2 = 39; // door sensor (0: clesed, 1:opened)
    int pin3 = 12; // lock trigger (0: unlock, 1: lock)

    register_pin(pin1);
    register_pin(pin2);
    register_pin(pin3);
    set_direction(pin3, "in");
    set_direction(pin3, "in");
    set_direction(pin3, "out");
    set_value(pin3, 1);


    int status [3];
    while(true){
//        sleep(1);
        usleep(1000);
        status[0] = get_value(pin1);
        status[1] = get_value(pin2);
        status[2] = get_value(pin3);
        if (status[0] == 1 && status[1] == 0 && status[2] == 1){
//            std::cout << "wait_state" << std::endl;
        }
        else if (status[0] == 0 && status[1] == 1 && status[2] == 0){
//            std::cout << "ready_state" << std::endl;
            set_value(pin3, 1);
            while(get_value(pin2) == 1){
//                std::cout << "door opened" << std::endl;
                usleep(1000);
            }
//            std::cout << "door closed" << std::endl;
        }
    }


    std::cout << get_value(pin1) << std::endl;
    std::cout << get_value(pin2) << std::endl;
    std::cout << get_value(pin3) << std::endl;

    set_value(pin3, 1);
    set_direction(pin3, "out");

    drop_pin(pin1);
    drop_pin(pin2);
    drop_pin(pin3);

//    test01
//    std::to_string(pin).c_str()

    return 0;
}

void register_pin(int pin){
    std::stringstream s1;
    // create pin code
    int fd = open("/sys/class/gpio/export", O_WRONLY); // create export fd
    char* dummy;
    s1 << pin;
    dummy = new char [s1.str().size()];
    s1 >> dummy; // int pin -> char* pin
    write(fd, dummy, strlen(dummy)); // create pin
    close(fd);
    delete dummy;
}
void drop_pin(int pin){
    std::stringstream s1;
    // create pin code
    int fd = open("/sys/class/gpio/unexport", O_WRONLY); // create export fd
    char* dummy;
    s1 << pin;
    dummy = new char [s1.str().size()];
    s1 >> dummy; // int pin -> char* pin
    write(fd, dummy, strlen(dummy)); // create pin
    close(fd);
    delete dummy;
}


void set_direction(int pin, std::string direction){
    std::stringstream s1;
    char* dummy = new char [direction.size()];
    s1 << "/sys/class/gpio/gpio" << pin << "/direction";
    int fd = open(s1.str().c_str(), O_WRONLY);
    s1.str(""); // clear s1
    s1.clear(); // clear s1
    write(fd, direction.c_str(), direction.size());
    close(fd);
    delete dummy;
}

int get_value(int pin){
    int result;

    std::stringstream s1;
    char* dummy = new char [3];
    s1 << "/sys/class/gpio/gpio" << pin << "/value";
    int fd = open(s1.str().c_str(), O_RDONLY);
    s1.str(""); // clear s1
    s1.clear(); // clear s1
    read(fd, dummy, 3);
    s1 << dummy;
    s1 >> result;
    close(fd);
    delete dummy;

    return result;
}

void set_value(int pin, int value){
    std::stringstream s1;
    char* dummy = new char [1];
    s1 << "/sys/class/gpio/gpio" << pin << "/value";
//    std::cout << s1.str() << std::endl;
    int fd = open(s1.str().c_str(), O_WRONLY);
//    std::cout << "fd : " << fd << std::endl;
    s1.str(""); // clear s1
    s1.clear(); // clear s1
    s1 << value;
    s1 >> dummy;
    int temp = write(fd, dummy, 1);
//    std::cout << "result : " << temp << std::endl;
    close(fd);
    delete dummy;
}
