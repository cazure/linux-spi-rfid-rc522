/********************************************************************************
 *      Copyright:  (C) 2014 wuhanruizhe
 *                  All rights reserved.
 *
 *       Filename:  mifare.h
 *    Description:  This head file is mifare.h
 *
 *        Version:  1.0.0(03/11/2014)
 *         Author:  fulinux <fulinux@sina.com>
 *      ChangeLog:  1, Release initial version on "03/11/2014 05:51:46 PM"
 *                 
 ********************************************************************************/

#ifndef __MIFARE_H__
#define __MIFARE_H__

#define MI_READ 0x01
#define MI_WRITE 0x02


void block_write (unsigned char block_addr);
void block_read (unsigned char block_addr);


#endif
