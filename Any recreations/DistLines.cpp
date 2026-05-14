// by @stepko


#define COUNTS 3
#define SPEED 1 // 1-255

const float R = (float)NUM_LEDS/128;

const float dXc = 255.0f/(float)LED_COLS;
const float dYc = 255.0f/(float)LED_ROWS;

float dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  byte dist;
  int a = y2 - y1;
  int b = x2 - x1;
  a *= a;
  b *= b;
  a += b;
  dist = sqrt(a);
  return dist;
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
    leds[XY(xn, yn)] = clr;
  }
  #undef WU_WEIGHT
}

void fill_circleF(float cx, float cy, float radius, CRGB col) {
  uint8_t rad = radius;
  for (float y = -radius; y < radius; y += fabs(y) < rad ? 1 : 0.2) {
    for (float x = -radius; x < radius; x += fabs(x) < rad ? 1 : 0.2) {
      if (x * x + y * y < radius * radius)
        drawPixelXYF(cx + x, cy + y, col);
    }
  }
}


static float fmap(const float x,
  const float in_min,
    const float in_max,
      const float out_min,
        const float out_max) {
  return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}

void drawLineF(float x1, float y1, float x2, float y2,
  const CRGB & col1,
    const CRGB & col2) {
  float deltaX = fabs(x2 - x1);
  float deltaY = fabs(y2 - y1);
  float steps = 255 / max(deltaX, deltaY);
  float error = deltaX - deltaY;
  CRGB col = col1;
  float signX = x1 < x2 ? 0.5 : -0.5;
  float signY = y1 < y2 ? 0.5 : -0.5;
  
  while (x1 != x2 || y1 != y2) {
    if ((signX > 0. && x1 > x2 + signX) || (signX < 0. && x1 < x2 + signX)) break;
    if ((signY > 0. && y1 > y2 + signY) || (signY < 0. && y1 < y2 + signY)) break;
    drawPixelXYF(x1/*+random(-10,10)*0.1 */, y1/*+random(-10,10)*0.1*/, nblend(col, col2, steps++));
    float error2 = error;
    if (error2 > -deltaY) {
      error -= deltaY;
      x1 += signX;
    }
    if (error2 < deltaX) {
      error += deltaX;
      y1 += signY;
    }
  }
}
float x[8];
float y[8];
uint8_t iniX[8];
uint8_t iniY[8];

bool firstStart = true;
float speedFactor;

byte number = COUNTS;

uint8_t Noise(double x=0,double y=0,double z=0){
  uint8_t N = qadd8(qsub8(inoise8(x,y,z),16),scale8(qsub8(inoise8(x,y,z),16),39));
  return N;
}

void draw() {
  static double t;
  if(firstStart) {
    firstStart = false;
    for (uint8_t i = 0; i < 8; i++) {
      iniX[i] = random(0, 8);
      iniY[i] = random(1, 9);
    }
  }
  t=(double)millis()*speedFactor;
  fadeToBlackBy(leds, NUM_LEDS, 192);;
  static byte hue;
  hue++;
  speedFactor = fmap(SPEED, 1, 255, 0.05, 1.5);
  for (byte i = 0; i < COUNTS; i++) {
  x[i] = (float) Noise(t+i*100,i*200,i*1000)/dXc;
  y[i] = (float) Noise(i*200,t+i*100,i*200)/dYc;
    for (byte j = i; j < COUNTS; j++) {
      byte a = dist(byte(x[i]), byte(y[i]), byte(x[j]), byte(y[j]));
      if ((i != j) & (a <= float(LED_COLS/2))) {
        drawLineF(x[i], y[i], x[j], y[j],CHSV(hue+(i*256/COUNTS), 255, 256 - 256/LED_ROWS * fabs(float(LED_ROWS/2) - y[i])),CHSV(hue+(j*256/COUNTS), 255, 256 - 256/LED_ROWS * fabs(float(LED_ROWS/2) - y[j])));                      
      }
      if (a == 0 and number == COUNTS) {
        number = i;   
      }
    }
    fill_circleF(x[i], y[i], (float)Noise(i*300,i*150,t+i*200)/64.0f, CHSV(hue+(i*256/COUNTS), 255, 256 - 256/LED_ROWS * fabs(float(LED_ROWS/2) - y[i])));
  }  
  EVERY_N_SECONDS(2) {
    if (number < COUNTS) {
      iniX[number] += random(-1, 2);
      //iniY[number] += random(-1, 2); 
    }
    number = COUNTS;
  }
  blur2d(leds, LED_COLS, LED_ROWS, 32); 
  delay(16);
}

