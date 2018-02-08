/**************************************************************************************
 *      Copyright:  (C) 2014 wuhanruizhe
 *                  All rights reserved.
 *
 *       Filename:  rfid.c
 *    Description:  This file is mifare.c.
 *                 
 *        Version:  1.0.0(03/02/2014)
 *         Author:  fulinux <fulinux@sina.com>
 *      ChangeLog:  1, Release initial version on "03/02/2014 05:46:18 AM"
 *                 
 *************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>

#include "rc522.h"

/* 
 * Sector password: A, Number of sectors: 16, Each sector
 * password number of bytes: 16Byte 
 */
unsigned char sector_key_a[16][16];

/* IC卡接收数据buf */
unsigned char card_rev_buf[16]  = { 0 };
/* write data buffer*/
unsigned char data_buf[16] = {0x01, 0x02, 0x03, 0x04, 
                              0x05, 0x06, 0x07, 0x08, 
                              0x09, 0x0A, 0x0B, 0x0C, 
                              0x0D, 0x0E, 0x0F, 0x10};


/**************************************************************************************
 *  Description:
 *   Input Args:
 *  Output Args:
 * Return Value:
 *************************************************************************************/
int mi_id_senu (unsigned char *card_rev_buf)
{
    int status          = MI_ERR;

    /* 
     * Looking for a card in the area of the antenna, 
     * that did not enter a dormant state. 
     * return card types, 2 bytes.
     */ 
    while(rfid_request(PICC_REQIDL, &card_rev_buf[0]) != MI_OK)
    {
        usleep(1000);
    }

    /* 
     * anticollision, return the card serialnumber,
     * 4 bytes.
     */
    if(rfid_anticoll(&card_rev_buf[2]) == MI_OK)
    {
        /* 
         * select a card.
         */
        status = rfid_select(&card_rev_buf[2]);
        if(status != MI_ERR)
        {
            printf ("\033[33m\033[5m\n");
            printf ("Choose card completd!\n");
            printf ("Card capacity: %dKbit\n", status);

            /* 
             * print the card id.
             */
            printf("Card ID: 0x%02x, 0x%02x\n", card_rev_buf[0], card_rev_buf[1]);

            printf("Card Number: 0x%02x, 0x%02x, 0x%02x, 0x%02x\n", 
                    card_rev_buf[2], card_rev_buf[3], 
                    card_rev_buf[4], card_rev_buf[5]);
            printf ("\033[0m\n");
        }

        return status;
    }

    return status;

} /* ----- End of mi_id_senu()  ----- */



/**************************************************************************************
 *  Description:
 *   Input Args:
 *  Output Args:
 * Return Value:
 *************************************************************************************/
void block_write (unsigned char block_addr)
{
    int status          = 0;

    /* 将每个扇区的Key A设置为6个0xff */
    memset(sector_key_a, 0xff, 256);

    /* 0扇区块0和控制块不写 */
    if(block_addr == 0 || block_addr % 4 == 3)
    {
        exit(1);
    }

    if(mi_id_senu(card_rev_buf) != MI_ERR)
    {
        /* 测试写IC卡功能 */
        int i   = 0;
        status = rfid_auth_state(PICC_AUTHENT1A, block_addr, 
                sector_key_a[block_addr/4], &card_rev_buf[2]);
        if(status == MI_OK)
        {
            printf ("\033[31m\033[1m\033[4m"
                    "write data to block: %d:              \033[0m\033[31m\033[1m\n", block_addr);
            for(i = 0; i < 16; i++)
            {
                if(0 == i % 4)
                {
                    printf ("\n");
                }
                printf("0x%02x, ", data_buf[i]);
            }
            printf ("\n\n\033[31m\033[1m\033[4m"
                    "                                     ");
            printf ("\033[0m\n");
            status = rfid_write(block_addr, data_buf);
            if(status != MI_OK)
            {
                printf ("rfid write failure!\n");
                return;
            }
            exit(0);
        }

        status = rfid_halt();
        if(status != MI_OK)
        {
            printf ("rfid halt failure!\n");
        }
    }
    return;
} /* ----- End of ctrl()  ----- */


/**************************************************************************************
 *  Description:
 *   Input Args:
 *  Output Args:
 * Return Value:
 *************************************************************************************/
void block_read (unsigned char block_addr)
{
    int status          = 0;

    /* 将每个扇区的Key A设置为6个0xff */
    memset(sector_key_a, 0xff, 256);

    if(mi_id_senu(card_rev_buf) != MI_ERR)
    {
        /* 测试读IC卡的功能 */
        /* 读数据块地址 */
        int i   = 0;

        status = rfid_auth_state(PICC_AUTHENT1A, block_addr, 
                sector_key_a[block_addr/4], &card_rev_buf[2]);
        if(status == MI_OK)
        {
            status = rfid_read(block_addr, data_buf);
            if(status == MI_OK)
            {
                printf ("\033[32m\033[1m\033[4m"
                        "read data from block: %d:             \033[0m\033[32m\033[1m\n", block_addr);
                for(i = 0; i < 16; i++)
                {
                    if(0 == i % 4)
                    {
                        printf ("\n");
                    }
                    printf("0x%02x, ", data_buf[i]);
                }
                printf ("\n\n\033[32m\033[1m\033[4m"
                        "                                     ");
                printf ("\033[0m\n");
            }
        }

        status = rfid_halt();
        if(status != MI_OK)
        {
            printf ("rfid halt failure!\n");
        }
        return;
    }
} /* ----- End of ctrl()  ----- */
