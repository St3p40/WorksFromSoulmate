void wu_pixel(uint32_t x, uint32_t y, CRGB * col) {      //awesome wu_pixel procedure by reddit u/sutaburosu
  // extract the fractional parts and derive their inverses
  uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    uint16_t xy = XY((x >> 8) + (i & 1), (y >> 8) + ((i >> 1) & 1));
    leds[xy].r = qadd8(leds[xy].r, col->r * wu[i] >> 8);
    leds[xy].g = qadd8(leds[xy].g, col->g * wu[i] >> 8);
    leds[xy].b = qadd8(leds[xy].b, col->b * wu[i] >> 8);
  }
}

#define ROT 0
#if(ROT)
#define AM LED_ROWS
#else
#define AM LED_COLS
#endif
byte val[AM];
byte min[AM];
byte max[AM];
byte col[AM];
byte speed[AM]; //just for test

void drawBar(byte value, byte minimum, byte maximum, byte pos, CRGB color, bool Vert) {
  if (Vert) {
    for (int x = 0; x < map(value, minimum, maximum, 0, LED_COLS*256); x +=16) {
      wu_pixel(x, pos * 256, &color);
    }
  } else {
    for (int y = 0; y < map(value, minimum, maximum, 0, LED_ROWS*256); y +=16) {
      wu_pixel(pos * 256, y, &color);
    }
  }
}

bool SetUp = true;
void draw() {
  if (SetUp) {
    SetUp = false;
    for (byte i = 0; i < AM; i++) {
      speed[i] = random(15, 30);
      col[i] = random();
      min[i] = random(0, 32);
      max[i] = random(64, 255);
      val[i] = 1;
    }
  }
  FastLED.clear();
  if (ROT) {
    for (byte y = 0; y < LED_ROWS; y++) {
      val[y] = beatsin8(speed[y], min[y], max[y]); //just for test
      drawBar(val[y], min[y], max[y], y, CHSV(col[y],255,255), 1);
    }
  } else {
    for (byte x = 0; x < LED_COLS; x++) {
      val[x] = beatsin8(speed[x], min[x], max[x]); //just for test
      drawBar(val[x], min[x], max[x], x, CHSV(col[x],255,255), 0);
    }
  }
}