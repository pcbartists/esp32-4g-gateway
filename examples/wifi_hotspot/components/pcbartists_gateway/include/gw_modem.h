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

#ifndef _GW_MODEM_H_
#define _GW_MODEM_H_

#define MODEM_CONNECT_BIT BIT0
#define MODEM_GOT_DATA_BIT BIT2

// Get event group handle for modem handler
EventGroupHandle_t pcbartists_modem_eventgroup (void);

// Get DCE pointer for the modem, gw_modem.c declares and manages its own static
// DCE structure that can be accessed using this function
esp_modem_dce_t *pcbartists_get_modem_dce (void);

// Get netif pointer for modem interface
// Mainted inside gw_modem.c as a static structure
esp_netif_t *pcbartists_get_modem_netif (void);

// Initializes the modem into PPP data mode after connecting to the internet
void pcbartists_modem_setup (void);

// De-inits DCE and netif structures for modem
void pcbartists_modem_deinit (void);

// Power-up reset function for modem
void pcbartists_modem_power_up_por (void);

// Power off the modem using GPIO controls
void pcbartists_gateway_modem_disable (void);

#endif
