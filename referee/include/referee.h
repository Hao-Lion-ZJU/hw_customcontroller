#ifndef REFEREE_H
#define REFEREE_H

#include "protocol.h"


#ifdef __cplusplus
extern "C"
{

#endif

#pragma pack(push)              //保存对齐状态 
#pragma pack(1)

//自定义控制器数据交互
typedef struct //0x0302
{
    float position_x;
    float position_y;
    float position_z;
    float w;
    float x;
    float y;
    float z;
    int16_t empty;
} ext_custom_controller_data_t;

#pragma pack(pop)


extern void get_custom_controller_data(float* pos_x, float* pos_y, float* pos_z, float* roll, float* pitch, float* yaw);

extern void referee_data_solve(uint8_t *frame);

#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif

