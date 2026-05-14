//Monster Face
//13.07.21
//@stepko
void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
  uint8_t V, uint8_t type) {
  int8_t deltaX = abs(x2 - x1);
  int8_t deltaY = abs(y2 - y1);
  int8_t signX = x1 < x2 ? 1 : -1;
  int8_t signY = y1 < y2 ? 1 : -1;
  int8_t error = deltaX - deltaY;
  
  switch (type) {
    case 0:
      leds[XY(x2 , y2)].r = V;
      break;
    case 1:
      leds[XY(x2 , y2)].g = V;
      break;
    case 2:
      leds[XY(x2 , y2)].b = V;
      break;
  }
  while (x1 != x2 || y1 != y2) {
    switch (type) {
      case 0:
        leds[XY(x1 , y1)].r = V;
        break;
      case 1:
        leds[XY(x1 , y1)].g = V;
        break;
      case 2:
        leds[XY(x1 , y1)].b = V;
        break;
    }
    int8_t error2 = error * 2;
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
void drawCircle(int x0, int y0, int radius, uint8_t V, uint8_t type) {
  int a = radius, b = 0;
  int radiusError = 1 - a;
  
  if (radius == 0) {
    switch (type) {
      case 0:
        leds[XY(x0 , y0)].r = V;
        break;
      case 1:
        leds[XY(x0 , y0)].g = V;
        break;
      case 2:
        leds[XY(x0 , y0)].b = V;
        break;
    }
    return;
  }
  while (a >= b) {
    switch (type) {
      case 0:
        leds[XY(a + x0 , b + y0)].r = V;
        leds[XY(b + x0 , a + y0)].r = V;
        leds[XY(-a + x0 , b + y0)].r = V;
        leds[XY(-b + x0 , a + y0)].r = V;
        leds[XY(-a + x0 , -b + y0)].r = V;
        leds[XY(-b + x0 , -a + y0)].r = V;
        leds[XY(a + x0 , -b + y0)].r = V;
        leds[XY(b + x0 , -a + y0)].r = V;
        break;
      case 1:
        leds[XY(a + x0 , b + y0)].g = V;
        leds[XY(b + x0 , a + y0)].g = V;
        leds[XY(-a + x0 , b + y0)].g = V;
        leds[XY(-b + x0 , a + y0)].g = V;
        leds[XY(-a + x0 , -b + y0)].g = V;
        leds[XY(-b + x0 , -a + y0)].g = V;
        leds[XY(a + x0 , -b + y0)].g = V;
        leds[XY(b + x0 , -a + y0)].g = V;
        break;
      case 2:
        leds[XY(a + x0 , b + y0)].b = V;
        leds[XY(b + x0 , a + y0)].b = V;
        leds[XY(-a + x0 , b + y0)].b = V;
        leds[XY(-b + x0 , a + y0)].b = V;
        leds[XY(-a + x0 , -b + y0)].b = V;
        leds[XY(-b + x0 , -a + y0)].b = V;
        leds[XY(a + x0 , -b + y0)].b = V;
        leds[XY(b + x0 , -a + y0)].b = V;
        break;
    }
    b++;
    if (radiusError < 0)
      radiusError += 2 * b + 1;
    else {
      a--;
      radiusError += 2 * (b - a + 1);
    }
  }
}

#define X1 LED_COLS / 16
#define Y1 LED_ROWS / 16
#define X2 LED_COLS / 8
#define Y2 LED_ROWS / 2 - 1
#define X3 LED_COLS / 4
#define Y3 LED_ROWS / 4
#define X4 LED_COLS / 3
#define Y4 LED_ROWS / 2
#define X5 LED_COLS / 2
#define Y5 LED_ROWS / 3

#define Yb LED_ROWS / 5
#define Ye LED_ROWS - LED_ROWS / 4

#define iX1 LED_COLS - LED_COLS / 16
#define iX2 LED_COLS - LED_COLS / 8
#define iX3 LED_COLS - LED_COLS / 4
#define iX4 LED_COLS - LED_COLS / 3
#define iX5 LED_COLS - LED_COLS / 2

void drawMouth(byte V, byte T, float OfX = 0, float OfY = 0) {
  drawLine(X1 + OfX, Y1 + OfY, X2 + OfX, Y2 + OfY, V, T);
  drawLine(X2 + OfX, Y2 + OfY, X3 + OfX, Y3 + OfY, V, T);
  drawLine(X3 + OfX, Y3 + OfY, X4 + OfX, Y4 + OfY, V, T);
  drawLine(X4 + OfX, Y4 + OfY, X5 + OfX, Y5 + OfY, V, T);
  
  drawLine(X1 + OfX, Y1 + OfY, X4+ OfX, Yb + OfY, V, T);
  drawLine(X4 + OfX, Yb + OfY, X5+ OfX, Y1 + OfY, V, T);
  drawLine(X5 + OfX, Y1 + OfY, iX5+ OfX, Y1 + OfY, V, T);
  drawLine(iX5 + OfX, Y1 + OfY, iX4+ OfX, Yb + OfY, V, T);
  drawLine(iX4 + OfX, Yb + OfY, iX1+ OfX, Y1 + OfY, V, T);
  
  drawLine(X5 + OfX, Y5 + OfY, iX5+ OfX, Y5 + OfY, V, T);
  drawLine(iX5 + OfX, Y5 + OfY, iX4+ OfX, Y4 + OfY, V, T);
  drawLine(iX4 + OfX, Y4 + OfY, iX3+ OfX, Y3 + OfY, V, T);
  drawLine(iX3 + OfX, Y3 + OfY, iX2+ OfX, Y2 + OfY, V, T);
  drawLine(iX2+ OfX, Y2 + OfY, iX1+ OfX, Y1 + OfY, V, T);
}
void drawEyes(byte V, byte T, float OfX = 0, float OfY = 0) {
  drawCircle(X3 + OfX, Ye + OfY, LED_COLS / 6, V, T);
  drawCircle(iX3 + OfX, Ye + OfY, LED_COLS / 6, V, T);
}
void draw() {
  unsigned long t = (float)millis()/100;
  FastLED.clear();
  drawMouth(255, 0, sin(t), cos(t));
  drawMouth(255, 1, sin(t*2), cos(t*2));
  drawMouth(255, 2, sin(t*1.5), cos(t*1.5));
  
  drawEyes(255, 0, sin(t), cos(t));
  drawEyes(255, 1, sin(t*2), cos(t*2));
  drawEyes(255, 2, sin(t*1.5), cos(t*1.5));
}