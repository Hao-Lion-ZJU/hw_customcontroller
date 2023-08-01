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