/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : gpio.cpp
 * @brief     : 读取树莓派GPIO接口
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-7-10       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */

#include "gpio.hpp"
#include "gpiod.hpp"
#include <thread>
#include <chrono>

Gpio::Gpio(const std::string chipname, uint8_t line_num, const char* consumer, int request_tpye)
{
    this->chipname_ = chipname;
    this->line_num_ = line_num;
    this->config.consumer = consumer;
    this->config.request_type = request_tpye;
    try {
            open();
        }
        catch (std::exception &e ) {
            close();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
}

Gpio::~Gpio()
{
    close();
}

bool Gpio::open()
{
    int ret;
    /*获取GPIO控制器*/
    chip_ = gpiod_chip_open(this->chipname_.c_str());
    if(chip_ == NULL)
    {
        printf("gpiod_chip_open error\n");
        gpiod_chip_close(this->chip_);
        return false;
    }

    /*获取GPIO引脚*/
    line_ = gpiod_chip_get_line(chip_, this->line_num_);
    if(line_ == NULL)
    {
        printf("gpiod_chip_get_line error\n");
        close();
        return false;
    }
    ret = gpiod_line_request(line_, &config, 1);
    if (ret)
    {
        fprintf(stderr, "gpio request error.\n");
        gpiod_line_release(this->line_);
        return false;
    }
    return true;
}

void Gpio::close()
{
    /*释放GPIO引脚*/
    gpiod_line_release(this->line_);
    /*释放GPIO控制器*/
    gpiod_chip_close(this->chip_);

}

void Gpio::write(bool Value)
{
    configure(GPIOD_LINE_REQUEST_DIRECTION_OUTPUT);
    gpiod_line_set_value(this->line_, Value);
    
}

bool Gpio::read()
{
    configure(GPIOD_LINE_REQUEST_DIRECTION_INPUT);
    return gpiod_line_get_value(this->line_);
}

bool Gpio::read_delay(uint16_t delay_time_)
{

    std::this_thread::sleep_for(std::chrono::milliseconds(delay_time_));
    configure(GPIOD_LINE_REQUEST_DIRECTION_INPUT);
    return gpiod_line_get_value(this->line_);
}

int Gpio::event_wait()
{
    return gpiod_line_event_wait(this->line_, NULL);
}

int Gpio::event_read()
{
    configure(GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES);
    if(gpiod_line_event_read(this->line_, &event_) != 0)
    {
        printf("gpiod_chip_get_line error\n");
        return 0;
    }
    else
    {
        return this->event_.event_type;
    }
}


void Gpio::configure(int request_tpye)
{
    int ret;
    if(config.request_type != request_tpye)
    {
        config.request_type = request_tpye;
        ret = gpiod_line_request(line_, &config, 1);
        if(ret < 0)
        {
            printf("gpiod_line_request error\n");
            gpiod_line_release(this->line_);
        }
    }
}




