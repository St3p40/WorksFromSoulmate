//Hypnosis
//Stepko and Sutaburosu
//30/01/23
bool setupm = 1;
const uint8_t C_X = LED_COLS / 2;
const uint8_t C_Y = LED_ROWS / 2;
const uint8_t mapp = 255 / LED_COLS;
struct{
  uint8_t angle;
  uint8_t radius;
} rMap[LED_COLS][LED_ROWS];

void draw() {
  FastLED.clear();
  if (setupm) {
    setupm = 0;
    for (int8_t x = -C_X; x < C_X + (LED_COLS % 2); x++) {
      for (int8_t y = -C_Y; y < C_Y + (LED_ROWS % 2); y++) {
        rMap[x + C_X][y + C_Y].angle = 128 * (atan2(y, x) / PI);
        rMap[x + C_X][y + C_Y].radius = hypot(x, y) * mapp; //thanks Sutaburosu
      }
    }
  }
  static uint16_t t;
  t += 4;
  for (uint8_t x = 0; x < LED_COLS; x++) {
    for (uint8_t y = 0; y < LED_ROWS; y++) {
      byte angle = rMap[x][y].angle;
      byte radius = rMap[x][y].radius;
      leds[XY(x, y)] = ColorFromPalette(RainbowStripeColors_p, t / 2 + radius + angle, sin8(angle + (radius * 2) - t));
    }
  }
  delay(16);
}