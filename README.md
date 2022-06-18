
For Olimex A20-Micro olinuxino Debian 8
1、概要

这个工程是开源的，希望对大家的使用有帮助;

2、目录架构
.
|-- LICENSE
|-- Makefile
|-- mifare
|   |-- Makefile
|   |-- mifare.c
|   `-- mifare.h
|-- rc522
|   |-- Makefile
|   |-- rc522.c
|   `-- rc522.h
|-- README.md
|-- rfid.c
`-- spi
    |-- makefile
        |-- spi.c
         -- spi.h
        |-- spidev.h

3、使用方法:

在linux主机上:

$make

在开发板中:

$mount -o nolock 192.168.1.2:/mnt/nfs /mnt

$cd /mnt

                                    
$./rfid -w block_address

e.g:

$./rfid -d /dev/spidev1.0 -w 4 FF0AFF

spi mode: 0
bits per word: 8
max speed: 10000 Hz (10 KHz)
                                         


    Choose card completd!
    Card capacity: 8Kbit
    Card ID: 0x04, 0x00
    Card Number: 0xb5, 0x13, 0xca, 0xc5

    write data to block: 4:              

    0x01, 0x02, 0x03, 0x04, 
    0x05, 0x06, 0x07, 0x08, 
    0x09, 0x0a, 0x0b, 0x0c, 
    0x0d, 0x0e, 0x0f, 0x10, 

$./rfid -r block_address

e.g:

$./rfid -d /dev/spidev1.0 -r 4

spi mode: 0
bits per word: 8
max speed: 10000 Hz (10 KHz)
                                         

    Choose card completd!
    Card capacity: 8Kbit
    Card ID: 0x04, 0x00
    Card Number: 0xb5, 0x13, 0xca, 0xc5

    read data from block: 4:             

    0x01, 0x02, 0x03, 0x04, 
    0x05, 0x06, 0x07, 0x08, 
    0x09, 0x0a, 0x0b, 0x0c, 
    0x0d, 0x0e, 0x0f, 0x10, 

$./rfid --id

e.g:

$./rfid -d /dev/spidev1.0 --id

4、联系方式

email: fulinux@sina.com

blog: http://blog.csdn.net/fulinus
