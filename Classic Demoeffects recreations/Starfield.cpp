#define NSTARS (LED_COLS + LED_ROWS)/2
int dist(int x1, int y1, int x2, int y2) {
  return sqrt16((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
void drawPixelXYF(float x, float y,
  const CRGB & color) {
  // extract the fractional parts and derive their inverses
  uint8_t xx = (x - (int) x) * 255, yy = (y - (int) y) * 255, ix = 255 - xx, iy = 255 - yy;
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
    int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    CRGB clr = leds[XY(xn, yn)];
    clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    nblend(leds[XY(xn, yn)],clr,128);
  }
  #undef WU_WEIGHT
}

static float fmap(const float x,
  const float in_min,
    const float in_max,
      const float out_min,
        const float out_max) {
  return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}

struct {
  int16_t PosX;
  int16_t PosY;
  int16_t PosW;
  
  void reg() {
    PosX = random(LED_COLS * -5, LED_COLS * 5);
    PosY = random(LED_ROWS * -5, LED_ROWS * 5);
    PosW = LED_COLS * 5;
  };
  void run() {
    PosW -=5;
    if (PosW < 0) {
      reg();
    }
    float SX = LED_COLS * 0.5 + fmap(float(PosX) / float(PosW), 0, 1, 0, (float) LED_COLS/2);
    float SY = LED_ROWS * 0.5 + fmap(float(PosY) / float(PosW), 0, 1, 0, (float) LED_ROWS/2);
    if(SX>0 && SX < LED_COLS && SY>0 && SY<LED_ROWS)
      drawPixelXYF(SX, SY, CHSV(0, 0, map(PosW, 0, LED_COLS * 5, 255, 100)));
  }
}
star[NSTARS];

bool loadingFlag = 1;

void draw() {
  if (loadingFlag) {
    for (byte i = 0; i < NSTARS; i++) {
      star[i].run();
      star[i].PosW = random(LED_COLS * 5);
    }
    loadingFlag = 0;
  }
  //FastLED.clear();
  fadeToBlackBy(leds, NUM_LEDS, 32);
  for (byte i = 0; i < NSTARS; i++) {
    star[i].run();
  }
}