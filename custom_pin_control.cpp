#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>

bool regist_pin (int pin_num);
bool set_direction (int pin_num, char* direction);
bool set_value (int pin_num, int value);
int get_value (int pin_num);


int main(){
    
    regist_pin(12);
    regist_pin(39);
    regist_pin(42);
    set_direction(12, "out");
    set_direction(39, "in");
    set_direction(42, "in");
    
    std::cout << "pin12 : " << get_value(12) << std::endl;
    std::cout << "pin39 : " << get_value(39) << std::endl;
    std::cout << "pin42 : " << get_value(42) << std::endl;
    
    return 0;

}

bool regist_pin (int pin_num){
    std::fstream reg_pin;
    reg_pin.open("/sys/class/gpio/export", std::ios::out);
    if (!reg_pin.is_open()){
        printf("can not open \"/sys/class/gpio/export\", please use sudo");
        return false;
    }
    reg_pin << pin_num;
    reg_pin.close();
    return true;
}

bool set_direction (int pin_num, char* direction){
    std::fstream pin;
    char* filepath;
    sprintf(filepath, "/sys/class/gpio/gpio%d/direction", pin_num);
    pin.open(filepath, std::ios::out);
    if (!pin.is_open()){
        printf("can not open \"%s\", please use sudo", filepath);
        return false;
    }
    pin << direction;
    pin.close();
    return true;
}
bool set_value (int pin_num, int value){
    std::fstream pin;
    char* filepath;
    sprintf(filepath, "/sys/class/gpio/gpio%d/value", pin_num);
    pin.open(filepath, std::ios::out);
    if (!pin.is_open()){
        printf("can not open \"%s\", please use sudo", filepath);
        return false;
    }
    pin << value;
    pin.close();
    return true;
}
int get_value (int pin_num){
    std::fstream pin;
    char* filepath;
    int value;
    sprintf(filepath, "/sys/class/gpio/gpio%d/value", pin_num);
    pin.open(filepath, std::ios::in);
    if (!pin.is_open()){
        printf("can not open \"%s\", please use sudo", filepath);
        exit(1);
    }
    pin >> value;
    pin.close();
    
    return value;
}
