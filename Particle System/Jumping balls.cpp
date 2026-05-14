//// -----------------------------  JumpingBalls(Particle sys) ------------------------------
//(c)stepko
//19.08.21
#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

#define SPARKS_AM WIDTH
float FADE_KOEF = 10;
float SpeedK = .98;
float SpeedDecX = .01;
float SpeedDecY = 0;
#define Board 1
struct{
bool GravityX, GravityY;
int16_t genPosX, genPosY;
int16_t gravityPosX, gravityPosY;

void setGenPos(int16_t x, int16_t y) {
  genPosX = x;
  genPosY = y;
}

void setGravityPos(int16_t x, int16_t y) {
  GravityX = 1; GravityY = 1;
  gravityPosX = x;
  gravityPosY = y;
}

void setGravityPos(int16_t x, bool y) {
  if(y){
  gravityPosX = x;
  GravityX = 1; GravityY = 0;
  }
  else{
  gravityPosY = y;
  GravityX = 0; GravityY = 1;
  }
}

}PS_Var;

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

struct{
int16_t PosX, PosY;
float SpeedX, SpeedY;
float Fade;
byte Color;

void reg() {
  PosX = PS_Var.genPosX;
  PosY = PS_Var.genPosY;
  SpeedX = random(-10, 10);
  SpeedY = random(-10, 10);
  Fade = 255;
  Color = random();
}

void phisics() {
  if (SpeedK) {
    if (PS_Var.GravityX) {
      if (PS_Var.gravityPosX < PosX)
        SpeedX -= SpeedK;
      else
        SpeedX += SpeedK;
    }
    if (PS_Var.GravityY) {
      if (PS_Var.gravityPosY < PosY)
        SpeedY -= SpeedK;
      else
        SpeedY += SpeedK;
    }
  }
  Fade -= (255. / (float)((HEIGHT + WIDTH) * FADE_KOEF));
  if (SpeedDecX || SpeedX) {
    if (SpeedX > 0)
      SpeedX -= SpeedDecX;
    else
      SpeedX += SpeedDecX;
    if (abs(SpeedX) <= SpeedDecX)
      SpeedX = 0;
  }
  if (SpeedDecY || SpeedY) {
    if (SpeedY > 0)
      SpeedY -= SpeedDecY;
    else
      SpeedY += SpeedDecY;
    if (abs(SpeedY) <= SpeedDecY)
      SpeedX = 0;
  }
  if (Board) { if (PosX < 0 || PosX >= WIDTH * 10) SpeedX = -SpeedX; if (PosY < 0) SpeedY = -SpeedY; }
    PosX += /*constrain(*/SpeedX/*, -10, 10)*/;
    PosY += /*constrain(*/SpeedY/*, -10, 10)*/;
}

void render(CRGB Col) {
  phisics();
  if (PosX < ((HEIGHT - 1) * 10) && PosY >= 0)
    if (PosX < ((WIDTH - 1) * 10) && PosX >= 0) {
      CRGB color = Col;
      wu_pixel(PosX * 25.6, PosY * 25.6, & color);
    }
}

void setRegenRule(bool b) {
  if (b) reg();
}
}dot[SPARKS_AM];

bool run = true;

bool loadingFlag = true;

void draw() {
  PS_Var.setGenPos(beatsin16(10, 0, WIDTH * 10), beatsin16(10, 0, HEIGHT * 10, 0, 16384));
  if (loadingFlag) {
    PS_Var.setGravityPos(0, false);
    for (byte i = 0; i < SPARKS_AM; i++) {
      dot[i].reg();
      dot[i].Fade = random();
      for (byte a = 0; a < i; a++) {
        dot[i].phisics();
      }
    }
    loadingFlag = false;
  }
  FastLED.clear();
  for (byte i = 0; i < SPARKS_AM; i++) {
    dot[i].setRegenRule((dot[i].Fade <= 35) ? 1 : 0);
    dot[i].render(CHSV(dot[i].Color, 255, constrain(dot[i].Fade, 32, 255)));
  }
  delay(16);
}