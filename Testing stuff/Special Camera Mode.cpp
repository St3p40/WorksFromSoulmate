#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

void drawPixel(byte x, byte y, CRGB color) {
  leds[XY(x, y)] += color;
  if (LED_COLS > 24 || LED_ROWS > 24) {
    leds[XY(x + 1, y)] += color;
    leds[XY(x - 1, y)] += color;
    leds[XY(x, y + 1)] += color;
    leds[XY(x, y - 1)] += color;
  }
  
}
int8_t zF;int8_t zD;
void MoveX(uint8_t am = 128, int8_t amplitude = 1, float shift = 0) {
  CRGB ledsbuff[LED_COLS];
  for (uint8_t y = 0; y < HEIGHT; y++) {
    int16_t amount = ((int16_t)am - 128) * 2 * amplitude + shift * 256  ;
    int8_t delta = abs(amount) >> 8 ;
    int8_t fraction = abs(amount) & 255;
    for (uint8_t x = 0 ; x < WIDTH; x++) {
      if (amount < 0) {
        zD = x - delta; zF = zD - 1;
      } else {
        zD = x + delta; zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black  ;
      if ((zD >= 0) && (zD < WIDTH)) PixelA = leds[XY(zD, y)];
      CRGB PixelB = CRGB::Black ;
      if ((zF >= 0) && (zF < WIDTH)) PixelB = leds[XY(zF, y)];
      ledsbuff[x] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));   // lerp8by8(PixelA, PixelB, fraction );
    }
    for(uint8_t x = 0; x < LED_COLS; x++){
    leds[XY(x,y)] = ledsbuff[x];}
  }
}

void MoveY(uint8_t am = 128, int8_t amplitude = 1, float shift = 0) {
  CRGB ledsbuff[LED_ROWS];
  for (uint8_t x = 0; x < WIDTH; x++) {
    int16_t amount = ((int16_t)am - 128) * 2 * amplitude + shift * 256 ;
    int8_t delta = abs(amount) >> 8 ;
    int8_t fraction = abs(amount) & 255;
    for (uint8_t y = 0 ; y < HEIGHT; y++) {
      if (amount < 0) {
        zD = y - delta; zF = zD - 1;
      } else {
        zD = y + delta; zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black ;
      if ((zD >= 0) && (zD < HEIGHT)) PixelA = leds[XY(x, zD)];
      CRGB PixelB = CRGB::Black ;
      if ((zF >= 0) && (zF < HEIGHT)) PixelB = leds[XY(x, zF)];
      ledsbuff[y] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    }
    for(uint8_t y = 0; y < LED_ROWS; y++){
    leds[XY(x,y)] = ledsbuff[y];}
  }
}

void draw() {
  MoveX(beatsin8(12, 0, 255));
  MoveY(beatsin8(15, 0, 255));
  drawPixel(beatsin8(12, 2, LED_COLS - 3), beatsin8(15, 2, LED_ROWS - 3), CHSV(millis() / 100, 255, 255));
  blur2d(leds, LED_COLS, LED_ROWS, 64);
  delay(16);
}
/*
void draw() {
  MoveX(beatsin8(36, 0, LED_COLS / 2) - LED_COLS / 4);
  MoveY(beatsin8(45, 0, LED_ROWS / 2, 0, 64) - LED_ROWS / 4);
  drawPixel(LED_COLS / 2, LED_ROWS / 2, CHSV(millis() / 100, 255, 255));
  blur2d(leds, LED_COLS, LED_ROWS, 64);
  delay(16);
}*/