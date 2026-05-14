//// ----------------------------- Sending ------------------------------
//(c)stepko
//19.02.21
//https://github.com/AlexGyver/LEDcube/blob/master/CUBE_Gyver/CUBE_Gyver.ino
#define regime 1

bool sending, sendDirection;
bool loading = true;
byte selX,selY;
uint16_t timer;

void sendVoxels() { // ported by me
  if (loading) {
    FastLED.clear();
    for (uint8_t x = 0; x < LED_COLS; x++) {
      leds[XY(x, (random(1) == 1) ? LED_ROWS - 1 : 0)] = CHSV(0, 0, 255);
    }
    loading = false;
  }
  if (!sending) {
    selX = random(0, LED_ROWS);
    if (leds[XY(selX, 0)] == CHSV(0, 0, 255)) {
      selY = 0;
      sendDirection = 1;
    } else if (leds[XY(selX, LED_ROWS - 1)]) {
      selY = LED_ROWS - 1;
      sendDirection = 0;
    }
    sending = true;
  } else {
    if (sendDirection == 1) {
      selY ++;
      leds[XY(selX, selY)] = CHSV(0, 0, 255);
      leds[XY(selX, selY - 1)] = 0;
      if (selY == LED_ROWS - 1) {
        sending = false;
      }
    } else {
      selY --;
      leds[XY(selX, selY)] = CHSV(0, 0, 255);
      leds[XY(selX, selY + 1)] = 0;
      if (selY == 0) {
        sending = false;
      }
    }
  }
}

//for v2
//bool sending, sendDirection; bool loading = true;
//byte selX;
int PosY[LED_COLS];
byte speed = 5;

void wu_pixelY(uint8_t x, uint32_t y, CRGB * col) { //awesome wu_pixel procedure by reddit u/sutaburosu
  // extract the fractional parts and derive their inverses
   uint8_t yy = y & 0xff, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a, b)((uint8_t)(((a) * (b) + (a) + (b)) >> 8))
  uint8_t wu[2] = {
iy, yy
  };
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (int8_t i = 1; i >= 0; i--) {
    uint16_t xy = XY(x, (y >> 8) + ((i >> 1) & 1));
    leds[xy].r = qadd8(leds[xy].r, col -> r * wu[i] >> 8);
    leds[xy].g = qadd8(leds[xy].g, col -> g * wu[i] >> 8);
    leds[xy].b = qadd8(leds[xy].b, col -> b * wu[i] >> 8);
  }
}

void sendVoxelsV2() { // remade by me
  if (loading) {
    FastLED.clear();
    for (uint8_t i = 0; i < LED_COLS; i++) {
      PosY[i] = (random(2) == 1) ? (LED_ROWS - 1) * 256 : 0;
    }
    loading = false;
  }
  //blur2d(leds,LED_COLS,LED_ROWS,5);
  for (uint8_t i = 0; i < LED_COLS; i++) {
    CRGB color = CHSV(150, 255, 255);
    if(i == selX)
    wu_pixelY(i, PosY[i], &color);
    else
    leds[XY(i, PosY[i] / 256)] += color;
    if (!sending) {
      selX = random(0, LED_COLS);
      if (PosY[selX] == 0) {
        sendDirection = 1;
      } else if (PosY[selX] ==(LED_ROWS - 1) * 256) {
        sendDirection = 0;
      }
      sending = true;
    } else {
      if (sendDirection) {
        PosY[selX] += speed;
        if (PosY[selX] >= (LED_ROWS - 1) * 256) {
          PosY[selX] = (LED_ROWS - 1) * 256;
          sending = false;
        }
      } else {
        PosY[selX] -= speed;
        if (PosY[selX] <= 0) {
          PosY[selX] = 0;
          sending = false;
        }
      }
    }
  }
  blur2d(leds, LED_COLS, LED_ROWS, 35);
}

//Idea from Metaball
//Yaroslaw Turbin 20.07.2020
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/
//https://www.reddit.com/r/FastLED/comments/hv77xm/my_metaballs_implementation_i_have_not_tried_it/

byte dist (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)  {
int8_t a = y2-y1;
int8_t b = x2-x1; 
byte dist = 220 / sqrt16(a*a+b*b);
return dist;
}

void LavaSending() {
  if (loading) {
    FastLED.clear();
    for (uint8_t i = 0; i < LED_COLS; i++) {
      PosY[i] = (random(2) == 1) ? (LED_ROWS - 1) * 256 : 0;
    }
    loading = false;
  }
  for (uint8_t i = 0; i < LED_COLS; i++) {
    if (!sending) {
      selX = random(0, LED_COLS);
      if (PosY[selX] == 0) {
        sendDirection = 1;
      } else if (PosY[selX] ==(LED_ROWS - 1) * 256) {
        sendDirection = 0;
      }
      sending = true;
    } else {
      if (sendDirection) {
        PosY[selX] += speed;
        if (PosY[selX] >= (LED_ROWS - 1) * 256) {
          PosY[selX] = (LED_ROWS - 1) * 256;
          sending = false;
        }
      } else {
        PosY[selX] -= speed;
        if (PosY[selX] <= 0) {
          PosY[selX] = 0;
          sending = false;
        }
      }
    }
  }
  for (int i = 0; i < LED_COLS; i++)    {
    for (int j = 0; j < LED_ROWS; j++) {

      byte  sum =  dist(i, j, 0, PosY[0]/256)/2;
      for (uint8_t s = 1; s < LED_COLS; s++) {sum += dist(i, j, s, PosY[s]/256)/2;}
      
      nblend(leds[XY (i, j)],  ColorFromPalette(HeatColors_p , sum + 128), 22);

    }
  }
}

void draw() {
  if (regime == 1) sendVoxelsV2();
  else if (regime == 2) LavaSending();
    else sendVoxels();
    delay(16);
}