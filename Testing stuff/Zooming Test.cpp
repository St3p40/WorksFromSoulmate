bool issetup = true;
CRGB zoombuff[(LED_COLS / 2) + LED_COLS % 2][(LED_ROWS / 2) + LED_ROWS % 2];
uint16_t pos = 0;
uint8_t zoomspeed = 128;
uint8_t zoomposX, zoomposY;
void draw() {
  if (issetup) {
    issetup = 0;
    for (uint8_t x = 0; x < LED_COLS; x++) {
      for (uint8_t y = 0; y < LED_ROWS; y++) {
        leds[XY(x, y)] = CHSV(inoise8(x * 180, y * 180), 255, 255);
      }
    }
    for (uint8_t x = 0; x < (LED_COLS / 2) + LED_COLS % 2; x++) {
      for (uint8_t y = 0; y < (LED_ROWS / 2) + LED_ROWS % 2; y++) {
        zoombuff[x][y] = leds[XY(x + LED_COLS / 4, y + LED_ROWS / 4)];
        
      }
    }
  }

  pos += zoomspeed;
  if (pos > 255) {
    pos = 0;
    for (uint8_t x = 0; x < LED_COLS; x++) {
    for (uint8_t y = 0; y < LED_ROWS; y++) {
      leds[XY(x, y)] = zoombuff[x / 2][y / 2];
    }
  }
  uint16_t t = millis() / 1000;
  zoomposX = map(sin8(t),0,255,0,LED_COLS / 2);
  zoomposY = map(sin8(t+10929),0,255,0,LED_ROWS / 2);
    for (uint8_t a = 0; a < NUM_LEDS / 16; a++) {
      leds[random(0, NUM_LEDS)] = CHSV(random(), 255, 255);
    }
    for (uint8_t x = 0; x < (LED_COLS / 2) + LED_COLS % 2; x++) {
      for (uint8_t y = 0; y < (LED_ROWS / 2) + LED_ROWS % 2; y++) {
        zoombuff[x][y] = leds[XY(x + zoomposX, y + zoomposY)];
      }
    } 
  }
  for (uint8_t x = 0; x < LED_COLS; x++) {
    for (uint8_t y = 0; y < LED_ROWS; y++) {
      nblend(leds[XY(x, y)], zoombuff[x / 2][y / 2], pos);
    }
  }
  delay(200);
}