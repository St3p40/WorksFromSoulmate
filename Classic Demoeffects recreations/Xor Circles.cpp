double log2(double num){
    double number=log(num)/log(2);
    return (number);
}

const uint8_t scale_x = log2(64/LED_COLS);
const uint8_t scale_y = log2(64/LED_ROWS);

void draw() {
  uint8_t x1sh = beatsin8(5, 0, LED_COLS);
  uint8_t y1sh = beatsin8(6, 0, LED_ROWS);
  uint8_t x2sh = beatsin8(7, 0, LED_COLS);
  uint8_t y2sh = beatsin8(4, 0, LED_ROWS);
  for (uint8_t y = 0; y < LED_ROWS; y++) {
    for (uint8_t x = 0; x < LED_COLS; x++) {
      int8_t cx = x-x1sh;
      int8_t cy = y-y1sh;
      uint8_t a = sqrt16((( cx * cx) + ( cy * cy))) << scale_x;
      cx = x-x2sh;
      cy = y-y2sh;
      uint8_t v = sqrt16((( cx * cx) + ( cy * cy))) << scale_y;
      leds[XY(x, y)] = (((a ^ v) >> 4) & 1) * 255;
    }
  }
}