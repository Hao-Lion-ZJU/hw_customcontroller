#include "referee.h"
#include "string.h"
#include "stdio.h"
#include "CRC8_CRC16.h"
#include "protocol.h"


frame_header_struct_t referee_receive_header;
frame_header_struct_t referee_send_header;


ext_custom_controller_data_t custom_controller_data;




void init_referee_struct_data(void)
{

    memset(&custom_controller_data, 0, sizeof(ext_custom_controller_data_t));

}

void referee_data_solve(uint8_t *frame)
{
    uint16_t cmd_id = 0;

    uint8_t index = 0;

    memcpy(&referee_receive_header, frame, sizeof(frame_header_struct_t));

    index += sizeof(frame_header_struct_t);

    memcpy(&cmd_id, frame + index, sizeof(uint16_t));
    index += sizeof(uint16_t);

    switch (cmd_id)
    {
        case CUSTOM_CONTROLLER_DATA_CMD_ID:
        {
            memcpy(&custom_controller_data, frame + index, sizeof(ext_custom_controller_data_t));
        }
        default:
        {
            break;
        }
    }
}



void get_custom_controller_data(float* pos_x, float* pos_y, float* pos_z, float* roll, float* pitch, float* yaw)
{
    *pos_x = custom_controller_data.position_x;
    *pos_y = custom_controller_data.position_y;
    *pos_z = custom_controller_data.position_z;
}