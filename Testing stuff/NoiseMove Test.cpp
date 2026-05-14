#define D_NUM LED_COLS / 4

struct {
  float x = random();
  float y = random();
  float w = random();
  uint8_t color = random();
  void rpoint(){
    if (x<0) x = LED_COLS; if(x>LED_COLS) x = 0;
    if (y<0) y = LED_ROWS; if(y>LED_ROWS) y = 0;
    if (w<128) w = 255; if(w>255) w = 128;
  }
}
dot[D_NUM];

const float speedfact = 35;//((float)NUM_LEDS/1024.0) * 20;

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
    leds[XY(xn, yn)] = clr;
  }
  #undef WU_WEIGHT
}
uint32_t t;
void draw() {
  t++;
  //FastLED.clear();
  EVERY_N_MILLISECONDS(500){
  fadeToBlackBy(leds, NUM_LEDS, 1);}
  for (uint8_t i = 0; i < D_NUM; i++) {
    dot[i].color += 0.1;
    dot[i].x += inoise8_raw(t + 39000, i * 2048, i * 40961) / speedfact;
    dot[i].y += inoise8_raw(i * 4096, i * 20418, t) / speedfact;
    dot[i].w += inoise8_raw(i * 4096, i * 20418, t) / speedfact * 2;
    dot[i].rpoint();
    drawPixelXYF(dot[i].x, dot[i].y, CHSV(dot[i].color, 255, dot[i].w));
  }
  blur2d(leds, LED_COLS, LED_ROWS, 4);
}