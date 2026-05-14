//Soap
//@Stepko
//Idea from https://www.youtube.com/watch?v=DiHBgITrZck&ab_channel=StefanPetrick
//01.09.22
int8_t zD;
int8_t zF;
uint8_t noise3d[LED_COLS][LED_ROWS];
uint32_t noise32_x;
uint32_t noise32_y;
uint32_t noise32_z;
uint32_t scale32_x;
uint32_t scale32_y;
bool isSetup = 1;
uint8_t noisesmooth;

void FillNoise() {
  for (uint8_t i = 0; i < LED_COLS; i++) {
    int32_t ioffset = scale32_x * (i - LED_COLS / 2);
    for (uint8_t j = 0; j < LED_ROWS; j++) {
      int32_t joffset = scale32_y * (j - LED_ROWS / 2);
      int8_t data = inoise16(noise32_x + ioffset, noise32_y + joffset, noise32_z) >> 8;
      int8_t olddata = noise3d[i][j];
      int8_t newdata = scale8(olddata, noisesmooth) + scale8(data, 255 - noisesmooth);
      data = newdata;
      noise3d[i][j] = data;
    }
  }
}


void MoveFractionalNoiseX(int8_t amplitude = 1, float shift = 0) {
  CRGB ledsbuff[LED_COLS];
  for (uint8_t y = 0; y < LED_ROWS; y++) {
    int16_t amount = ((int16_t) noise3d[0][y] - 128) * 2 * amplitude + shift * 256;
    int8_t delta = abs(amount) >> 8;
    int8_t fraction = abs(amount) & 255;
    for (uint8_t x = 0; x < LED_COLS; x++) {
      if (amount < 0) {
        zD = x - delta;
        zF = zD - 1;
      } else {
        zD = x + delta;
        zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black;
      if ((zD >= 0) && (zD < LED_COLS)) PixelA = leds[XY(zD, y)]; else PixelA = CHSV(~noise3d[abs(zD)%LED_COLS][y]*3,255,255);
      CRGB PixelB = CRGB::Black;
      if ((zF >= 0) && (zF < LED_COLS)) PixelB = leds[XY(zF, y)]; else PixelB = CHSV(~noise3d[abs(zF)%LED_COLS][y]*3,255,255);
      ledsbuff[x] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction))); // lerp8by8(PixelA, PixelB, fraction );
    }
    for (uint8_t x = 0; x < LED_COLS; x++) {
      leds[XY(x, y)] = ledsbuff[x];
    }
  }
}

void MoveFractionalNoiseY(int8_t amplitude = 1, float shift = 0) {
  CRGB ledsbuff[LED_ROWS];
  for (uint8_t x = 0; x < LED_COLS; x++) {
    int16_t amount = ((int16_t) noise3d[x][0] - 128) * 2 * amplitude + shift * 256;
    int8_t delta = abs(amount) >> 8;
    int8_t fraction = abs(amount) & 255;
    for (uint8_t y = 0; y < LED_ROWS; y++) {
      if (amount < 0) {
        zD = y - delta;
        zF = zD - 1;
      } else {
        zD = y + delta;
        zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black;
      if ((zD >= 0) && (zD < LED_ROWS)) PixelA = leds[XY(x, zD)]; else PixelA = CHSV(~noise3d[x][abs(zD)%LED_ROWS]*3,255,255); 
      CRGB PixelB = CRGB::Black;
      if ((zF >= 0) && (zF < LED_ROWS))PixelB = leds[XY(x, zF)];  else PixelB = CHSV(~noise3d[x][abs(zF)%LED_ROWS]*3,255,255);
      ledsbuff[y] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    }
    for (uint8_t y = 0; y < LED_ROWS; y++) {
      leds[XY(x, y)] = ledsbuff[y];
    }
  }
}
uint16_t mov = max(LED_COLS,LED_ROWS)*47;
void draw() {
  if (isSetup) {
  noise32_x = random16();
  noise32_y = random16();
  noise32_z = random16();
    scale32_x = 160000/LED_COLS;
    scale32_y = 160000/LED_ROWS;
    FillNoise();
    for (byte i = 0; i < LED_COLS; i++) {
      for (byte j = 0; j < LED_ROWS; j++) {
        leds[XY(i, j)] = CHSV(~noise3d[i][j]*3,255,255);
      }
    }
    isSetup = 0;
  }
  noise32_x += mov;
  noise32_y += mov;
  noise32_z += mov;
  FillNoise();
  MoveFractionalNoiseX(LED_COLS/8);
  MoveFractionalNoiseY(LED_ROWS/8);
}