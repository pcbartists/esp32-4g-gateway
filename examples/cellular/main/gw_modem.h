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

#ifndef PCBARTISTS_GATEWAY_MODEM_GEN1_H_
#define PCBARTISTS_GATEWAY_MODEM_GEN1_H_

#define MODEM_CONNECT_BIT BIT0
#define MODEM_GOT_DATA_BIT BIT2

EventGroupHandle_t pcbartists_modem_eventgroup (void);
void pcbartists_modem_setup (void);
void pcbartists_modem_deinit (void);
void pcbartists_modem_power_up_por (void);
void pcbartists_gateway_modem_disable (void);

#endif
