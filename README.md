# Artisanal

This is the main branch for Artisanal Project. For the Artisan project, there is an ESP32-S3-SIM7670G-4G. The device will capture images of the pyrolysis process, retrieve GPS data and battery percentage, and send this information to a doctype (Frappe ERP NEXT FRAMEWORK) in the dMRV system via the cellular 4G network.

Header - URL : https://dev-artisan.8848digitalerp.com/api/method/artisan.api.image_log.create_imglog
         Authorization : Token b441d57636c7de9:38bcfa86df28dfc
         Content-Type : application/json
         Body : 
                  {
                    "payload": {
                      "image": "base 64  encoded image string",
                      "metadata": {
                        "T1": "25",
                        "T2": "50",
                        "T3": "75",
                        "T4": "100",
                        "BAT": "85",
                        "GPS": "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"
                      }
                    }
                  }
Database - User ID : administrator
           Password : Admin@01
           Site Home : https://dev-artisan.8848digitalerp.com/app/artisan-pro-dashboard
           Site Map : Home - Build - DocType - Image Log - Image Log List
Documentation - https://www.waveshare.com/wiki/ESP32-S3-SIM7670G-4G#Install_Espressif_IDF_Plug-in


# ESP32-S3-SIM7670G-4G Development Board Overview

The ESP32-S3-SIM7670G-4G is a high-performance microcontroller development board designed by Waveshare. It integrates a wide range of features and peripherals, making it an ideal platform for various IoT (Internet of Things) applications. Key use cases include mobile device connectivity, outdoor monitoring, smart home solutions, and more.

The board features an ESP32-S3R2 SoC with WiFi and Bluetooth functionality, and the SIM7670G 4G communication module, which adds mobile network connectivity. In addition to this, the board is equipped with a camera interface, a TF card slot, RGB LEDs, a 18650 battery holder, and a wide range of peripherals and expansion options.

## Core Components and Features

### 1. ESP32-S3R2 Chip

Processor: Xtensa® 32-bit LX7 dual-core processor, capable of running at a clock speed of 240 MHz.
Memory:
SRAM: 512KB
ROM: 384KB
PSRAM: 2MB
Flash: 16MB external Flash memory.
Wireless Connectivity:
Wi-Fi: Integrated 2.4GHz Wi-Fi (802.11 b/g/n).
Bluetooth: Bluetooth LE 5.0 with superior RF performance.
Hardware Security:
Encryption Accelerator.
Random Number Generator (RNG).
HMAC (Hash-based Message Authentication Code).
Digital Signature Module for secure IoT communication and data encryption.

### 2. SIM7670G Cellular Module

#### 4G Cat-1 Communication:
Support for LTE-FDD: Bands B1, B2, B3, B4, B5, B7, B8, B12, B13, B18, B19, B20, B25, B26, B28, and B66.
Support for LTE-TDD: Bands B34, B38, B39, B40, and B41.
SIM7670G AT Commands for Cellular and Network Control
The SIM7670G module is controlled using AT commands over UART or USB. These commands control everything from network initialization to data transmission and GPS functionality.

SIM7670G AT Commands for 4G Networking
AT+CSQ: Check signal quality.
AT+CREG?: Check network registration status.
AT+CGATT?: Query GPRS attach status.
AT+CGDCONT=1,"IP","<APN>": Set the Access Point Name (APN) for the cellular connection.
ATD991#**: Establish PPP dial-up connection.
AT+CGACT=1,1: Activate the PDP context.

SIM7670G AT Commands for HTTP/HTTPS Control
AT+HTTPINIT: Initialize HTTP session.
AT+HTTPPARA="CID",<context ID>: Set HTTP context.
AT+HTTPPARA="URL","<URL>": Set the URL for the HTTP/HTTPS request.
AT+HTTPACTION=<method>: Execute the HTTP/HTTPS action (0 for GET, 1 for POST).
AT+HTTPREAD: Read the HTTP response.
AT+HTTPTERM: Terminate HTTP session.

#### GNSS (Global Navigation Satellite System): Supports GPS, BeiDou, GLONASS, and Galileo for high-precision navigation and positioning.
SIM7670G AT Commands for GNSS (GPS) Control
AT+CGNSSPWR=1: Turn GNSS power on.
AT+CGNSSPWR=0: Turn GNSS power off.
AT+CGNSSTST=1: Start GNSS data output.
AT+CGNSSPORTSWITCH=0,1
<parse_data_port> 0 output theparseddataofNMEAtoUSBATport, 1 output theparseddataofNMEAtoUARTport.
<nmea_data_port> 0 outputrawNMEAdatatoUSBNMEAport , 1 outputrawNMEAdatatoUARTport.

### 3. Camera Interface
24-PIN Interface compatible with camera modules such as OV2640 and OV5640.
Supported Applications: Image recognition, facial recognition, and other machine vision applications using the ESP32 Camera framework.
Camera Interface Pin Assignments
The development board includes a 24-pin camera interface for connecting camera modules like the OV2640 or OV5640.

PWDN_GPIO_NUM: -1 (Power down GPIO, typically not connected)
RESET_GPIO_NUM: -1 (Reset GPIO, typically not connected)
XCLK_GPIO_NUM: 34 (External clock for the camera)
SIOD_GPIO_NUM: 15 (Serial Data line for I2C communication)
SIOC_GPIO_NUM: 16 (Serial Clock line for I2C communication)
Y9_GPIO_NUM: 14 (Data output Y9 from camera)
Y8_GPIO_NUM: 13 (Data output Y8 from camera)
Y7_GPIO_NUM: 12 (Data output Y7 from camera)
Y6_GPIO_NUM: 11 (Data output Y6 from camera)
Y5_GPIO_NUM: 10 (Data output Y5 from camera)
Y4_GPIO_NUM: 9 (Data output Y4 from camera)
Y3_GPIO_NUM: 8 (Data output Y3 from camera)
Y2_GPIO_NUM: 7 (Data output Y2 from camera)
VSYNC_GPIO_NUM: 36 (Vertical synchronization signal from the camera)
HREF_GPIO_NUM: 35 (Horizontal synchronization signal from the camera)
PCLK_GPIO_NUM: 37 (Pixel clock signal from the camera)

### 4. TF Card Slot
A TF card slot is onboard, enabling memory expansion for data storage.
The board includes an SD/MMC interface for connecting a TF card (SD card). The pin assignments are as follows:

SDMMC_CLK (Clock): GPIO 5
SDMMC_CMD (Command): GPIO 4
SDMMC_DATA (Data lines): GPIO 6 (single line for data)
SD_CD_PIN (Card detection): GPIO 46

### 5. 18650 Battery Holder
Battery Holder: Supports a 3.7V 18650 lithium battery (battery not included).
Battery Management: Includes battery power measurement using the MAX17048G IC, which monitors the real-time battery capacity.
Protection Circuits: Built-in anti-reverse battery protection ensures safety in deployment.

### 6. Charging Interfaces
USB and Solar Panel Charging:
Supports charging through a USB Type-C port or solar panel input.
Onboard CN3791 Solar Charging IC allows real-time charging and power management, designed for use with 5V, 6V, 9V, 12V, and 18V solar panels.
Power input voltage can be adjusted for different solar panel configurations.

### 7. Power Supply Options
Multiple Power Sources:
Powered via USB Type-C, 18650 battery holder, or through the VBAT pin header for external 3.7V lithium battery input.
DC-DC converters and power management ICs, such as ETA6098 and SY8105IADC, provide multiple voltage rails and power management options.
Power Indicators:
Battery reverse warning indicator: Alerts if the battery is inserted incorrectly.
Solar power input indicator: Displays the status of solar power input.
Power Switching:
Onboard DIP switches allow toggling the power supply to individual circuits (e.g., camera, SIM7670G module, USB HUB driver).

### 8. GNSS and Antennas
GNSS Antenna: Comes with a high-precision ceramic antenna for GNSS positioning.
LTE, Wi-Fi, and GNSS Antenna Connectors: Three IPEX connectors (1 for LTE, 1 for Wi-Fi, 1 for GNSS) are provided. By default, the board uses onboard ceramic antennas, but external antennas can be added by modifying solder pads.

### 9. DIP Switch Controls
CAM Switch: Toggles the power to the camera module.
HUB Switch: Controls the USB hub power supply.
4G Switch: Powers on/off the SIM7670G cellular module.
USB Switch: Switches the USB interface between the SIM7670G module and ESP32-S3 for debugging or data transmission.
Battery On/Off Switch: Manages the power supply from the battery.

### 10. USB Interfaces and Peripherals
USB Type-C Port: Used for power, debugging, and connecting peripherals.
USB Switching IC (FSUSB42UMX): Facilitates switching between different USB channels to avoid conflicts between the ESP32-S3 and SIM7670G.
USB Hub (CH334) and UART to USB IC (CH343): Allow the ESP32-S3 and SIM7670G to communicate over USB for debugging and dial-up internet access.

### 11. LEDs and Indicators
WS2812B RGB LED: A programmable RGB LED for user-defined visual indications or status feedback. The WS2812B RGB LED on the board can be controlled through a single GPIO pin 38
Power Indicators: Show real-time power status, including USB power, solar charging, battery level, and network activity.
Network Status Indicator: LED indicating the status of the 4G network connection.

### 12. GNSS Functionality
GNSS Power Control: GNSS functionality can be activated by the AT command AT+CGNSSPWR=1.
GNSS Data Output: The GNSS module outputs data using AT+CGNSSTST=1, providing precise location data for IoT applications.

### 13. SIM7670G Communication
4G Dial-up and Network Functions:
The board supports PPP (Point-to-Point Protocol) for internet access via the SIM7670G module.
AT Commands: Used for controlling HTTP and HTTPS connections. Example AT commands include AT+HTTPINIT, AT+HTTPPARA, and AT+HTTPACTION.

### 14. Solar Panel Input Interface
Voltage Configuration: The board supports input from solar panels with various voltages, including 5V, 6V, 9V, 12V, and 18V. The default configuration is for 5V.
Power Management: Efficient power management circuitry ensures real-time charging, protection, and status feedback when powered by solar energy.

### 15. On Board Communication 

USB Type-C Port:
DN, DP: These are the data lines from the USB Type-C port, which connect to the inputs of the CH334F USB Hub IC (DU N, DU P).
CH334F USB Hub IC:
Input (DU N, DU P): Connected to the USB Type-C port's data lines.
Output:
USB3 (p, n): Connected to the FSUSB42 IC.
USB4 (p, n): Connected to the CH343 IC, which converts USB signals to UART.
CH343 UART to USB IC:
Outputs (CH TXD, CH RXD): These lines are connected to the U0TXD and U0RXD pins of the ESP32-S3, allowing UART communication with the ESP32-S3 over the USB4 channel from the hub.
FSUSB42 USB Switch IC:
Inputs:
USB SET: Controlled by a DIP switch (HUB).
USB2: Connected to GPIO19 and GPIO20 of the ESP32-S3, providing control over USB switching.
USB: Connected to the SIM7670 module for USB communication.
USB3: Connected to the USB3 output from the CH334F USB hub, allowing the switch to control whether the USB hub or SIM7670 has access to the USB lines.
SIM7670 4G Module:
UART1:
The TXD1 line of the SIM7670 is routed through a TXB010PWR voltage level translator and connected to GPIO17 and GPIO18 of the ESP32-S3 for UART1 communication.
USB Interface: Routed through the FSUSB42 switch and CH334F hub.
GNSS (GPS): Uses UART3 of the SIM7670 to communicate with the system.
Power Management:
MAX17048G: Battery management IC, which monitors the battery's status.
SDA (GPIO3) and SCL (GPIO2) are connected to the I²C bus for communication.
BAT SET DIP Switch: Controls power to the SIM7670.
The switch routes power through a control circuit that inputs VBAT and connects PWRKEY (GPIO33), allowing control over the power state of the SIM7670 module.
Summary of Switches:
USB SET DIP Switch (HUB): Controls whether the USB connection is routed to the SIM7670 or the ESP32-S3.
BAT SET DIP Switch (4G): Manages power routing to the SIM7670 via VBAT and the PWRKEY.

