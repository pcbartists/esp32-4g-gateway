/*
4G Gateway: https://pcbartists.com/product/esp32-4g-lte-gateway-gen1/

Copyright (C) 2021, PCB Artists OPC Pvt Ltd, all right reserved.

Author:     Pratik Panda
E-mail:     hello@pcbartists.com
Website:    pcbartists.com

The code referencing this license is open source software. Redistribution
and use of the code in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
    - Redistribution of original and modified source code must retain the 
    above copyright notice, this condition and the following disclaimer.
    This code (or modifications) in source or binary forms may NOT be used
    in a commercial application without obtaining permission from the Author.
    - This software is provided by the copyright holder and contributors "AS IS"
    and any warranties related to this software are DISCLAIMED. The copyright 
    owner or contributors are NOT LIABLE for any damages caused by use of this 
    software.
*/

#ifndef _PCBARTISTS_GATEWAY_H_
#define _PCBARTISTS_GATEWAY_H_

// I2C config and GPIOs
#define     ESP_I2C_NUM     0
#define     ESP_I2C_SDA     15
#define     ESP_I2C_SCL     13

// Modem UART config and GPIOs
#define MODEM_UART_NUM  UART_NUM_1
#define MODEM_UART_BAUD 115200
#define MODEM_UART_TXD  14
#define MODEM_UART_RXD  33
#define MODEM_UART_RTS  -1
#define MODEM_UART_CTS  -1

#define MODEM_STATUS_IO 36
#define MODEM_RST_IO    4
#define MODEM_PWRKEY_IO 5

// Ethernet config and GPIOs
#define     ETH_CLOCK_POWER_IO      12
#define     ETH_MDC_IO              32
#define     ETH_MDIO_IO             18

#endif
