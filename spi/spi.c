/**************************************************************************************
 *      Copyright:  (C) 2014 wuhanruizhe
 *                  All rights reserved.
 *
 *       Filename:  spi.c
 *    Description:  This file initial the spi. like mode, speed and so on. 
 *                 
 *        Version:  1.0.0(03/02/2014)
 *         Author:  fulinux <fulinux@sina.com>
 *      ChangeLog:  1, Release initial version on "03/02/2014 06:36:28 AM"
 *                 
 *************************************************************************************/
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spi.h"



/**************************************************************************************
 *  Description:
 *   Input Args:
 *  Output Args:
 * Return Value:
 *************************************************************************************/
void spi_init (int fd, unsigned char mode, unsigned char bits, unsigned int speed)
{
    int ret;

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
    {
       printf("can't set spi mode\n");
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
    {
        printf("can't get spi mode\n");
    }

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        printf("can't set bits per word\n");
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        printf("can't get bits per word\n");
    }

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        printf("can't set max speed hz\n");
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        printf("can't get max speed hz\n");
    }

    printf ("\033[36m\033[4m"
            "                                     \n");
    printf ("\033[0m\033[36m\n");
    printf("spi mode: %d\n", mode);
    printf("bits per word: %d\n", bits);
    printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
    printf ("\033[0m\033[36m\033[4m"
            "                                     \n");
    printf ("\033[0m\n");

} /* ----- End of spi_init()  ----- */

