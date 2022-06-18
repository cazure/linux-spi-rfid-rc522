/**************************************************************************************
 *      Copyright:  (C) 2014 wuhanruizhe
 *                  All rights reserved.
 *
 *       Filename:  rfid.c
 *    Description:  This file is main file of the program.
 *                 
 *        Version:  1.0.0(03/02/2014)
 *         Author:  fulinux <fulinux@sina.com> / побългарено от Konstantin Kolev konstantinkolev@abv.bg
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

#include "spi/spi.h"
#include "rc522/rc522.h"
#include "mifare/mifare.h"

static unsigned char mode;
static unsigned char addr = 4;
static unsigned char flag = 0;
static unsigned char bits = 8;
static unsigned int speed = 10000;
static const char *device = "/dev/spidev1.0";

unsigned char card_rev_buf[16]  = { 0 };
    /* 
    * Sector password: A, Number of sectors: 16, Each sector
    * password number of bytes: 16Byte 
    */
    unsigned char sector_key_a[16][16];
    unsigned char data_buf[16] = 
    {0x01, 0x02, 0x03, 0x04, 
     0x05, 0x06, 0x07, 0x08, 
     0x09, 0x0A, 0x0B, 0x0C, 
     0x0D, 0x0E, 0x0F, 0x10};


#if 1
static void print_usage(const char *prog)
{
    printf("Usage: %s [-dirwka]\n", prog);
    puts(   "  -d	--device	device to use (default /dev/spidev0.0)\n"
    	    "  -i	--id		read Id\n"
            "  -r	--read		read message from block (e.g: -r 4)\n"
            "  -w	--write		write message to block (e.g: -w 4 xx)\n"
            "  -k	--key		set key (e.g.: -k \"00 00 00 00\")\n"
	    "  -a	--data		set data to write (e.g.: -a \"00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10\")\n"
            );
    exit(1);
}

void print_buff(unsigned char* data_buf, int len) {    
    int i = 0;
    for(i = 0; i < len; i++){
	printf("%02x", data_buf[i]);
    }
    printf("\n");
}

void analyze(unsigned char * buf, int len, char * data) {
    char* ptrData ;
    char* endPtr ;
    int itteration = 0;
    ptrData = data;
    while (ptrData) {
	if (itteration < len) {
    	    buf[itteration++]  = (unsigned char)strtoul(ptrData, &endPtr, 16);
	}
	if (*endPtr) ptrData = endPtr; else ptrData = NULL;
    }
}

static void parse_opts(int argc, char *argv[])
{
    while (1) {
        const struct option lopts[] = {
	    
    	    {"id",      0, 0, 'i'},
    	    {"device",  1, 0, 'd'},
            {"read",    1, 0, 'r'},
            {"write",   1, 0, 'w'},
            {"key",     1, 0, 'k'},
	    {"data",   	1, 0, 'a'},
	    {0, 0, 0, 0}
        };
        int c;

        c = getopt_long(argc, argv, "id:r:w:k:a:", lopts, NULL);

        if (c == -1)
                break;
	switch (c) {
            case 'd':
                device = optarg;
                break;
            case 'i':
		flag |= MI_GET_ID;
		break;
            case 'r':
                flag |= MI_READ;
                addr = atoi(optarg);
                break;
            case 'w':
                flag |= MI_WRITE;
                addr = atoi(optarg);
                break;
            case 'k':
		//key = ...
		break;
	    case 'a':
		memset(data_buf, 0x00, sizeof data_buf);
		analyze(data_buf, 16, optarg);
		print_buff(data_buf, 16);
                break;	    
            default:
		printf("[%c]??\n", c);
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
int main (int argc, char **argv) {
    memset(data_buf, 0x00, sizeof data_buf);
    //print_buff(data_buf, 16);
    int status		= MI_ERR;
    int numAtempt	= 1;
    int fd   = 0;
    parse_opts(argc, argv);
    if (flag == 0) {
    	print_usage(argv[0]);
    	exit(1);
    }
    fd = open(device, O_RDWR);
    if(fd < 0)
    {
        perror("open the device failed\n");
        exit(1);
    }
    spi_init(fd, mode, bits, speed);
  
	rfid_init(fd); 

    /*
     * Looking for a card in the area of the antenna,.
     * that did not enter a dormant state..
     * return card types, 2 bytes.
     */
    while(rfid_request(PICC_REQIDL, &card_rev_buf[0]) != MI_OK && numAtempt-- >= 0) {
        usleep(500);
    }
    /*
     * anticollision, return the card serialnumber,
     * 4 bytes.
     */
    if(rfid_anticoll(&card_rev_buf[2]) == MI_OK) {
	
	status = rfid_select(&card_rev_buf[2]);
	if(status != MI_ERR) {
	    if(flag == MI_GET_ID) {
		/*
		 * print the card id.
		*/
		//printf("Card ID: 0x%02x, 0x%02x\n", card_rev_buf[0], card_rev_buf[1]);
		printf("%02x%02x%02x%02x\n", card_rev_buf[2], card_rev_buf[3],	card_rev_buf[4], card_rev_buf[5]);
	    } else if (flag == MI_READ) {
		memset(sector_key_a, 0xff, 256);
		memset(data_buf, 0x00, sizeof data_buf);
		status = rfid_auth_state(PICC_AUTHENT1A, addr, sector_key_a[addr/4], &card_rev_buf[2]);
    		if(status == MI_OK) {
        	    status = rfid_read(addr, data_buf);
        	    if(status == MI_OK) {
            		print_buff(data_buf, 16);
			/*for(i = 0; i < 16; i++){
                	    printf("%02x ", data_buf[i]);
            		}
            		printf("\n");
			*/
        	    }
    		} else {
				printf("Error reading");
				close(fd);
				exit(1);
			}
	    } else if (flag == MI_WRITE) {
		memset(sector_key_a, 0xff, 256);
	        if(addr == 0 || addr % 4 == 3) {
				close(fd);
    		    exit(1);
		}
		status = rfid_auth_state(PICC_AUTHENT1A, addr,  sector_key_a[addr/4], &card_rev_buf[2]);
    		if(status == MI_OK) {
		    status = rfid_write(addr, data_buf);
	       	    if(status != MI_OK) {
            		printf("rfid write failure!\n");
					close(fd);
            		exit(1);
        	    }    
		} else {
		    printf("Error writing");
			close(fd);
		    exit(1);
		}
	    } else {
		printf("Not implemented\n");
	    }
	    status = rfid_halt();
        if(status != MI_OK)  {
			//printf ("rfid halt failure! [ERROR %d]\n", status);
    	}
	} else {
	    printf("None\n");
	}
    } else {
		printf("None\n");
    }
	close(fd);
    return 0;
} /* ----- End of main() ----- */
