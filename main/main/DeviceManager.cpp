#include "DeviceManager.h"
#include "esp_camera.h"
#include <Wire.h>
#include "AT_Commands.h"
#include "ATCommandHandler.h"
#include "DeviceManager.h"

#define MAX17048_I2C_ADDRESS 0x36

// RGB LED Setup
Adafruit_NeoPixel rgbLED = Adafruit_NeoPixel(1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

// Initialize the MAX17048 battery monitor
MAX17048 pwr_mgmt;

static const int RXPin = 17;  // RX Pin for Serial1 (to TX of the other device)
static const int TXPin = 18;  // TX Pin for Serial1 (to RX of the other device)
static const uint32_t SERIAL1_BAUD = 115200;  // Baud rate for Serial1
static const uint32_t SERIAL_BAUD = 115200;   // Baud rate for Serial (for debugging)

// Initialize Serial communication for debugging and Serial1 for external communication
void initSerial() {
    // Initialize default Serial (USB) for debugging
    Serial.begin(SERIAL_BAUD);
    delay(500);  // Allow some time for Serial to start

    // Initialize Serial1 for communication with external device
    Serial1.begin(SERIAL1_BAUD, SERIAL_8N1, RXPin, TXPin);
    Serial.println("Serial and Serial1 initialized");
}

// Initialize the Camera
void initCamera() {
    camera_config_t config;
    
    // Setup camera pin configuration
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    // Set XCLK frequency for camera
    config.xclk_freq_hz = 20000000;  // 20 MHz clock frequency

    // Set the pixel format to JPEG for high-quality image capture
    config.pixel_format = PIXFORMAT_JPEG;  // for JPEG image capture (not streaming)

    // Configure the frame size for high resolution (UXGA 1600x1200 is a good high-quality option)
    config.frame_size = FRAMESIZE_UXGA;  // UXGA (1600x1200) for high-quality capture

    // Use PSRAM for frame buffer if available for more buffer space
    config.fb_location = CAMERA_FB_IN_PSRAM;

    // Set JPEG quality to high (lower values produce higher-quality images)
    config.jpeg_quality = 10;  // Adjusted for higher quality image (lower value = better quality)

    // Set grab mode to capture the latest frame when triggered
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;  // Ensures it captures the latest available frame

    // Use 1 frame buffer as this is a one-time capture scenario
    config.fb_count = 1;

    // Check if PSRAM is available
    if (psramFound()) {
        config.jpeg_quality = 10;  // Use a high JPEG quality for better images
        config.fb_count = 1;  // Single frame buffer for high-quality single image capture
        config.grab_mode = CAMERA_GRAB_LATEST;  // Capture latest frame
    } else {
        // If PSRAM is not available, lower the frame size to save DRAM memory
        config.frame_size = FRAMESIZE_SVGA;  // Limit frame size to SVGA (800x600) to fit DRAM
        config.fb_location = CAMERA_FB_IN_DRAM;
    }

    // Initialize the camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    Serial.println("Camera initialized successfully");
    
}

// Initialize RGB LED
void initRGBLED() {
    rgbLED.begin();
    rgbLED.show();
    Serial.println("RGB LED initialized");
}

// Initialize SD card
void initSDCard() {
    if (!SD.begin(SDMMC_CLK)) {
        Serial.println("Card Mount Failed");
        return;
    }
    Serial.println("SD card initialized");
}

// Initialize Wi-Fi in AP mode
void initWiFiAP() {
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);
    Serial.println("WiFi AP Started");
}

// Initialize Wi-Fi in STA mode
void initWiFiSTA() {
    WiFi.begin(WIFI_STA_SSID, WIFI_STA_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi Connected");
}

void initBatteryManager() {
    Wire.begin(3, 2); // Initialize I2C with specified SDA and SCL pins
}

float readBatteryLevel() {
    Wire.beginTransmission(MAX17048_I2C_ADDRESS);
    Wire.write(0x02);
    Wire.endTransmission();

    Wire.requestFrom(MAX17048_I2C_ADDRESS, 2);
    uint16_t soc = (Wire.read() << 8) | Wire.read();

    if (soc > 65535) {
        soc = 65535;
    }

    float batteryLevel = (float)soc / 65535.0 * 100; // Percentage of battery level
    return batteryLevel;
}

// Write data to SD card
void writeSDCard(const char* fileName, const char* data) {
    File file = SD.open(fileName, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    file.print(data);
    file.close();
    Serial.println("File written successfully");
}

// Read data from SD card
String readSDCard(const char* fileName) {
    File file = SD.open(fileName);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return String();
    }
    String data;
    while (file.available()) {
        data += (char)file.read();
    }
    file.close();
    return data;
}

// Capture image and save to SD card
void captureImage(fs::FS &fs, const char* path) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    file.write(fb->buf, fb->len);
    file.close();

    esp_camera_fb_return(fb);
    Serial.println("Image captured and saved");
}

// Set RGB LED color
void setRGBColor(uint8_t r, uint8_t g, uint8_t b) {
    rgbLED.setPixelColor(0, rgbLED.Color(r, g, b));
    rgbLED.show();
}

// Turn off RGB LED
void turnOffRGB() {
    rgbLED.setPixelColor(0, rgbLED.Color(0, 0, 0));
    rgbLED.show();
}

// Initialize all hardware components by calling respective initialization functions
void initESP() {
    initSerial();
    delay(10000); 
    initBatteryManager();
    delay(10000); 
    initCamera();
    delay(10000); 
    initRGBLED();
    delay(10000); 
    initSDCard();
    delay(10000); 
}

void initAT() {
    ResetModule();                  // AT+CRESET
    delay(10000);                   // Wait for the module to reset

    ResetSimModule();              // AT+CFUN=6
    delay(10000);                   // Wait for SIM module to reset

    TurnOffEcho();                 // ATE0
    delay(10000);                   // Wait for echo to be turned off

    FullFunctionalityMode();       // AT+CFUN=1
    delay(10000);                   // Wait for full functionality mode

    SetNetworkRegistration();       // AT+CREG=2
    delay(10000);                   // Wait for network registration

    AttachPDP();                   // AT+CGATT=1
    delay(10000);                   // Wait for PDP attachment

    ActivatePDP();                 // AT+CGACT=1,1
    delay(10000);                   // Wait for PDP activation

    PowerOnGNSS();                 // AT+CGNSSPWR=1
    delay(10000);                   // Wait for GNSS power on

    StartGNSSUART();               // AT+CGNSSTST=1
    delay(10000);                   // Wait for GNSS UART to start

    SetGNSSMode();                 // AT+CGNSSMODE=15
    delay(10000);                   // Wait for GNSS mode setting

    SetGNSSNEMA();                 // AT+CGNSSNMEA=1,1,1,1,1,1,1,1,1,1
    delay(10000);                   // Wait for NMEA sentences to set

    SetGNSSPort();                 // AT+CGNSSPORTSWITCH=1,1
    delay(10000);                   // Wait for GNSS port to set

    SetCriticalVoltageWarning();    // AT+CVALARM=1,3200,4300
    delay(10000);                   // Wait for voltage warning to set

    ControlTemperatureAlarm();      // AT+CMTE=1
    delay(10000);                   // Wait for temperature alarm to set

    SetCriticalVoltagePowerDown();  // AT+CPMVT=1,3100,4400
    delay(10000);                   // Wait for critical voltage power down
}


