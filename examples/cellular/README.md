| Supported Products | Gateway Gen.1 | Gateway ULP | Controller Gen.1 |
| ----------------- | ------------- | ----------- | ---------------- |

# HTTPS GET Example
This example demonstrates how to make an HTTPS GET request using the 4G modem for connecting to the internet. 

## How to Use Example
- Make sure that you are using IDF v.5.0.1
- Set target to ESP32 using `idf.py set-target esp32`
- Using `idf.py menuconfig`, set example parameters to match the Gateway product and modem that you are using
- Flash the build to your product by `idf.py build flash -b 921600 flash monitor -p COMx`

## Hardware Required
* Any PCB Artists Gateway or Controller listed in Supported Products above.
* An ESP32 programmer connected to the Gateway via USB.
* The board should be powered up using an external power source as outlined in the product's hardware guide

## Example Output
If you have configured the example correctly, you should see something like this in the terminal logs:
```
W (435) gw_modem.c: Turning off modem...
I (20935) gw_modem.c: Turning on modem
I (46435) gw_modem.c: Modem is powered up and ready
I (46435) gw_modem.c: Initializing esp_modem for the SIM7600 module...
I (46435) uart: queue free spaces: 30
I (46455) gw_modem.c: Signal quality: rssi=25, ber=99
I (46925) modem_https_example: Waiting for IP address
I (53025) gw_modem.c: Modem Connect to PPP Server
I (53025) gw_modem.c: ~~~~~~~~~~~~~~
I (53025) gw_modem.c: IP          : 100.73.44.148
I (53025) esp-netif_lwip-ppp: Connected
I (53025) gw_modem.c: Netmask     : 255.255.255.255
I (53035) gw_modem.c: Gateway     : 10.64.64.64
I (53045) gw_modem.c: Name Server1: 117.96.0.32
I (53045) gw_modem.c: Name Server2: 117.96.0.8
I (53055) gw_modem.c: ~~~~~~~~~~~~~~
I (53055) gw_modem.c: GOT ip event!!!
I (53065) gw_modem.c: PPP state changed event 0
I (60075) modem_https_example: Connection established...
I (60075) modem_https_example: 70 bytes written
I (60075) modem_https_example: esp_tls_conn_write returned 70
I (60075) modem_https_example: Reading HTTP response...
I (65475) modem_https_example: connection closed

I (65475) modem_https_example: Server responded with 516 bytes: 
HTTP/1.1 200 OK
Date: Wed, 22 Feb 2023 07:05:39 GMT
Server: nginx/1.21.6
Content-Type: text/plain
Content-Length: 71
Content-Security-Policy: upgrade-insecure-requests;
Last-Modified: Sat, 29 Oct 2022 04:51:58 GMT
Cache-Control: max-age=86400
Expires: Wed, 22 Feb 2023 13:39:43 GMT
Vary: Accept-Encoding,User-Agent
X-Endurance-Cache-Level: 2
X-nginx-cache: WordPress
X-Server-Cache: true
X-Proxy-Cache: HIT
Accept-Ranges: bytes

This is a test file on pcbartists.com.
ABCD
1234
!@#$
-- End of File --
I (3671155) esp-netif_lwip-ppp: Connection lost
I (3671155) gw_modem.c: Modem Disconnect from PPP Server
I (3785145) gw_modem.c: Modem Disconnect from PPP Server
```
## More Information
See [our website](https://www.pcbartists.com) for more information about our ESP32 4G Gateway and other IoT products.
