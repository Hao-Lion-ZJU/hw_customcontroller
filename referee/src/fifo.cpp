/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : fifo.cpp
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

#include"fifo.h"
#include<assert.h>
#include<string.h>

fifo_single::fifo_single(uint32_t uint_cnt)
{
    
    //! Check input parameters.
    assert(uint_cnt > 0);

    //! Allocate Memory for pointer of new FIFO
    this->p_start_addr = new char[uint_cnt];
    //! Allocate Failure, exit now
    assert(p_start_addr!=nullptr);
    //! Initialize FIFO Control Block.
    this->p_end_addr = (char *)p_start_addr + uint_cnt - 1;
    this->free_num = uint_cnt;
    this->used_num = 0;
    this->p_read_addr = (char *)p_start_addr;
    this->p_write_addr = (char *)p_start_addr;
    this->unit_size = 1;
}

fifo_single::~fifo_single()
{
    assert(p_start_addr!=nullptr);
    delete[] p_start_addr;
    p_start_addr = nullptr;
}



char fifo_single::fifo_s_get()
{
    char retval = 0;
    std::lock_guard<std::mutex> guard(fifo_mt);
    if(IsEmpty())
        retval = NULL;
    if (p_read_addr > p_end_addr)
        p_read_addr = p_start_addr;
    retval = *this->p_read_addr;
    // Update information
    this->p_read_addr++;
    this->free_num++;
    this->used_num--;
    return (retval);
}

int fifo_single::fifo_s_gets(char *p_dest, uint8_t len)
{
    int retval;
    int len_to_end;
    int len_from_start;
    std::lock_guard<std::mutex> guard(fifo_mt);
    if (NULL == p_dest || 0 == this->used_num)
    {
        return (-1);
    }

    if (this->p_read_addr > this->p_end_addr)
        this->p_read_addr = this->p_start_addr;

    len = (len < this->used_num) ? len : this->used_num;
    len_to_end = this->p_end_addr - this->p_read_addr + 1;

    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_dest, this->p_read_addr, len_to_end);
        this->p_read_addr += len_to_end;
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_dest, p_read_addr, len_to_end);
        memcpy(p_dest + len_to_end, this->p_start_addr, len_from_start);
        this->p_read_addr = this->p_start_addr + len_from_start;
    }

    this->free_num += len;
    this->used_num -= len;

    retval = len;
    return retval;
}

int fifo_single::fifo_s_put(char element)
{
    if (0 == this->free_num)
    {
        //! Error, FIFO is full!
        return (-1);
    }
    std::lock_guard<std::mutex> guard(fifo_mt);
    if (this->p_write_addr > this->p_end_addr)
    this->p_write_addr = this->p_start_addr;
    *(this->p_write_addr) = element;
    this->p_write_addr++;
    this->free_num--;
    this->used_num++;
    return (0);
}


int fifo_single::fifo_s_puts(char* p_source, uint8_t len)
{
    int retval;
    int len_to_end;
    int len_from_start;

    if (NULL == p_source || 0 == this->free_num)
    {
        return (-1);
    }

    std::lock_guard<std::mutex> guard(fifo_mt);

    if (this->p_write_addr > this->p_end_addr)
        this->p_write_addr = this->p_start_addr;

    len = (len < this->free_num) ? len : this->free_num;
    len_to_end = this->p_end_addr - this->p_write_addr + 1;
    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(this->p_write_addr, p_source, len_to_end);
        this->p_write_addr += len_to_end;
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(this->p_write_addr, p_source, len_to_end);
        memcpy(this->p_start_addr, p_source + len_to_end, len_from_start);
        this->p_write_addr = this->p_start_addr + len_from_start;
    }

    this->free_num -= len;
    this->used_num += len;
    retval = len;

    return retval;
}

void fifo_single::fifo_s_flush()
{
    std::lock_guard<std::mutex> guard(fifo_mt);
    this->free_num = this->p_end_addr - this->p_start_addr + 1;
    this->used_num = 0;
    this->p_read_addr = this->p_start_addr;
    this->p_write_addr = this->p_start_addr;
}