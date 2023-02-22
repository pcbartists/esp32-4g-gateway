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
#include "esp_event.h"
#include "driver/gpio.h"

#include "gw_config.h"
#include "gw_modem.h"

#if defined(CONFIG_EXAMPLE_FLOW_CONTROL_NONE)
#define EXAMPLE_FLOW_CONTROL ESP_MODEM_FLOW_CONTROL_NONE
#elif defined(CONFIG_EXAMPLE_FLOW_CONTROL_SW)
#define EXAMPLE_FLOW_CONTROL ESP_MODEM_FLOW_CONTROL_SW
#elif defined(CONFIG_EXAMPLE_FLOW_CONTROL_HW)
#define EXAMPLE_FLOW_CONTROL ESP_MODEM_FLOW_CONTROL_HW
#endif

static const char *TAG = "gw_modem.c";

static EventGroupHandle_t event_group = NULL;
static const int USB_DISCONNECTED_BIT = BIT3; // Used only with USB DTE but we define it unconditionally, to avoid too many #ifdefs in the code

#define CHECK_USB_DISCONNECTION(event_group)

static void on_ppp_changed(void *arg, esp_event_base_t event_base,
                           int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "PPP state changed event %d", (int)event_id);
    if (event_id == NETIF_PPP_ERRORUSER) {
        /* User interrupted event from esp-netif */
        esp_netif_t *netif = event_data;
        ESP_LOGI(TAG, "User interrupted event from netif:%p", netif);
    }
}

static void on_ip_event(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "IP event! %d", (int)event_id);
    if (event_id == IP_EVENT_PPP_GOT_IP) {
        esp_netif_dns_info_t dns_info;

        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        esp_netif_t *netif = event->esp_netif;

        ESP_LOGI(TAG, "Modem Connect to PPP Server");
        ESP_LOGI(TAG, "~~~~~~~~~~~~~~");
        ESP_LOGI(TAG, "IP          : " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(TAG, "Netmask     : " IPSTR, IP2STR(&event->ip_info.netmask));
        ESP_LOGI(TAG, "Gateway     : " IPSTR, IP2STR(&event->ip_info.gw));
        esp_netif_get_dns_info(netif, 0, &dns_info);
        ESP_LOGI(TAG, "Name Server1: " IPSTR, IP2STR(&dns_info.ip.u_addr.ip4));
        esp_netif_get_dns_info(netif, 1, &dns_info);
        ESP_LOGI(TAG, "Name Server2: " IPSTR, IP2STR(&dns_info.ip.u_addr.ip4));
        ESP_LOGI(TAG, "~~~~~~~~~~~~~~");
        xEventGroupSetBits(event_group, MODEM_CONNECT_BIT);

        ESP_LOGI(TAG, "GOT ip event!!!");
    } else if (event_id == IP_EVENT_PPP_LOST_IP) {
        ESP_LOGI(TAG, "Modem Disconnect from PPP Server");
    } else if (event_id == IP_EVENT_GOT_IP6) {
        ESP_LOGI(TAG, "GOT IPv6 event!");

        ip_event_got_ip6_t *event = (ip_event_got_ip6_t *)event_data;
        ESP_LOGI(TAG, "Got IPv6 address " IPV6STR, IPV62STR(event->ip6_info.ip));
    }
}

EventGroupHandle_t pcbartists_modem_eventgroup (void)
{
    return event_group;
}

static esp_modem_dce_t *dce;
static esp_netif_t *esp_netif;
void pcbartists_modem_setup (void)
{
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &on_ip_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(NETIF_PPP_STATUS, ESP_EVENT_ANY_ID, &on_ppp_changed, NULL));

    /* Configure the PPP netif */
    esp_modem_dce_config_t dce_config = ESP_MODEM_DCE_DEFAULT_CONFIG(CONFIG_EXAMPLE_MODEM_PPP_APN);
    esp_netif_config_t netif_ppp_config = ESP_NETIF_DEFAULT_PPP();
    esp_netif = esp_netif_new(&netif_ppp_config);
    assert(esp_netif);

    event_group = xEventGroupCreate();

    /* Configure the DTE */
    esp_modem_dte_config_t dte_config = ESP_MODEM_DTE_DEFAULT_CONFIG();
    /* setup UART specific configuration based on kconfig options */
    dte_config.uart_config.tx_io_num = CONFIG_EXAMPLE_MODEM_UART_TX_PIN;
    dte_config.uart_config.rx_io_num = CONFIG_EXAMPLE_MODEM_UART_RX_PIN;
    dte_config.uart_config.rts_io_num = CONFIG_EXAMPLE_MODEM_UART_RTS_PIN;
    dte_config.uart_config.cts_io_num = CONFIG_EXAMPLE_MODEM_UART_CTS_PIN;
    dte_config.uart_config.flow_control = EXAMPLE_FLOW_CONTROL;
    dte_config.uart_config.rx_buffer_size = CONFIG_EXAMPLE_MODEM_UART_RX_BUFFER_SIZE;
    dte_config.uart_config.tx_buffer_size = CONFIG_EXAMPLE_MODEM_UART_TX_BUFFER_SIZE;
    dte_config.uart_config.event_queue_size = CONFIG_EXAMPLE_MODEM_UART_EVENT_QUEUE_SIZE;
    dte_config.task_stack_size = CONFIG_EXAMPLE_MODEM_UART_EVENT_TASK_STACK_SIZE;
    dte_config.task_priority = CONFIG_EXAMPLE_MODEM_UART_EVENT_TASK_PRIORITY;
    dte_config.dte_buffer_size = CONFIG_EXAMPLE_MODEM_UART_RX_BUFFER_SIZE / 2;

#if CONFIG_EXAMPLE_MODEM_DEVICE_SIM7000 == 1
    ESP_LOGI(TAG, "Initializing esp_modem for the SIM7000 module...");
    //esp_modem_dce_t *dce = esp_modem_new_dev(ESP_MODEM_DCE_SIM7000, &dte_config, &dce_config, esp_netif);
    dce = esp_modem_new_dev(ESP_MODEM_DCE_SIM7000, &dte_config, &dce_config, esp_netif);
#elif CONFIG_EXAMPLE_MODEM_DEVICE_SIM7070 == 1
    ESP_LOGI(TAG, "Initializing esp_modem for the SIM7070 module...");
    dce = esp_modem_new_dev(ESP_MODEM_DCE_SIM7070, &dte_config, &dce_config, esp_netif);
#elif CONFIG_EXAMPLE_MODEM_DEVICE_SIM7600 == 1
    ESP_LOGI(TAG, "Initializing esp_modem for the SIM7600 module...");
    dce = esp_modem_new_dev(ESP_MODEM_DCE_SIM7600, &dte_config, &dce_config, esp_netif);
#else
    ESP_LOGI(TAG, "Initializing esp_modem for a generic module...");
    dce = esp_modem_new(&dte_config, &dce_config, esp_netif);
#endif
    assert(dce);

    xEventGroupClearBits(event_group, MODEM_CONNECT_BIT | MODEM_GOT_DATA_BIT | USB_DISCONNECTED_BIT);

    /* Run the modem demo app */
#if CONFIG_EXAMPLE_NEED_SIM_PIN == 1
    // check if PIN needed
    bool pin_ok = false;
    if (esp_modem_read_pin(dce, &pin_ok) == ESP_OK && pin_ok == false) {
        if (esp_modem_set_pin(dce, CONFIG_EXAMPLE_SIM_PIN) == ESP_OK) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        } else {
            abort();
        }
    }
#endif

    int rssi, ber;
    esp_err_t err = esp_modem_get_signal_quality(dce, &rssi, &ber);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_modem_get_signal_quality failed with %d %s", err, esp_err_to_name(err));
        return;
    }
    ESP_LOGI(TAG, "Signal quality: rssi=%d, ber=%d", rssi, ber);

    err = esp_modem_set_mode(dce, ESP_MODEM_MODE_DATA);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_modem_set_mode(ESP_MODEM_MODE_DATA) failed with %d", err);
        return;
    }
}

void pcbartists_modem_deinit (void)
{
    esp_err_t err = esp_modem_set_mode(dce, ESP_MODEM_MODE_COMMAND);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_modem_set_mode(ESP_MODEM_MODE_COMMAND) failed with %d", err);
        return;
    }

#if 0    
    char imsi[32];
    err = esp_modem_get_imsi(dce, imsi);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_modem_get_imsi failed with %d", err);
        return;
    }
    ESP_LOGI(TAG, "IMSI=%s", imsi);
#endif

    // UART DTE clean-up
    esp_modem_destroy(dce);
    esp_netif_destroy(esp_netif);
}

static void pcbartists_modem_gpio_init (void)
{
    gpio_reset_pin(MODEM_PWRKEY_IO);
    gpio_reset_pin(MODEM_RST_IO);
    gpio_reset_pin(MODEM_STATUS_IO);

    gpio_set_direction(MODEM_PWRKEY_IO, GPIO_MODE_OUTPUT);
    gpio_set_direction(MODEM_RST_IO, GPIO_MODE_OUTPUT);
    gpio_set_direction(MODEM_STATUS_IO, GPIO_MODE_INPUT);

    // Disable reset pin and pwrkey
    gpio_set_level(MODEM_RST_IO, 0);
    gpio_set_level(MODEM_PWRKEY_IO, 0);
}

// Power up the 4G LTE modem with Power-on-reset
void pcbartists_modem_power_up_por (void)
{
    pcbartists_modem_gpio_init ();

    bool modem_on = false;

    if (gpio_get_level (MODEM_STATUS_IO) == 1)
        modem_on = true;

    if (modem_on)
    {
        // Turn off modem
        ESP_LOGW (TAG, "Turning off modem...");
        gpio_set_level (MODEM_PWRKEY_IO, 1);        // high
        vTaskDelay (500/portTICK_PERIOD_MS);
        gpio_set_level (MODEM_PWRKEY_IO, 0);        // low
        vTaskDelay (500/portTICK_PERIOD_MS);
        gpio_set_level (MODEM_PWRKEY_IO, 1);        // high
        vTaskDelay (500/portTICK_PERIOD_MS);

        // wait for status to go low
        while (1)
        {
            if (gpio_get_level (MODEM_STATUS_IO) == 1)
                vTaskDelay (1000/portTICK_PERIOD_MS);
            else
                break;
        }
    }

    // Turn on modem
    ESP_LOGI (TAG, "Turning on modem");
    gpio_set_level (MODEM_PWRKEY_IO, 1);        // high
    vTaskDelay (500/portTICK_PERIOD_MS);
    gpio_set_level (MODEM_PWRKEY_IO, 0);        // low
    vTaskDelay (500/portTICK_PERIOD_MS);
    gpio_set_level (MODEM_PWRKEY_IO, 1);        // high
    vTaskDelay (500/portTICK_PERIOD_MS);

    // Wait for status to go high
    while (1)
    {
        if (gpio_get_level (MODEM_STATUS_IO) == 0)
            vTaskDelay (1000/portTICK_PERIOD_MS);
        else
            break;
    }
    
    vTaskDelay (10000/portTICK_PERIOD_MS);
    ESP_LOGI (TAG, "Modem is powered up and ready");
    // Set up UART here or later
}

void pcbartists_gateway_modem_disable (void)
{
    pcbartists_modem_gpio_init ();

    gpio_set_level (MODEM_RST_IO, 0);
    gpio_set_level (MODEM_PWRKEY_IO, 0);
}
