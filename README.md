# hw_customcontroller
浙江大学RoboMaster2023赛季工程自定义控制器代码

## 前言
`树莓派由于是arm架构，所以使用realsense驱动库需要从源码构建` [2.53版本源码链接](https://github.com/IntelRealSense/librealsense/tree/R/253)
由于英特尔已经停产T265，所以必须下载2.53版本以前的驱动，否则会找不到相机

需要安装的库
包     | 版本
-------- | -----
gpiod  | lastest
Eigen3  | lastest
realsense  | <=2.53

配置好环境后clone代码，输入以下命令
```bash
mkdir build 
cmake ..
make -j4 #树莓派最高四核
```