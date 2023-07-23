#ifndef SEND_REFEREE_H
#define SEND_REFEREE_H


#include "referee.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void init_send_struct_data(void);
extern uint16_t referee_send_pose(uint8_t* buff, float pos_x, float pos_y, float pos_z, float w, float x, float y, float z);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif
//-----------------------------------------------------END OF FILE--------------------------------------------------------------------------------
