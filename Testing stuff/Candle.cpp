int dist(int x1, int y1, int x2, int y2) {
  return sqrt16((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int dist0(int x1, int y1, int x2, int y2) {
  return sqrt16((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) / 2);
}

void draw() {
  uint32_t t = millis();
  byte dX = map(inoise8(t, 0, t), 0, 255, 0, LED_COLS);
  byte dY = map(inoise8(0, t), 0, 255, LED_ROWS / 2, LED_ROWS * 2);
  for (byte x = 0; x < LED_COLS; x++) {
    for (byte y = 0; y < LED_ROWS; y++) {
      int dista = dist0(x, y, LED_COLS / 3, LED_ROWS / 10);
      dista += dist0(x, y, LED_COLS - 1 - LED_COLS / 3, LED_ROWS / 10);
      dista += dist0(x, y, LED_COLS / 2, LED_ROWS / 10);
      dista += dist(x, y, dX, dY);
      if (dista >= LED_COLS + LED_ROWS) dista = 0;
      nblend(leds[XY(x, y)], ColorFromPalette(HeatColors_p, map(dista * 2, 0, (LED_COLS + LED_ROWS), 255, 0)), 30);
    }
  }
  blur2d(leds, LED_COLS, LED_ROWS, 64);
}