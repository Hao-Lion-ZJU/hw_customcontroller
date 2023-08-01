/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       send_referee.c/h
  * @brief      ����ϵͳ���ڷ���Э��ӿ�
  * @note       
  * @history
  *  Version    Date            Author          Liangliang Hao (������)
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

	
#define MAX_SIZE          128    //�ϴ��������ĳ���
#define DATA_MAX_SIZE     30   //���ݶ����ĳ���
#define frameheader_len  	5       //֡ͷ����
#define cmd_len          	2       //�����볤��
#define crc_len          	2       //CRC16У��
#define sof							 	0xA5		//�̶���ʼ�ַ�
#define id_len						2				//ID����
#define data_frameheader_len						6			//���ݶ�֡ͷ����

uint8_t tx_buff[MAX_SIZE];			//���ڷ��Ͱ�
//����֡�̶���ʽ   SOF+data_len+seq+CRC8     cmd_id		data_id+send_id+recive_id+data		CRC16
uint8_t seq=0;									//֡������



ext_custom_controller_data_t custom_controller_send_data;
		
	
/**
  * @brief     �������ݴ��
  * @param[in] cmd_id:  ��������ID
  * @param[in] *p_data: ���ݶ�
  * @param[in] len:     ���ݶγ���
  */
	

static void referee_data_pack(uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
	unsigned char i=i;

	uint16_t frame_length = frameheader_len + cmd_len + len + crc_len;   //����֡����	

	memset(tx_buff,0,frame_length);  //�洢���ݵ���������
	
	/*****֡ͷ���*****/
	tx_buff[0] = sof;//����֡��ʼ�ֽ�
	memcpy(&tx_buff[1],(uint8_t*)&len,2);//����֡��data�ĳ���
	tx_buff[3] = seq;//�����
	Append_CRC8_Check_Sum(tx_buff,frameheader_len);  //֡ͷУ��CRC8

	/*****��������*****/
	memcpy(&tx_buff[frameheader_len],(uint8_t*)&cmd_id, cmd_len);
  // tx_buff[4] = cmd_id >> 8;
  // tx_buff[5] = (cmd_id & 0xff);

	
	/*****���ݴ��*****/
	memcpy(&tx_buff[frameheader_len+cmd_len], p_data, len);
	Append_CRC16_Check_Sum(tx_buff,frame_length);  //һ֡����У��CRC16
	/***���°������к�***/
	if (seq == 0xff) seq=0;
  else seq++;
}


/**
  * @brief     ��ʼ���ṹ������
  * @param[in] none

  */
	
void init_send_struct_data(void)
{
    //memset(&student_interactive_data, 0, sizeof(ext_student_interactive_data_t));
    memset(&custom_controller_send_data, 0, sizeof(ext_custom_controller_data_t));
}

/**
  * @brief     �����ַ�������,Ĭ���߿�Ϊ5�������СΪ20,�ַ�ͼ��Ϊ4
  * @param[in] buff ���صĴ���buffer
  * @param[in] pos_x   x����λ��
  * @param[in] pos_y   y����λ��
  * @param[in] pos_z   z����λ��
  * @param[in] roll    roll�Ƕ�
  * @param[in] pitch   pitch�Ƕ�
  * @param[in] yaw     yaw�Ƕ�
  * @retval    �������ݳ���
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

	/*****���ݶ����*****/
	memcpy(&data_buff[0],(uint8_t*)&custom_controller_send_data,sizeof(ext_custom_controller_data_t));
	
	/*****�������ݴ��*****/
	referee_data_pack(CUSTOM_CONTROLLER_DATA_CMD_ID,data_buff,sizeof(ext_custom_controller_data_t));

	uint16_t frame_length = frameheader_len + cmd_len + sizeof(ext_custom_controller_data_t) + crc_len;   //����֡����	

	memcpy(&buff[0],tx_buff, frame_length);

	memset(tx_buff,0,MAX_SIZE);

  return frame_length;
}



