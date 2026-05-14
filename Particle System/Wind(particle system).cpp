//// -----------------------------  WIND (Particle sys) ------------------------------
//(c)stepko
//19.08.21
#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

const int SPARKS_AM = HEIGHT * 0.5;
float FADE_KOEF = 1.2;
float SpeedK = 2;
float SpeedDecX = .1;
float SpeedDecY = 0;

int sparksPos[2][SPARKS_AM];
float sparksSpeed[2][SPARKS_AM];
float sparksFade[SPARKS_AM];

int genPos[2];
int gravityPos[2];
bool run = true;

bool loadingFlag = true;
void reg(byte id) {//broken
  sparksPos[0][id] = genPos[0];
  sparksPos[1][id] = random(0,HEIGHT * 10);
  byte al = random8();
  sparksSpeed[0][id] = (sin(radians(128 - al)) * WIDTH / 2);
  sparksSpeed[1][id] = (cos(radians(128 - al)) * HEIGHT / 8);
  sparksFade[id] = 255;
}

void phisics(byte id) {//broken
  if (SpeedK) {
    if (gravityPos[0] < sparksPos[0][id])
      sparksSpeed[0][id] -= SpeedK;
    else
      sparksSpeed[0][id] += SpeedK;
    if (gravityPos[1] < sparksPos[1][id])
      sparksSpeed[1][id] -= SpeedK/2;
    else
      sparksSpeed[1][id] += SpeedK/2;
  }
  sparksFade[id] -= (255. / float(WIDTH * FADE_KOEF));
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
  sparksPos[0][id] += constrain(sparksSpeed[0][id],-15,15);
  sparksPos[1][id] += constrain(sparksSpeed[1][id],-15,15);
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
  phisics(id);
  if (sparksPos[1][id] < ((HEIGHT - 1) * 10) and sparksPos[1][id] >= 0)
    if (sparksPos[0][id] < ((WIDTH - 1) * 10) and sparksPos[0][id] >= 0) {
      CRGB color = Col;
      wu_pixel(sparksPos[0][id] * 25.6, sparksPos[1][id] * 25.6, & color);
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
  setGenPos(0, HEIGHT * 5);
  byte noise = inoise8(millis() / 10);
  setGravityPos(WIDTH * 11, map(abs(128 - noise), 0, 127, HEIGHT, HEIGHT * 9));
  if (loadingFlag) {
    start();
    loadingFlag = false;
  }
  //FastLED.clear();
  fadeToBlackBy(leds, NUM_LEDS, 20);
  for (byte i = 0; i < SPARKS_AM; i++) {
    setRegenRule(i, (sparksPos[0][i] >= (WIDTH - 1) * 10 || sparksPos[1][i] < 0 || sparksPos[1][i] >= (HEIGHT - 1) * 10 || sparksFade[i] < 20) ? 1 : 0);
    render(i, CHSV(0,0,sparksFade[i]));
  }
  //blur2d(leds, LED_COLS, LED_ROWS, 32);
  delay(16);
}