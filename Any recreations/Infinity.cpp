void draw() {
  CRGB color = CHSV(beatsin8(10, 1, 10), 10, 255);
  leds[XY(beatsin8(12, 0, LED_COLS - 1), beatsin8(24, LED_ROWS / 4, LED_ROWS - 1 - LED_ROWS / 4))] += color;
  if (LED_COLS > 24 || LED_ROWS > 24) {
    leds[XY(beatsin8(12, 0, LED_COLS - 1) + 1, beatsin8(24, LED_ROWS / 4, LED_ROWS - 1 - LED_ROWS / 4))] += color;
    leds[XY(beatsin8(12, 0, LED_COLS - 1) - 1, beatsin8(24, LED_ROWS / 4, LED_ROWS - 1 - LED_ROWS / 4))] += color;
    leds[XY(beatsin8(12, 0, LED_COLS - 1), beatsin8(24, LED_ROWS / 4, LED_ROWS - 1 - LED_ROWS / 4) + 1)] += color;
    leds[XY(beatsin8(12, 0, LED_COLS - 1), beatsin8(24, LED_ROWS / 4, LED_ROWS - 1 - LED_ROWS / 4) - 1)] += color;
    blur2d(leds, LED_COLS, LED_ROWS, 64);
  }
  fadeToBlackBy(leds, NUM_LEDS, 2);
  delay(16);
}