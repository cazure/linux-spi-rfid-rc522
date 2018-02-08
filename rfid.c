/**************************************************************************************
 *      Copyright:  (C) 2014 wuhanruizhe
 *                  All rights reserved.
 *
 *       Filename:  rfid.c
 *    Description:  This file is main file of the program.
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
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spi.h"
#include "rc522.h"
#include "mifare.h"

static unsigned char mode;
static unsigned char addr = 4;
static unsigned char flag = 0;
static unsigned char bits = 8;
static unsigned int speed = 10000;
static const char *device = "/dev/spidev0.0";


#if 1
static void print_usage(const char *prog)
{
    printf("Usage: %s [-drwbsCcH]\n", prog);
    puts(   "  -d --device   device to use (default /dev/spidev0.0)\n"
            "  -r --rmfb     read message from block (e.g: -r 4)\n"
            "  -w --wmtb     write message to block (e.g: -w 4)\n"
            "  -b --bpw      bits per word (default -b 8)\n"
            "  -s --speed    max speed of spi baud rate (Hz) (default -s 10000)\n"
            "  -C --cs-high  chip select active high\n"
            "  -c --cpol     clock polarity\n"
            "  -H --cpha     clock phase\n");
    exit(1);
}



static void parse_opts(int argc, char *argv[])
{
    while (1) {
        const struct option lopts[] = {
            { "device",  1, 0, 'd' },
            { "rdfb",    1, 0, 'r' },
            { "wdtb",    1, 0, 'w' },
            { "bpw",     1, 0, 'b' },
            { "speed",   1, 0, 's' },
            { "cs-high", 0, 0, 'C' },
            { "cpol",    0, 0, 'c' },
            { "cpha",    0, 0, 'H' },
            { NULL, 0, 0, 0 },
        };
        int c;

        c = getopt_long(argc, argv, "d:r:w:b:s:CcH", lopts, NULL);

        if (c == -1)
                break;

        switch (c) {
            case 'd':
                device = optarg;
                break;
            case 'r':
                flag |= MI_READ;
                addr = atoi(optarg);
                break;
            case 'w':
                flag |= MI_WRITE;
                addr = atoi(optarg);
                break;
            case 'b':
                bits = atoi(optarg);
                break;
            case 's':
                speed = atoi(optarg);
                break;
            case 'C':
                mode |= SPI_CS_HIGH;
                break;
            case 'c':
                mode |= SPI_CPOL; 
            case 'H':
                mode |= SPI_CPHA;
                break;
            default:
                print_usage(argv[0]);
                break;
        }
    }
}

#endif

/**************************************************************************************
 *  Description:
 *   Input Args:
 *  Output Args:
 * Return Value:
 *************************************************************************************/
int main (int argc, char **argv)
{
    int fd              = 0;

    parse_opts(argc, argv);

    fd = open(device, O_RDWR);
    if(fd < 0)
    {
        perror("open the device failed\n");
        exit(1);
    }

    /* 初始化spi */
    spi_init(fd, mode, bits, speed);

    /* 初始化RC522 */
    rfid_init(fd);

    while(1)
    {
        if(flag == MI_WRITE)
        {
            block_write(addr);
            exit(0);
        }

        if(flag == MI_READ)
        {
            block_read(addr);
        }
    }

    return 0;
} /* ----- End of main() ----- */
