class block {
  public:
    int16_t pos = 0;
  int16_t vel = 0;
  void update() {
    vel += (vel < 0) ? 1 : (vel > 0) ? -1 : 0;
    pos += vel / 2;
  }
};
block bl[LED_COLS];
bool issetup = 1;

void dPixXYF(uint8_t x, float y, CRGB col) {
  byte ay = byte(y);
  byte ysh = (y - ay) * 255;
  CRGB col1 = nblend(col, CRGB(0, 0, 0), ysh);
  CRGB col2 = nblend(col, CRGB(0, 0, 0), ~ysh);
  leds[XY(x, ay)] += col1;
  leds[XY(x, ay + 1)] += col2;
}

void draw() {
  if (issetup) {
    issetup = 0;
    bl[random()%LED_COLS].vel = -256;
  }
  EVERY_N_SECONDS(5){
    bl[random()%LED_COLS].vel = -256;
  }
  for (uint8_t i = 0; i < LED_COLS; i++) {
    int16_t sum = 0;
    if (i) {
      sum += bl[i - 1].pos;
    }
    if (i != LED_COLS - 1) {
      sum += bl[i + 1].pos;
    }
    bl[i].vel += ((sum / 2) - bl[i].pos);
  }
  for (uint8_t i = 0; i < LED_COLS; i++) {
    bl[i].update();
    uint16_t coord = map(bl[i].pos, -1024, 1024, 0, (LED_ROWS - 1) << 8);
    for (uint8_t j = 0; j < LED_ROWS; j++) {
      leds[XY(i, j)] = (j < coord >> 8) ? CHSV(150, 255, 255) : (j == coord >> 8) ? CHSV(150, 255, coord % 255) : CRGB(0, 0, 0);
    }
  }
}