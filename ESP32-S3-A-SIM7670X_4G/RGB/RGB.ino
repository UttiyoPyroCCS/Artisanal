#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 38, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(100);
  strip.show();
}

void loop() {
  for(int i = 0; i <= 255; i++) {
    strip.setPixelColor(0, Wheel(i));
    strip.show();
    delay(10);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3) / 2, 0, (WheelPos * 3) / 2);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3) / 2, (255 - WheelPos * 3) / 2);
  }
  WheelPos -= 170;
  return strip.Color((WheelPos * 3) / 2, (255 - WheelPos * 3) / 2, 0);
}
