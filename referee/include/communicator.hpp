
#ifndef __COMMUNICATOR_HPP__
#define __COMMUNICATOR_HPP__

#include <stdint.h>
#include "serial/serial.h"



struct SerialPkg
{
    float w;
    float x; 
	float y; 
	float z;

    float pos_x;
	float pos_y; 
	float pos_z; 

    int16_t empty;
};


class Communicator: public serial::Serial 
{
private:
    bool enable;
    bool connected;
	SerialPkg pkg;
public:
    // 不上车调试时，设enable为false
    Communicator(bool _enable);
    ~Communicator();

    void connect();

    // usage: send({...})
    // 隐式调用SerialPkg构造
    void send(SerialPkg pkg) ;

    int receive(uint8_t *buffer, size_t n);
};



#endif
