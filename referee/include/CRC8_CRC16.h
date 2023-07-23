
#ifndef CRC8_CRC16_H
#define CRC8_CRC16_H

#include<stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C"
{
#endif
 
/* 函数声明代码块 */
/**
  * @brief          calculate the crc8  
  * @param[in]      pch_message: data
  * @param[in]      dw_length: stream length = data + checksum
  * @param[in]      ucCRC8: init CRC8
  * @retval         calculated crc8
  */
/**
  * @brief          计算CRC8
  * @param[in]      pch_message: 数据
  * @param[in]      dw_length: 数据和校验的长度
  * @param[in]      ucCRC8:初始CRC8
  * @retval         计算完的CRC8
  */
extern uint8_t Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);

/**
  * @brief          CRC8 verify function  
  * @param[in]      pch_message: data
  * @param[in]      dw_length:stream length = data + checksum
  * @retval         true of false
  */
/**
  * @brief          CRC8校验函数
  * @param[in]      pch_message: 数据
  * @param[in]      dw_length: 数据和校验的长度
  * @retval         真或者假
  */
extern uint32_t Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);

/**
  * @brief          append CRC8 to the end of data
  * @param[in]      pch_message: data
  * @param[in]      dw_length:stream length = data + checksum
  * @retval         none
  */
/**
  * @brief          添加CRC8到数据的结尾
  * @param[in]      pch_message: 数据
  * @param[in]      dw_length: 数据和校验的长度
  * @retval         none
  */
extern void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);

/**
  * @brief          calculate the crc16  
  * @param[in]      pch_message: data
  * @param[in]      dw_length: stream length = data + checksum
  * @param[in]      wCRC: init CRC16
  * @retval         calculated crc16
  */
/**
  * @brief          计算CRC16
  * @param[in]      pch_message: 数据
  * @param[in]      dw_length: 数据和校验的长度
  * @param[in]      wCRC:初始CRC16
  * @retval         计算完的CRC16
  */
extern uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);

/**
  * @brief          CRC16 verify function  
  * @param[in]      pch_message: data
  * @param[in]      dw_length:stream length = data + checksum
  * @retval         true of false
  */
/**
  * @brief          CRC16校验函数
  * @param[in]      pch_message: 数据
  * @param[in]      dw_length: 数据和校验的长度
  * @retval         真或者假
  */
extern uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);

/**
  * @brief          append CRC16 to the end of data
  * @param[in]      pch_message: data
  * @param[in]      dw_length:stream length = data + checksum
  * @retval         none
  */
/**
  * @brief          添加CRC16到数据的结尾
  * @param[in]      pch_message: 数据
  * @param[in]      dw_length: 数据和校验的长度
  * @retval         none
  */
extern void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);
 
#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif