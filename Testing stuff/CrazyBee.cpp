void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
  const CRGB & color) {
  int8_t deltaX = abs(x2 - x1);
  int8_t deltaY = abs(y2 - y1);
  int8_t signX = x1 < x2 ? 1 : -1;
  int8_t signY = y1 < y2 ? 1 : -1;
  int8_t error = deltaX - deltaY;
  
  leds[XY(x2, y2)] = color;
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
byte coord[2][2];
uint16_t offest;
void draw() {
  fadeToBlackBy(leds, NUM_LEDS, 2);
  long t = millis();
  offest++;
  coord[0][1] = coord[0][0];
  coord[1][1] = coord[1][0];
  byte x = inoise8(t, offest);
  x = qadd8(qsub8(x,16),scale8(qsub8(x,16),39));
  byte y = inoise8(offest,t);
  y = qadd8(qsub8(y,16),scale8(qsub8(y,16),39));
  coord[0][0] = map(x, 0, 255, 0, LED_COLS - 1);
  coord[1][0] = map(y, 0, 255, 0, LED_ROWS - 1);
  drawLine(coord[0][0], coord[1][0], coord[0][1], coord[1][1], CHSV(qadd8(qsub8(inoise8(offest),16),scale8(qsub8(inoise8(offest),16),39)),255,255));
  blur2d(leds, LED_COLS, LED_ROWS, 16);
}