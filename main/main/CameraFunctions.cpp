#include "CameraFunctions.h"
#include "esp_camera.h"
#include "Base64.h"
#include "Utilities.h"
#include <esp_http_server.h>

// Function to capture an image
camera_fb_t* captureImage() {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        showRedLED(); // Indicate failure with red LED
        delay(1000);
        ESP.restart();
    }
    return fb;
}

// Function to encode image data to Base64
String encodeImageBase64(camera_fb_t* fb) {
    String encodedImage = "";
    if (fb) {
        encodedImage.reserve((fb->len * 4) / 3 + 4); // Approximate base64 size
        char output[base64_enc_len(3)];
        char* input = (char*)fb->buf;

        for (size_t i = 0; i < fb->len; i += 3) {
            base64_encode(output, input + i, 3);
            encodedImage += output;
        }
    } else {
        Serial.println("Invalid frame buffer");
        showRedLED();
    }
    return urlencode(encodedImage);
}

// Function to start the camera server
void startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    httpd_uri_t stream_uri = {
        .uri = "/stream",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL
    };

    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &stream_uri);
        showGreenLED(); // Indicate success with green LED
    } else {
        Serial.println("Failed to start HTTP server");
        showRedLED();
    }
}

// Stream handler function
esp_err_t stream_handler(httpd_req_t* req) {
    esp_err_t res = ESP_OK;
    camera_fb_t* fb = NULL;
    uint8_t* _jpg_buf = NULL;
    size_t _jpg_buf_len = 0;

    res = httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
    if (res != ESP_OK) return res;

    while (true) {
        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
            break;
        }

        if (fb->format != PIXFORMAT_JPEG) {
            if (!frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len)) {
                Serial.println("JPEG compression failed");
                esp_camera_fb_return(fb);
                res = ESP_FAIL;
                break;
            }
        } else {
            _jpg_buf = fb->buf;
            _jpg_buf_len = fb->len;
        }

        char part_buf[64];
        size_t hlen = snprintf(part_buf, sizeof(part_buf),
                               "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", _jpg_buf_len);
        res = httpd_resp_send_chunk(req, part_buf, hlen);
        if (res == ESP_OK) res = httpd_resp_send_chunk(req, (const char*)_jpg_buf, _jpg_buf_len);
        if (fb->format != PIXFORMAT_JPEG) free(_jpg_buf);
        esp_camera_fb_return(fb);

        if (res != ESP_OK) break;
        yield();
    }

    return res;
}
