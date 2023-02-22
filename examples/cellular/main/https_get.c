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

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_netif.h"
#include "esp_netif_ppp.h"
#include "esp_modem_api.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_event.h"

#include "gw_modem.h"
#include "esp_tls.h"

static const char *TAG = "modem_https_example";

// Buffer to store outgoing and incoming data
static char tlsbuf [2048];

// Returns the number of bytes sent out
// or the error number (negative)
static int tls_send (esp_tls_t *tls, const void  *data, size_t datalen)
{
    size_t written_bytes = 0;
    do {
        int ret = esp_tls_conn_write(tls, data + written_bytes, datalen - written_bytes);
        if (ret >= 0) {
            ESP_LOGI(TAG, "%d bytes written", ret);
            written_bytes += ret;
        } else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write  returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
            return ret;
        }
    } while (written_bytes < datalen);

    return written_bytes;
}

// Pass buffer and buffer size (max)
// Returns bytes received
static int tls_get (esp_tls_t *tls, const void  *data, size_t datalen)
{
    int ret;
    size_t read_bytes = 0;
    do
    {
        ret = esp_tls_conn_read(tls, data + read_bytes, datalen - read_bytes);

        if (ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ) {
            continue;
        } else if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_read  returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
            break;
        } else if (ret == 0) {
            ESP_LOGI(TAG, "connection closed");
            break;
        }

        read_bytes += ret;
        ESP_LOGD(TAG, "%d bytes read", ret);
    } while (1);

    return read_bytes;
}

void app_main(void)
{
    pcbartists_modem_power_up_por ();

    /* Init and register system/core components */
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Set up modem netif and basic event handlers
    pcbartists_modem_setup ();
    
    /* Wait for IP address */
    ESP_LOGI(TAG, "Waiting for IP address");
    xEventGroupWaitBits (pcbartists_modem_eventgroup(), MODEM_CONNECT_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    // Make an HTTPS GET request
    esp_tls_t *tls = esp_tls_init();

    // Set interface here
    esp_tls_cfg_t cfg;
    bzero (&cfg, sizeof (esp_tls_cfg_t));
    do 
    {
        if (esp_tls_conn_http_new_async("https://pcbartists.com/test.txt", &cfg, tls) == 1)
        {
            ESP_LOGI(TAG, "Connection established...");
            break;
        }
        else
        {
            ESP_LOGE(TAG, "Connection failed... retrying in 2 seconds");
            vTaskDelay (5000/portTICK_PERIOD_MS);
        }
    } while (1);

    bzero (tlsbuf, sizeof (tlsbuf));
    strcpy (tlsbuf, "GET https://pcbartists.com/test.txt HTTP/1.1\r\n");
    strcat (tlsbuf, "Host: pcbartists.com\r\n");
    strcat (tlsbuf, "\r\n");
    int tlssize = tls_send (tls, tlsbuf, strlen (tlsbuf));
    ESP_LOGI (TAG, "esp_tls_conn_write returned %d", tlssize);

    ESP_LOGI(TAG, "Reading HTTP response...");
    memset(tlsbuf, 0x00, sizeof(tlsbuf));
    tlssize = tls_get (tls, tlsbuf, sizeof (tlsbuf));
    ESP_LOGI (TAG, "Server responded with %d bytes: \n%s", tlssize, tlsbuf);

    // De-init modem resources
    pcbartists_modem_deinit ();
}
