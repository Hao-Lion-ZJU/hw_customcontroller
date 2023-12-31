/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : fifo.h
 * @brief     : fifo字节环形队列
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-5-30       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */
#ifndef FIFO_H
#define FIFO_H
#include<stdint.h>
#include <mutex>
//环形字符队列
class fifo_single
{
public:
    fifo_single(uint32_t uint_cnt);
    ~fifo_single();

    bool IsEmpty()
    {
        return (this->used_num ? false : true);
    }
    bool IsFull()
    {
        return (this->free_num ? false : true);
    }

    int fifo_s_used() const
    {
        return used_num;
    }

    int fifo_s_free() const
    {
        return free_num;
    }
    
    int fifo_s_put(char element);
    int fifo_s_puts(char* p_source, uint8_t len);


    char fifo_s_get();
    int fifo_s_gets(char *p_dest, uint8_t len);

    void fifo_s_flush();

private:
    std::mutex fifo_mt;      //!< FIFO lock
    char *p_start_addr; //!< FIFO Memory Pool Start Address
    char *p_end_addr;   //!< FIFO Memory Pool End Address
    uint32_t free_num;       //!< The remain capacity of FIFO
    uint32_t used_num;       //!< The number of elements in FIFO
    uint8_t unit_size;      //!< FIFO Element Size(Unit: Byte)
    char *p_read_addr;  //!< FIFO Data Read Index Pointer
    char *p_write_addr; //!< FIFO Data Write Index Pointer
};

#endif /* FIFO_H */