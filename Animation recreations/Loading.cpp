void wu_pixel(uint32_t x, uint32_t y, CRGB * col) { //awesome wu_pixel procedure by reddit u/sutaburosu
  // extract the fractional parts and derive their inverses
  uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
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
    uint16_t xy = XY((x >> 8) + (i & 1), (y >> 8) + ((i >> 1) & 1));
    leds[xy].r = qadd8(leds[xy].r, col -> r * wu[i] >> 8);
    leds[xy].g = qadd8(leds[xy].g, col -> g * wu[i] >> 8);
    leds[xy].b = qadd8(leds[xy].b, col -> b * wu[i] >> 8);
  }
}


void draw() {
  FastLED.clear();
  CRGB col = CRGB(255, 255, 255);
  for (byte i = 0; i < 16; i++) {
    wu_pixel(
      beatsin16(20, (LED_COLS >> 3) << 8, (LED_COLS - (LED_COLS >> 3) - 1) << 8, 0, -beatsin16(16, 1, 2048) * i),
      beatsin16(20, (LED_ROWS >> 3) << 8, (LED_ROWS - (LED_ROWS >> 3) - 1) << 8, 0, 16384 - beatsin16(16, 1, 2048) * i),
      & col);
  }
}