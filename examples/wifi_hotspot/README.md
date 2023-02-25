| Supported Products | Gateway Gen.1 | Gateway ULP | Controller Gen.1 |
| ----------------- | ------------- | ----------- | ---------------- |

# ESP32 4G Hotspot Example
This example demonstrates how to use the PCB Artists 4G Gateway as a Wi-Fi hotspot.
A hotspot nables sharing of 4G cellular data with multiple sensor nodes within Wi-Fi range.

## How to Use Example
- Make sure that you are using IDF v.5.0.1
- Set target to ESP32 using `idf.py set-target esp32`
- Using `idf.py menuconfig`, set parameters to match the Gateway product and modem that you are using
- Flash the build to your product by `idf.py build flash -b 921600 flash monitor -p COMx`
- Once the Gateway connects to the internet over 4G, it will create a Wi-Fi access point. This may take a minute.
- Connect to AP to use the internet!

## Hardware Required
* Any PCB Artists Gateway or Controller listed in Supported Products above.
* An ESP32 programmer connected to the Gateway via USB.
* The board should be powered up using an external power source as outlined in the product's hardware guide.

## Example Output
If you have configured the example correctly, you should see something like this in the terminal logs:
```
W (573) gw_modem.c: Turning off modem...
I (21073) gw_modem.c: Turning on modem
I (46573) gw_modem.c: Modem is powered up and ready
I (46573) gw_modem.c: Initializing esp_modem for a generic module...
I (46573) uart: queue free spaces: 30
I (46593) gw_modem.c: Signal quality: rssi=24, ber=99
I (47053) hotspot: Waiting for IP address
I (53153) gw_modem.c: Modem Connect to PPP Server
I (53153) gw_modem.c: ~~~~~~~~~~~~~~
I (53153) gw_modem.c: IP          : 100.68.243.209
I (53153) esp-netif_lwip-ppp: Connected
I (53153) gw_modem.c: Netmask     : 255.255.255.255
I (53163) gw_modem.c: Gateway     : 10.64.64.64
I (53173) gw_modem.c: Name Server1: 117.96.122.32
I (53173) gw_modem.c: Name Server2: 117.96.122.8
I (53183) gw_modem.c: ~~~~~~~~~~~~~~
I (53183) gw_modem.c: GOT ip event!!!
I (53193) gw_modem.c: PPP state changed event 0
I (53203) wifi:wifi driver task: 3ffc6df8, prio:23, stack:6656, core=0
I (53203) system_api: Base MAC address is not set
I (53203) system_api: read default base MAC address from EFUSE
I (53223) wifi:wifi firmware version: 17afb16
I (53223) wifi:wifi certification version: v7.0
I (53223) wifi:config NVS flash: enabled
I (53223) wifi:config nano formating: disabled
I (53233) wifi:Init data frame dynamic rx buffer num: 32
I (53233) wifi:Init management frame dynamic rx buffer num: 32
I (53243) wifi:Init management short buffer num: 32
I (53243) wifi:Init dynamic tx buffer num: 32
I (53253) wifi:Init static rx buffer size: 1600
I (53253) wifi:Init static rx buffer num: 10
I (53253) wifi:Init dynamic rx buffer num: 32
I (53263) wifi_init: rx ba win: 6
I (53263) wifi_init: tcpip mbox: 32
I (53273) wifi_init: udp mbox: 6
I (53273) wifi_init: tcp mbox: 6
I (53273) wifi_init: tcp tx win: 5744
I (53283) wifi_init: tcp rx win: 5744
I (53283) wifi_init: tcp mss: 1440
I (53293) wifi_init: WiFi IRAM OP enabled
I (53293) wifi_init: WiFi RX IRAM OP enabled
I (53303) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (53413) wifi:mode : softAP (10:97:bd:13:13:61)
I (53413) wifi:Total power save buffer number: 16
I (53413) wifi:Init max length of beacon: 752/752
I (53413) wifi:Init max length of beacon: 752/752
I (53423) hotspot: wifi_init_softap finished. SSID:pcbartists password:password channel:1
W (53423) hotspot: Hotspot should now be functional...
```
## More Information
See [our website](https://www.pcbartists.com) for more information about our ESP32 4G Gateway and other IoT products.
