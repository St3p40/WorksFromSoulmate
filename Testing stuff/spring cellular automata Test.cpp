class block {
  public:
    int16_t pos = 0;
  int8_t vel = 10;
  void update() {
    pos += vel;
  }
};

block bl[NUM_LEDS];
bool first = 1;
void draw() {
  if (first) {
    for (uint8_t y = 0; y < LED_ROWS; y++) {
      for (uint8_t x = 0; x < LED_COLS; x++) {
        bl[XY(x, y)].pos = sin8((x + y) * 16);
      }
    }
    first = 0;
  }
  for (uint8_t y = 0; y < LED_ROWS; y++) {
    for (uint8_t x = 0; x < LED_COLS; x++) {
      int16_t sum = 0;
      uint8_t sum_div = 0;
      if (x) {
        sum += bl[XY(x - 1, y)].pos;
        sum_div++;
      }
      if (x != LED_COLS - 1) {
        sum += bl[XY(x + 1, y)].pos;
        sum_div++;
      }
      if (y) {
        sum += bl[XY(x, y - 1)].pos;
        sum_div++;
      }
      if (y != LED_ROWS - 1) {
        sum += bl[XY(x, y + 1)].pos;
        sum_div++;
      }
      bl[XY(x, y)].vel += (sum / sum_div) - bl[XY(x, y)].pos;
    }
  }
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    bl[i].update();
    leds[i] = ColorFromPalette(HeatColors_p, constrain(abs(bl[i].pos), 0, 255));
  }
}