//// -----------------------------  Fire (Particle sys) ------------------------------
//(c)stepko
//19.08.21
#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

#define LENGHT HEIGHT

byte speed = 255;
static float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max)
{
  return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}

const int SPARKS_AM = HEIGHT * 4;
float FADE_KOEF = fmap(HEIGHT, 16, 32, 0.8, 0.5)*fmap(speed,1,255,3.5,1.);
float SpeedK = fmap(speed,1,255,0.2,2);
float SpeedDecX = fmap(NUM_LEDS, 256, 1024, 0.05, 0.8)*fmap(speed,1,255,0.1,1);
float SpeedDecY = 0;

int sparksPos[2][SPARKS_AM];
float sparksSpeed[2][SPARKS_AM];
float sparksFade[SPARKS_AM];

int genPos[2];
int gravityPos[2];
bool run = true;

bool loadingFlag = true;
void reg(byte id) {
  sparksPos[0][id] = genPos[0];
  sparksPos[1][id] = genPos[1];
  byte al = random8();
  sparksSpeed[0][id] = (sin(radians(128 - al)) * LENGHT / 2)*fmap(speed,1,255,0.1,1);;
  sparksSpeed[1][id] = (cos(radians(128 - al)) * LENGHT / 8)*fmap(speed,1,255,0.1,1);;
  sparksFade[id] = 255;
}

void phisics(byte id) {
  if (SpeedK) {
    if (gravityPos[0] < sparksPos[0][id])
      sparksSpeed[0][id] -= SpeedK;
    else
      sparksSpeed[0][id] += SpeedK;
    if (gravityPos[1] < sparksPos[1][id])
      sparksSpeed[1][id] -= SpeedK;
    else
      sparksSpeed[1][id] += SpeedK;
  }
  sparksFade[id] -= (255. / float(LENGHT * FADE_KOEF));
  if (SpeedDecX && sparksSpeed[0][id]) {
    if (sparksSpeed[0][id] > 0)
      sparksSpeed[0][id] -= SpeedDecX;
    else
      sparksSpeed[0][id] += SpeedDecX;
    if (abs(sparksSpeed[0][id]) <= SpeedDecX)
      sparksSpeed[0][id] = 0;
  }
  if (SpeedDecY && sparksSpeed[1][id]) {
    if (sparksSpeed[1][id] > 0)
      sparksSpeed[1][id] -= SpeedDecY;
    else
      sparksSpeed[1][id] += SpeedDecY;
    if (abs(sparksSpeed[1][id]) <= SpeedDecY)
      sparksSpeed[1][id] = 0;
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

const uint8_t maxSize = max(LED_COLS, LED_ROWS); 

void render(byte id, CRGB color) {
  phisics(id);
  if (sparksPos[1][id] < ((HEIGHT - 1) * 10) and sparksPos[1][id] >= 0)
    if (sparksPos[0][id] < ((WIDTH - 1) * 10) and sparksPos[0][id] >= 0) {
      wu_pixel(sparksPos[0][id] * 25.6, sparksPos[1][id] * 25.6, & color);
      if (LENGHT > 24) {
        wu_pixel((sparksPos[0][id] + 1) * 25.6, sparksPos[1][id] * 25.6, & color);
        wu_pixel(sparksPos[0][id] * 25.6, (sparksPos[1][id] + 1) * 25.6, & color);
        wu_pixel((sparksPos[0][id] - 1) * 25.6, sparksPos[1][id] * 25.6, & color);
        wu_pixel(sparksPos[0][id] * 25.6, (sparksPos[1][id] - 1) * 25.6, & color);
      }
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

void start() {
  for (byte i = 0; i < SPARKS_AM; i++) {
    reg(i);
    for (byte a = 0; a < i; a++) {
      setRegenRule(a, (sparksPos[0][a] <= 0 || sparksPos[0][a] >= (WIDTH - 1) * 10 || sparksPos[1][a] < 0 || sparksPos[1][a] >= (HEIGHT - 1) * 10 || sparksFade[a] < 20) ? 1 : 0);
      phisics(a);
    }
  }
}

void draw() {
  if (loadingFlag) {
    setGenPos(WIDTH * 5, HEIGHT);
  }
  uint16_t t = millis() / 16;
  byte noise = inoise8(t);
  setGravityPos(map(noise, 0, 255, WIDTH * 4, WIDTH * 6), map(abs(128 - noise), 0, 127, HEIGHT * 8, HEIGHT * 11));
  if (loadingFlag) {
    start();
    loadingFlag = false;
  }
  //FastLED.clear();
  fadeToBlackBy(leds, NUM_LEDS, 150);
  for (byte i = 0; i < SPARKS_AM; i++) {
    setRegenRule(i, (sparksPos[0][i] <= 0 || sparksPos[0][i] >= (WIDTH - 1) * 10 || sparksPos[1][i] < 0 || sparksPos[1][i] >= (HEIGHT - 1) * 10 || sparksFade[i] < 20) ? 1 : 0);
    render(i, CHSV(10 + sparksFade[i] / 25.5, constrain(255 - (sparksFade[i] / 5.), 0, 255), sparksFade[i]));
  }
  blur2d(leds, LED_COLS, LED_ROWS, 128);
  delay(16);
}