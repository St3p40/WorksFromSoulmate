#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

//// ----------------------------- Curve ------------------------------
//(c)stepko
//Idea from Ldir's F_lying

#define subPix 0

void drawCurve(uint8_t x, uint8_t y, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, CRGB col) {
  float xu = 0.0, yu = 0.0, u = 0.0;
  int i = 0;
  for (u = 0.0; u <= 1.0; u += 0.01) {
    xu = pow(1 - u, 3) * x + 3 * u * pow(1 - u, 2) * x2 + 3 * pow(u, 2) * (1 - u) * x3 +
      pow(u, 3) * x3;
    yu = pow(1 - u, 3) * y + 3 * u * pow(1 - u, 2) * y2 + 3 * pow(u, 2) * (1 - u) * y3 +
      pow(u, 3) * y3;
    leds[XY(xu, yu)] += col;
  }
}

void drawPixelXYF(float x, float y, CRGB color) {
  // if (x < 0 || y < 0 || x > ((float)WIDTH - 1) || y > ((float)HEIGHT - 1)) return;
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
    leds[XY(xn, yn)] += clr;
  }
}

void drawCurveF(float x, float y, float x2, float y2, float x3, float y3, CRGB col) {
  float xu = 0.0, yu = 0.0, u = 0.0;
  int i = 0;
  for (u = 0.0; u <= 1.0; u += 0.01) {
    xu = pow(1 - u, 3) * x + 3 * u * pow(1 - u, 2) * x2 + 3 * pow(u, 2) * (1 - u) * x3 +
      pow(u, 3) * x3;
    yu = pow(1 - u, 3) * y + 3 * u * pow(1 - u, 2) * y2 + 3 * pow(u, 2) * (1 - u) * y3 +
      pow(u, 3) * y3;
    drawPixelXYF(xu, yu, col);
  }
}

byte hue;
#define speed - 5
void draw() {
  fadeToBlackBy(leds, NUM_LEDS, 30);
  byte x1 = beatsin8(18 + speed, 0, (LED_COLS - 1));
  byte x2 = beatsin8(23 + speed, 0, (LED_COLS - 1));
  byte x3 = beatsin8(27 + speed, 0, (LED_COLS - 1));
  
  byte y1 = beatsin8(20 + speed, 0, (LED_ROWS - 1));
  byte y2 = beatsin8(26 + speed, 0, (LED_ROWS - 1));
  byte y3 = beatsin8(15 + speed, 0, (LED_ROWS - 1));
  
  if (subPix)
    drawCurveF(x1, y1, x2, y2, x3, y3, CHSV(hue, 255, 255));
  else
    drawCurve(x1, y1, x2, y2, x3, y3, CHSV(hue, 255, 255));
  blur2d(leds, LED_COLS, LED_ROWS, 64);
  hue++;
  delay(16);
}