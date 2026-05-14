#define scale 16
float bX = (random() * random()) / 255, bY = random() % 200;
float bsX = 0, bsY = -25;
uint16_t bsx[max(LED_COLS,LED_ROWS)], bsy[max(LED_COLS,LED_ROWS)];
uint32_t t = 0;
bool start = 1;
float dist(float x1, float y1, float x2, float y2) {
  return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}
void drawPixelXYF(float x, float y, CRGB color) {
  if (x < 0 || y < 0 || x > ((float) LED_COLS - 1) || y > ((float) LED_ROWS - 1)) return;
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
}
void draw() {
  t++;
  uint16_t nd=t/4;
  if (start) {
    while (inoise8(bX + float(LED_COLS / 2. - 0.5) * scale, bY + float(LED_ROWS / 2. - 0.5) * scale,nd) >= 150) { bY++; }
    start = 1;
  }
  bX += bsX;
  bY += bsY;
  //bsY -= 0.98;
  if (inoise8(bX + float(LED_COLS / 2. - 0.5) * scale, bY + float(LED_ROWS / 2. - 0.5) * scale,nd) >= 150) {
    bX -= bsX;
    bY -= bsY;
   float XsX, YsY;
    float max[3] = { 0, 0, 0 };
    for (uint16_t i = 0; i < 360; i+=9) {
      float rad = radians(i);
      XsX = cos(rad);
      YsY = sin(rad);
       uint8_t nois =inoise8((bX + float(LED_COLS / 2. - 0.5) * scale) + scale*XsX, (bY + float(LED_ROWS / 2. - 0.5) * scale) + scale*YsY,nd);
      if (max[2] < nois) {
        rad = radians((i+180)%360);
        max[0] = cos(rad);
        max[1] = sin(rad);
        max[2] = nois;
      }
    }
    float bsXY = dist(0,0,bsX, bsY);
    bsY = max[1]*bsXY;
    bsX = max[0]*bsXY;
  }
  for(byte i = 1; i < max(LED_COLS,LED_ROWS)-1; i++){
    bsx[i-1]=bsx[i]; bsy[i-1]=bsy[i];}
  bsx[max(LED_COLS,LED_ROWS)-1]=bX-bsX; bsy[max(LED_COLS,LED_ROWS)-1]=bY-bsY;
  //fadeToBlackBy(leds, NUM_LEDS, 32);
  for (byte x = 0; x < LED_COLS; x++) {
    for (byte y = 0; y < LED_ROWS; y++) {
      uint8_t nois = inoise8(bX + x * scale, bY + y * scale, nd);
      leds[XY(x, y)] = (nois >= 150) ? CHSV(nois, 255, constrain(nois * 1.2,0,255)) : CRGB(0,0,0); //leds[XY(x, y)];
  }
}
//leds[XY(LED_COLS / 2-bsX/scale, LED_ROWS / 2-bsY/scale)] = CHSV(0, 0, 255);
//for(byte i = 1; i < max(LED_COLS,LED_ROWS)-1; i++){
 //drawPixelXYF((float(LED_COLS/2-0.5)*scale+bX-bsx[i])/scale, (float(LED_ROWS/2-0.5)*scale+bY-bsy[i])/scale, CHSV(0,0,(max(LED_COLS,LED_ROWS)-1-i)*16));}
drawPixelXYF(float(LED_COLS/2-0.5), float(LED_ROWS/2-0.5), CHSV(0,0,255));
//blur2d(leds, LED_COLS, LED_ROWS, 32);
}