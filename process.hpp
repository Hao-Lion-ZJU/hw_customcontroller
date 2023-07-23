#ifndef __PROCESS_HPP__
#define __PROCESS_HPP__
#pragma once
#include <memory>
#include <iostream>
#include <mutex>

#include "predict.h"
#include "communicator.hpp"
#include "gpio.hpp"

class CustomController{
public:
    /**
     * @brief Construct a new CustomController object
     */
    CustomController();
    /**
     * @brief Destroy the CustomController object
     */
    ~CustomController();
    /**
     * @brief 从数据流读取位姿
     */
    int Poser();
    /**
     * @brief 从裁判系统解析
     */
    void Parser();
    /**
     * @brief 监测GPIO口电平变化
     */
    void Gpiomon();
/**********************data**********************/
private:
    uint8_t flags;

    //T265是否可以开启
    bool gpio_status;
    //T265是否开启
    bool rs_status;
    std::mutex gpio_mtx;
    std::unique_ptr<Communicator> Communicator_Ptr;
    std::unique_ptr<Gpio> Gpio_Ptr;
};


#endif //__PROCESS_HPP__