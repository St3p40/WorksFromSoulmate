#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

void drawPixel(byte x, byte y, CRGB color) {
  leds[XY(x, y)] += color;
  if (WIDTH > 24 || HEIGHT > 24) {
    leds[XY(x + 1, y)] += color;
    leds[XY(x - 1, y)] += color;
    leds[XY(x, y + 1)] += color;
    leds[XY(x, y - 1)] += color;
  }
  
}

#define Speed 240
float Koef = 1; //1,3,5,7,9(standart),10,12
#define DotsX2 7
#define sx ((256 / (WIDTH / 2)) - (WIDTH / 16))
#define cy ((256 / (HEIGHT / 2)) - (HEIGHT / 16))

void draw() {
  fadeToBlackBy(leds, NUM_LEDS, 8);
  double t = millis() / (256 - Speed);
  //fadeUsingColor(leds, N_LEDS, CHSV(0, 255, 255));
  for (uint8_t c = 0; c < DotsX2; c++) {
    byte xx = sin8(t + (100 * c) * Koef) / sx;
    byte yy = cos8(t + (150 * c) * Koef) / cy;
      drawPixel(xx, yy, CHSV(c * (256 / DotsX2), 200, 255));
      drawPixel(WIDTH - xx - 1, HEIGHT - yy - 1,  CHSV(c * (256 / DotsX2), 255, 255));
  }
  uint8_t blurAmount = beatsin8(2, 8, 200);
  Koef +=0.005;
  blur2d(leds, LED_COLS, LED_ROWS, blurAmount);
  delay(16);
}

//SubPixel variant
/*
void drawPixelXYF(float x, float y,
  const CRGB & color) {
  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int) x) * 255, yy = (y - (int) y) * 255, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a, b)((uint8_t)(((a) * (b) + (a) + (b)) >> 8))
  uint8_t wu[4] = {
    WU_WEIGHT(ix, iy),
    WU_WEIGHT(xx, iy),
    WU_WEIGHT(ix, yy),
    WU_WEIGHT(xx, yy)
  };
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    CRGB clr = leds[XY(xn, yn)];
    clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    leds[XY(xn, yn)] = clr;
  }
  #undef WU_WEIGHT
}

void drawPixelF(byte x, byte y, CRGB color) {
   drawPixelXYF(x, y, color);
  if (WIDTH > 24 || HEIGHT > 24) {
    drawPixelXYF(x+1, y, color);
    drawPixelXYF(x-1, y, color);
    drawPixelXYF(x, y+1, color);
    drawPixelXYF(x, y-1, color);
  }
  
}

void draw() {
  fadeToBlackBy(leds, NUM_LEDS, 8);
  double t = millis() / (256 - Speed);
  //fadeUsingColor(leds, N_LEDS, CHSV(0, 255, 255));
  for (uint8_t c = 0; c < DotsX2; c++) {
    float xx = (1+sin(radians(t + (100 * c) * Koef))) * 128 / sx;
    float yy = (1+cos(radians(t + (150 * c) * Koef))) * 128 / cy;
      drawPixelF(xx, yy, CHSV(c * (256 / DotsX2), 200, 255));
      drawPixelF(WIDTH - xx - 1, HEIGHT - yy - 1,  CHSV(c * (256 / DotsX2), 255, 255));
  }
  uint8_t blurAmount = beatsin8(2, 8, 200);
  Koef +=0.005;
  blur2d(leds, LED_COLS, LED_ROWS, blurAmount);
  delay(16);
}
*/