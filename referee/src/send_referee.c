/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       send_referee.c/h
  * @brief      裁判系统串口发送协议接口
  * @note       
  * @history
  *  Version    Date            Author          Liangliang Hao (郝亮亮)
	* E-mail  1526885155@qq.com
  *  V1.0.0     March-3-2022     ZJU            1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  */
	

#include "send_referee.h"
#include "string.h"
#include "stdio.h"
#include "CRC8_CRC16.h"
#include "referee.h"

	
#define MAX_SIZE          128    //上传数据最大的长度
#define DATA_MAX_SIZE     30   //数据段最大的长度
#define frameheader_len  	5       //帧头长度
#define cmd_len          	2       //命令码长度
#define crc_len          	2       //CRC16校验
#define sof							 	0xA5		//固定起始字符
#define id_len						2				//ID长度
#define data_frameheader_len						6			//数据段帧头长度

uint8_t tx_buff[MAX_SIZE];			//串口发送包
//数据帧固定格式   SOF+data_len+seq+CRC8     cmd_id		data_id+send_id+recive_id+data		CRC16
uint8_t seq=0;									//帧数计数



ext_custom_controller_data_t custom_controller_send_data;
		
	
/**
  * @brief     发送内容打包
  * @param[in] cmd_id:  命令内容ID
  * @param[in] *p_data: 数据段
  * @param[in] len:     数据段长度
  */
	

static void referee_data_pack(uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	unsigned char i=i;

	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //数据帧长度	

	memset(tx_buff,0,frame_length);  //存储数据的数组清零
	
	/*****帧头打包*****/
	tx_buff[0] = sof;//数据帧起始字节
	memcpy(&tx_buff[1],(uint8_t*)&len,2);//数据帧中data的长度
	tx_buff[3] = seq;//包序号
	Append_CRC8_Check_Sum(tx_buff,frameheader_len);  //帧头校验CRC8

	/*****命令码打包*****/
	memcpy(&tx_buff[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
  // tx_buff[4] = cmd_id >> 8;
  // tx_buff[5] = (cmd_id & 0xff);

	
	/*****数据打包*****/
	memcpy(&tx_buff[frameheader_len+cmd_len], p_data, len);
	Append_CRC16_Check_Sum(tx_buff,frame_length);  //一帧数据校验CRC16
	/***更新包数序列号***/
	if (seq == 0xff) seq=0;
  else seq++;
}


/**
  * @brief     初始化结构体数据
  * @param[in] none

  */
	
void init_send_struct_data(void)
{
    //memset(&student_interactive_data, 0, sizeof(ext_student_interactive_data_t));
    memset(&custom_controller_send_data, 0, sizeof(ext_custom_controller_data_t));
}

/**
  * @brief     发送字符串数据,默认线宽为5，字体大小为20,字符图层为4
  * @param[in] buff 返回的串口buffer
  * @param[in] pos_x   x方向位置
  * @param[in] pos_y   y方向位置
  * @param[in] pos_z   z方向位置
  * @param[in] roll    roll角度
  * @param[in] pitch   pitch角度
  * @param[in] yaw     yaw角度
  * @retval    发送数据长度
  */
uint16_t referee_send_pose(uint8_t* buff, float pos_x, float pos_y, float pos_z, float roll, float pitch, float yaw)
{
	
	uint8_t data_buff[DATA_MAX_SIZE];

	custom_controller_send_data.position_x = pos_x;
	custom_controller_send_data.position_y = pos_y;
	custom_controller_send_data.position_z = pos_z;
	custom_controller_send_data.roll = roll;
	custom_controller_send_data.pitch = pitch;
	custom_controller_send_data.yaw = yaw;
  memset(custom_controller_send_data.empty, 0xaa, sizeof(custom_controller_send_data.empty));

	/*****数据段填充*****/
	memcpy(&data_buff[0],(uint8_t*)&custom_controller_send_data,sizeof(ext_custom_controller_data_t));
	
	/*****发送内容打包*****/
	referee_data_pack(CUSTOM_CONTROLLER_DATA_CMD_ID,data_buff,sizeof(ext_custom_controller_data_t));

	uint16_t frame_length = frameheader_len + cmd_len + sizeof(ext_custom_controller_data_t) + crc_len;   //数据帧长度	

	memcpy(&buff[0],tx_buff, frame_length);

	memset(tx_buff,0,MAX_SIZE);

  return frame_length;
}



