#include <mutex>
#include <thread>
#include <iostream>

#include "communicator.hpp"
#include "send_referee.h"
#include "serial/serial.h"
#include "send_referee.h"

using namespace serial;
using namespace std;

static mutex mtx;

#define CHECK_ENABLE(ret)    \
    if ( false == enable ) { \
        return ret;          \
    }

#define CHECK_CONNECT() {      \
    mtx.lock();                \
    if ( false == connected )  \
        connect();             \
    mtx.unlock();              \
    }


Communicator::Communicator(bool _enable)
 : Serial(), enable(_enable)
{
    CHECK_ENABLE()
    connect();
}

Communicator::~Communicator()
{
    close();
}

void Communicator::connect()
{
    CHECK_ENABLE()
    const string port_names[] = {"/dev/ttyUSB0"};
    int sz = sizeof(port_names) / sizeof(port_names[0]);
    int idx = 0;
    while ( true ) {
        std::cout << "connecting to " << port_names[idx] << std::endl;
        try {
            setPort(port_names[idx]);
            setBaudrate(115200);  // 设置波特率
            auto timeout = Timeout::simpleTimeout(100);
            setTimeout(timeout);
            // open();
            if ( ! isOpen() ) {
                open();
            }
            connected = true;
            break;

        }
        catch ( exception &e ) {
            std::cout << "serial open failed !" << std::endl;
            std::cout << e.what();
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
        idx = (idx + 1) % sz;
    }
}


void Communicator::send(SerialPkg pkg)
{
    CHECK_ENABLE()
    CHECK_CONNECT()
    uint8_t frame[128] = {0};   //最大一帧数据量

    uint8_t frame_len =  referee_send_pose(frame, pkg.pos_x, pkg.pos_y, pkg.pos_z, pkg.w, pkg.x, pkg.y, pkg.z);

    try {
        write(frame, frame_len);
    } catch (std::exception &e) {
        std::cout << e.what();
    }
    // std::cout << "x: " << pkg.pos_x << "\ty : " << pkg.y << "\tz: " << pkg.z << "\troll: " << pkg.roll << "\tpitch: " << pkg.pitch << "\tyaw: " << pkg.yaw << std::endl;
}

int Communicator::receive(uint8_t *buffer, size_t n)
{
    CHECK_ENABLE(0)
    CHECK_CONNECT()
    try {
        
    } catch(std::exception &e) {
        std::cout << e.what();
    }
}
