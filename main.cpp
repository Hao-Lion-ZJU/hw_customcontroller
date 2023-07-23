// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Intel Corporation. All Rights Reserved.



#include <thread>
#include <unistd.h>


#include "process.hpp"




int main(int argc, char * argv[])
{
    CustomController ctrl;
    
    std::thread Pose_thread(&CustomController::Poser, &ctrl);

    std::thread Mon_thread(&CustomController::Gpiomon, &ctrl);


    Pose_thread.join();
    Mon_thread.join();
    return 0;

}

// int main(int argc, char * argv[]) try
// {
//   	//声明一个realsense传感器设备
//     rs2::pipeline pipe;
//     // 创建一个配置信息
//     rs2::config cfg;
//     //告诉配置信息，我需要传感器的POSE和6DOF IMU数据
//     cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);

    
//     std::mutex mutex;
//     //回调函数
//     auto callback = [&](const rs2::frame& frame)
//     {
//         std::lock_guard<std::mutex> lock(mutex);
//         if (rs2::pose_frame fp = frame.as<rs2::pose_frame>()) {
//             rs2_pose pose_data = fp.get_pose_data();
//             auto now = std::chrono::system_clock::now().time_since_epoch();
//             double now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
//             double pose_time_ms = fp.get_timestamp();
//             float dt_s = static_cast<float>(std::max(0., (now_ms - pose_time_ms)/1000.));
//             rs2_pose predicted_pose = predict_pose(pose_data, dt_s);
//             std::cout << "Predicted " << std::fixed << std::setprecision(3) << dt_s*1000 << "ms " <<
//                     "Confidence: " << pose_data.tracker_confidence << " T: " <<
//                     predicted_pose.translation.x << " " <<
//                     predicted_pose.translation.y << " " <<
//                     predicted_pose.translation.z << " (meters)   \r" << std::endl;
//         }
//     };

//     //开始接收数据，接收数据之后进入回调函数进行处理
//     rs2::pipeline_profile profiles = pipe.start(cfg, callback);
//     std::cout << "started thread\n";
//     while(true) {
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//         std::cout << "runing " << std::endl;
//     }

//     return EXIT_SUCCESS;
// }
// catch (const rs2::error & e)
// {
//     std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
//     return EXIT_FAILURE;
// }
// catch (const std::exception& e)
// {
//     std::cerr << e.what() << std::endl;
//     return EXIT_FAILURE;
// }