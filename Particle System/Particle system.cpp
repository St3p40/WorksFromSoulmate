//// -----------------------------  Particle sys ------------------------------
//(c)stepko
//19.08.21
#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

#define SPARKS_AM WIDTH
float FADE_KOEF = 1;

float sparksPos[2][SPARKS_AM];
float sparksSpeed[2][SPARKS_AM];
float sparksFade[SPARKS_AM];
byte sparksColor[SPARKS_AM];

float genPos[2];
float gravityPos[2];
bool run = true;

float dist(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool loadingFlag = true;
void reg(byte id) {
  sparksPos[0][id] = genPos[0];
  sparksPos[1][id] = genPos[1];
  sparksSpeed[0][id] = (float)random(-15, 15)/10.;
  sparksSpeed[1][id] = (float)random(-15, 15)/10.;
  sparksFade[id] = 255;
  sparksColor[id] = random();
}
int8_t sign(int8_t a){
  if(a < 0) return -1;
  if(a > 0) return 1;
  if(a == 0) return 0;
}
void phisics(byte id) {
  float force[2] { gravityPos[0] - sparksPos[0][id], gravityPos[1] - sparksPos[1][id]};
  float d = dist(force[0], force[1], force[0] * 2, force[1] * 2);
  force[0] *= (1. / d);
  force[1] *= (1. / d);
  d = constrain(d, 5., HEIGHT * 2.);
  float s = (1.5 * 10) / (d * d);
  force[0] *= s;
  force[1] *= s;
  sparksSpeed[0][id] += force[0];
  sparksSpeed[1][id] += force[1];
  sparksFade[id] -= (255. / (float)(((HEIGHT + WIDTH) / 2) * FADE_KOEF));
  float sq = ((sparksSpeed[0][id] * sparksSpeed[0][id]) + (sparksSpeed[1][id] * sparksSpeed[1][id]));
  if (sq > 2.25) {
    sq = sqrtf(sq);
    sparksSpeed[0][id] *= (1. / sq) * 1.5;
    sparksSpeed[1][id] *= (1. / sq) * 1.5;
  }
  sparksPos[0][id] += sparksSpeed[0][id];
  sparksPos[1][id] += sparksSpeed[1][id];
}

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

void render(byte id, CRGB Col) {
  if (loadingFlag) {
    for (byte i = 0; i < SPARKS_AM; i++) {
      reg(i);
      for (byte a = 0; a < i; a++) {
        phisics(a);
      }
    }
    loadingFlag = false;
  }
  phisics(id);
  if (sparksPos[1][id] < ((float)HEIGHT - 1.) and sparksPos[1][id] >= 0)
    if (sparksPos[0][id] < ((float)WIDTH - 1.) and sparksPos[0][id] >= 0) {
      CRGB color = Col;
      wu_pixel(sparksPos[0][id]*255, sparksPos[1][id]*255, & color);
    }
}

void setGenPos(int x, int y) {
  genPos[0] = x;
  genPos[1] = y;
}

void setGravityPos(int x, int y) {
  gravityPos[0] = x;
  gravityPos[1] = y;
}

void setRegenRule(byte id, bool b) {
  if (b) reg(id);
}

void draw() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  setGenPos((float)beatsin16(10, 0, (WIDTH-1) * 10)/10., (float)beatsin16(10, 0, (HEIGHT-1) * 10, 0, 16384)/10.);
  setGravityPos((float)WIDTH / 2., (float)HEIGHT / 2.);
  for (byte i = 0; i < SPARKS_AM; i++) {
    setRegenRule(i, (sparksPos[0][i] <= 0 || sparksPos[0][i] >= (float)WIDTH - 1. || sparksPos[1][i] < 0 || sparksPos[1][i] >= (float)HEIGHT - 1. || sparksFade[i] <= 35) ? 1 : 0);
    render(i, CHSV(sparksColor[i], 255, constrain(sparksFade[i], 32, 255)));
  }
  delay(16);
}