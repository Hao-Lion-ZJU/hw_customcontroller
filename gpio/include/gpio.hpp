#ifndef _GPIO_HPP_
#define _GPIO_HPP_
#include "gpiod.h"
#include <stdint.h>
#include <string>

// 	GPIOD_LINE_REQUEST_DIRECTION_AS_IS = 1,
/**< Request the line(s), but don't change current direction. */
// 	GPIOD_LINE_REQUEST_DIRECTION_INPUT,
/**< Request the line(s) for reading the GPIO line state. */
// 	GPIOD_LINE_REQUEST_DIRECTION_OUTPUT,
/**< Request the line(s) for setting the GPIO line state. */
// 	GPIOD_LINE_REQUEST_EVENT_FALLING_EDGE,
/**< Only watch falling edge events. */
// 	GPIOD_LINE_REQUEST_EVENT_RISING_EDGE,
/**< Only watch rising edge events. */
// 	GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES,
/**< Monitor both types of events. */


class Gpio
{
private:
    std::string chipname_;
    uint8_t line_num_;
    struct gpiod_chip * chip_;      //GPIO控制器句柄
    struct gpiod_line * line_;      //GPIO引脚句柄
    struct gpiod_line_request_config config;//GPIO配置
    struct gpiod_line_event event_;  //中断事件句柄
public:
    Gpio(const std::string chipname, uint8_t line_num, const char* consumer = "blink", int request_tpye = GPIOD_LINE_REQUEST_DIRECTION_INPUT);
    ~Gpio();
    bool open();
    void close();
    
    void write(bool Value);

    bool read();

    bool read_delay(uint16_t delay_time_);

    void configure(int request_tpye);

    int event_wait();

    int event_read();
};

#endif /* _GPIO_HPP_ */
