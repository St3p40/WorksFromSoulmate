#define CenterX ((LED_COLS / 2) - 0.5)
#define CenterY ((LED_ROWS / 2) - 0.5)
byte AM = 1;
float Angle;
bool change = true;
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
bool incenter; 
draw() {
  fadeToBlackBy(leds, NUM_LEDS, 8);
  float t = (float)millis() / 500.0f;
  float CalcRad = (sin(t / 2) + 1);
  if (CalcRad <= 0.001) {
    if(!incenter){
    AM += ((change * 2) - 1) * ((AM >= 4)? 2 : 1);
    Angle = 6.28318531 / AM;
    if (AM <= 1) change = true;
    else if (AM >= (LED_COLS + LED_ROWS) / 2) change = false;}
    incenter = 1;
  } else incenter = 0;
  float radX = CalcRad * CenterX / 2;
  float radY = CalcRad * CenterY / 2;
  for (byte i = 0; i < AM; i++) {
    drawPixelXYF((CenterX + sin(t + (Angle * i)) * radX), (CenterY + cos(t + (Angle * i)) * radY), ColorFromPalette(HeatColors_p, t*10 + ((256 / AM) * i)));
  }
  delay(16);
}