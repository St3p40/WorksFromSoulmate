
void drawPixelXYF_X(float x, uint16_t y,
  const CRGB & color) {
  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int) x) * 255, ix = 255 - xx;
  // calculate the intensities for each affected pixel
  uint8_t wu[2] = { ix, xx };
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (int8_t i = 1; i >= 0; i--) {
    int16_t xn = x + (i & 1);
    CRGB clr = leds[XY(xn, y)];
      clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
      clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
      clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    leds[XY(xn, y)] = clr;
  }
}

const float a = (256.0 / (float)LED_COLS+1);
float t;
bool flag = true;

bool bals = true; // line or bals

void draw() {
  fadeToBlackBy(leds, NUM_LEDS, 32);
  t += 10; // (float)millis()/10;
  for (uint8_t i = 0; i < LED_ROWS; i++) {
    uint16_t shift = (i * 8); 
    float sin1 = (1.0+sin(radians(t + shift)))*128.0; 
    byte bright =  constrain(112 * (1 + sin(radians(t + shift + 90))) + 30, 96, 255); 
    byte bright2 =  constrain(112 * (1 + sin(radians(t + shift + 270))) + 30, 96, 255);

    if (flag or !bals) 
      drawPixelXYF_X(sin1 /a, i, CHSV(sin1, 255, bright));
    if (!flag or !bals)
      drawPixelXYF_X((LED_COLS-1)-(sin1 / a), i, CHSV(~(byte)sin1, 255, bright2));
    flag = !flag; 
  }


  blur2d(leds, LED_COLS, LED_ROWS, 64);
  delay(16);
}