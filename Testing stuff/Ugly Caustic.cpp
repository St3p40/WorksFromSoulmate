#define scale 24
#define bri 128
#define speed 8

uint8_t noise[2][LED_COLS + 1][LED_ROWS + 1];
void wu_pixel(int16_t x, int16_t y) {
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
  #undef WU_WEIGHT
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t local_x = (x >> 8) + (i & 1);
    uint8_t local_y = (y >> 8) + ((i >> 1) & 1);
    if ((local_x >= 0 && local_x <= LED_COLS) && (local_y >= 0 && local_y <= LED_ROWS))
    noise[1][local_x][local_y] = constrain(qsub8(noise[1][local_x][local_y], (bri * wu[i]) >> 8), 15, 255);
  }
}
uint16_t x, y, z;
void fillNoise() {
  uint8_t dataSmoothing = 0;
  if (speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }
  for (uint8_t i = 0; i < LED_COLS + 1; i++) {
    int32_t ioffset = scale * i;
    for (uint8_t j = 0; j < LED_ROWS + 1; j++) {
      int32_t joffset = scale * j;
      
      uint8_t data = inoise8(x + ioffset, y + joffset, z);
      
      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));
      
      if (dataSmoothing) {
        uint8_t olddata = noise[0][i][j];
        uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
        data = newdata;
      }
      
      noise[0][i][j] = data;
    }
  }
  z += speed;
  
  // apply slow drift to X and Y, just for visual variation.
  x += speed >> 3;
  y -= speed >> 4;
}
void draw() {
  fillNoise();
  memset8(&noise[1][0][0],255,(LED_COLS+1)*(LED_ROWS+1));
  for (byte x = 0; x < LED_COLS; x++) {
    for (byte y = 0; y < LED_ROWS; y++) {
      uint8_t n0 = noise[0][x][y];
      uint8_t n1 = noise[0][x + 1][y];
      uint8_t n2 = noise[0][x][y + 1];
      int8_t xl = n0 - n1;
      int8_t yl = n0 - n2;
      int16_t xa = (x << 8) + ((xl * avg8(n0, n1)) >> 1);
      int16_t ya = (y << 8) + ((yl * avg8(n0, n2)) >> 1);
      wu_pixel(xa, ya);
    }
  }
  for (byte i = 0; i < LED_COLS; i++) {
    for (byte j = 0; j < LED_ROWS; j++) {
      nblend(leds[XY(i, j)], CHSV(150, noise[1][i][j], 255),64);
    }
  }
}