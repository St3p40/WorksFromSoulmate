byte buff[LED_COLS][LED_ROWS + 1];
uint16_t shift = 255;
uint16_t t;
void draw() {
  t = millis() / 10;
  shift += 254;
  if (shift >= 255) {
    if (random() % 2) buff[random(0, LED_COLS)][LED_ROWS] = random(192, 255);
    for (byte x = 0; x < LED_COLS; x++) {
      for (byte y = 0; y < LED_ROWS + 1; y++) {
        if (buff[x][y] && y) { buff[x][y - 1] = buff[x][y];
          buff[x][y] = 0; } else if (buff[x][y] && !y) {
          buff[x][y] = 0;
        }
      }
    }
    shift %= 255;
  }
  for (byte y = 0; y < LED_ROWS; y++) {
    int yred = (LED_ROWS-y)*(255/LED_ROWS)*2;
    for (byte x = 0; x < LED_COLS; x++) {
      uint8_t noise = constrain((inoise8(x*10 + t,y*25 + t) * 3) / 2 - yred,0,255);
      leds[XY(x,y)]=CHSV(0,0,noise);
      CRGB col = CHSV(0, 0, buff[x][y]);
      leds[XY(x, y)] += nblend(col, CHSV(0, 0, buff[x][y + 1]), shift);
    }
  }
}