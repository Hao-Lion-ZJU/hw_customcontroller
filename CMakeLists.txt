cmake_minimum_required(VERSION 3.1.0)
project(test)
# set(CMAKE_BUILD_TYPE "release")

# 设置编译模式
set( CMAKE_BUILD_TYPE "Debug" )
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") 

find_package(realsense2 REQUIRED)
include_directories(  ${realsense2_INCLUDE_DIR} )


# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

include_directories(serial/include )

file(GLOB REFEREE referee/src/*.*)
include_directories(referee/include)

file(GLOB ALOGRITHM alogrithm/src/*.*)
include_directories(alogrithm/include)

file(GLOB GPIO gpio/src/*.*)
include_directories(gpio/include)


add_executable(test main.cpp process.cpp ${REFEREE} ${ALOGRITHM} ${GPIO})
target_link_libraries(test ${realsense2_LIBRARY} )

target_link_libraries(test ${PROJECT_SOURCE_DIR}/serial/lib/arm64/libserial.a)
#gpiod
target_link_libraries(test /usr/lib/aarch64-linux-gnu/libgpiod.a)
