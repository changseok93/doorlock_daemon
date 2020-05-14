#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
int main()
{
    // create pin
    std::fstream regist_pin;
    regist_pin.open("/sys/class/gpio/export", std::ios::out);
    if (!regist_pin.is_open()){
        std::cout << "[sys] can not open file \"/sys/class/gpio/export\" please check again" << std::endl;
        std::exit(1);
    }

    regist_pin << 12;
    regist_pin.seekg(0);
    regist_pin << 39;
    regist_pin.seekg(0);
    regist_pin << 42;
    regist_pin.seekg(0);
    regist_pin.close();

    // set direction
    std::fstream pin12; // lock trigger, dir:out
    std::fstream pin39; // door sensor, dir:in
    std::fstream pin42; // lock sensor, dir:in
    pin12.open("/sys/class/gpio/gpio12/direction");
    pin39.open("/sys/class/gpio/gpio39/direction");
    pin42.open("/sys/class/gpio/gpio42/direction");
    pin12 << "out";
    pin39 << "in";
    pin42 << "in";
    pin12.close();
    pin39.close();
    pin42.close();

    // assign pin
    pin12.open("/sys/class/gpio/gpio12/value");
    pin39.open("/sys/class/gpio/gpio39/value");
    pin42.open("/sys/class/gpio/gpio42/value");

    // default state : lock
    pin12 << "1";
    pin12.seekg(0);

    // run daemon
    int status[3];
    int temp;

    while(true) {
        usleep(1000);

        pin12 >> status[0];
        pin39 >> status[1];
        pin42 >> status[2];

        pin12.seekg(0);
        pin39.seekg(0);
        pin42.seekg(0);

        if (status[0] == 0 && status[1] == 1 && status[2] == 0) {
            std::cout << "door open" << std::endl;
            pin12 << "1";
            pin12.seekg(0);
            while (true) {
                usleep(1000);
                pin39 >> temp;
                pin39.seekg(0);
                if (temp == 0)
                    break;
            }
        }
    }
    return 0;
}
