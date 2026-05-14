void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
  const CRGB & color) {
  int8_t deltaX = abs(x2 - x1);
  int8_t deltaY = abs(y2 - y1);
  int8_t signX = x1 < x2 ? 1 : -1;
  int8_t signY = y1 < y2 ? 1 : -1;
  int8_t error = deltaX - deltaY;
  
  leds[XY(x2, y2)] += color;
  while (x1 != x2 || y1 != y2) {
    leds[XY(x1, y1)] = color;
    int8_t error2 = error * 2;
    if (error2 > -deltaY) {
      error -= deltaY;
      x1 += signX;
    }
    if (error2 < deltaX) {
      error += deltaX;
      y1 += signY;
    }
  }
}
byte offest;
void draw() {
  fadeToBlackBy(leds, NUM_LEDS, 128);
  offest+=5;
  for (byte i = 0; i < 12; i++) {
   drawLine(beatsin8(10+i,0,LED_COLS-1,i*i),beatsin8(12-i,0,LED_ROWS-1,i*5,64),beatsin8(8+i,0,LED_COLS-1,i*20),beatsin8(14-i,0,LED_ROWS-1,i*5,64),CHSV(90+offest+i*10,255,255));
  }
}