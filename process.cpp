/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : process.cpp
 * @brief     : 线程函数
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

#include "process.hpp"

#include <mutex>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>

#include <Eigen/Dense>

/**
 * @brief Construct a new CustomController object
 */
CustomController::CustomController()
{
    Communicator_Ptr = std::make_unique<Communicator>(true);
    Gpio_Ptr = std::make_unique<Gpio>("/dev/gpiochip0", 2);
    gpio_status = false;
    rs_status = false;
}

CustomController::~CustomController()
{
    Communicator_Ptr.release();
    Gpio_Ptr.release();
    
}

/**
 * @brief 从数据流读取位姿
 */
int CustomController::Poser()
{
    while (true)
    {
        try
        {
            //声明一个realsense传感器设备
            rs2::pipeline pipe;
            // 创建一个配置信息
            rs2::config cfg;
            //告诉配置信息，我需要传感器的POSE和6DOF IMU数据
            cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);

            SerialPkg pkg = 
            {
                .pos_x = 0.0,
                .pos_y = 0.0,
                .pos_z = 0.0,
            };

            
            std::mutex mutex;
            //回调函数
            auto callback = [&](const rs2::frame& frame)
            {
                std::lock_guard<std::mutex> lock(mutex);
                if (rs2::pose_frame fp = frame.as<rs2::pose_frame>()) {
                    rs2_pose pose_data = fp.get_pose_data();
                    auto now = std::chrono::system_clock::now().time_since_epoch();
                    double now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
                    double pose_time_ms = fp.get_timestamp();
                    float dt_s = static_cast<float>(std::max(0., (now_ms - pose_time_ms)/1000.));
                    rs2_pose predicted_pose = predict_pose(pose_data, dt_s);
                    Eigen::Quaterniond quaternion(predicted_pose.rotation.w,predicted_pose.rotation.x,predicted_pose.rotation.y,predicted_pose.rotation.z);
                    Eigen::Vector3d eulerAngle=quaternion.matrix().eulerAngles(0,1,2);
                    std::cout << 
                            "x: " << predicted_pose.translation.x << " " <<
                            "y: " << predicted_pose.translation.y << " " <<
                            "z: " << predicted_pose.translation.z << " (meters)   \t" <<
                            "yaw: " << eulerAngle(2) * 180/3.14  <<
                            "pitch: " << eulerAngle(1) * 180/3.14 <<
                            "roll: " << eulerAngle(0) * 180/3.14 << std::endl;
                    pkg.pos_x = predicted_pose.translation.x; pkg.pos_y = predicted_pose.translation.y; pkg.pos_z = predicted_pose.translation.z;
                    pkg.roll = eulerAngle(0); pkg.pitch = eulerAngle(1); pkg.yaw = eulerAngle(2);
                }
            };

            

            while(true) {
                if(gpio_status && !rs_status)
                {
                    gpio_mtx.lock();
                    //开始接收数据，接收数据之后进入回调函数进行处理
                    rs2::pipeline_profile profiles = pipe.start(cfg, callback);
                    rs_status = true;
                    std::cout << "started out\n";
                    gpio_mtx.unlock();

                }
                else if(!gpio_status && rs_status)
                {
                    gpio_mtx.lock();
                    //停止数据接收
                    pipe.stop();
                    rs_status = false;
                    std::cout << "stopped out\n";
                    gpio_mtx.unlock();
                }
                Communicator_Ptr->send(pkg);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            return EXIT_SUCCESS;
        }
        catch (const rs2::error & e)
        {
            std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
            rs_status = false;
            continue;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            rs_status = false;
            continue;
        }
        std::cout << "T265 connecting...!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    
}


void CustomController::Gpiomon()
{
    //配置GPIO引脚
    Gpio_Ptr->configure(GPIOD_LINE_REQUEST_DIRECTION_INPUT);
    bool value;
    value = Gpio_Ptr->read();
    while (true)
    {
        value = Gpio_Ptr->read();

        //延时消抖读取
        value = Gpio_Ptr->read_delay(20);
        if(value)  //按键松开
        {
            gpio_mtx.lock();
            this->gpio_status = false;      //停止T265数据流输出
            gpio_mtx.unlock();
        }
        else
        {
            gpio_mtx.lock();
            this->gpio_status = true;      //开启T265数据流输出
            gpio_mtx.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
    }
    
}

// void CustomController::Gpiomon()
// {
//     //配置GPIO引脚
//     Gpio_Ptr->configure(GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES);
//     while (true)
//     {
//         //等待触发回调
//         Gpio_Ptr->event_wait();
//         int event_type = Gpio_Ptr->event_read();
//         if(!event_type)
//         {
//             continue;
//         }
//         else if (event_type == GPIOD_LINE_EVENT_RISING_EDGE)        //按键松开
//         {
//             //延时消抖读取
//             bool value = Gpio_Ptr->read_delay(20);
//             if(value != 1)
//                 continue;
//             gpio_mtx.lock();
//             this->gpio_status = false;      //停止T265数据流输出
//             gpio_mtx.unlock();
//         }
//         else if (event_type == GPIOD_LINE_EVENT_FALLING_EDGE)       //按键按下
//         {
//             //延时消抖读取
//             bool value = Gpio_Ptr->read_delay(20);
//             if(value != 1)
//                 continue;
//             gpio_mtx.lock();
//             this->gpio_status = true;      //开启T265数据流输出
//             gpio_mtx.unlock();
//         }
//     }
    
// }
