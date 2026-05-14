//Puzzles
//@stepko
//06.11.21
#define PSizeX 4
#define PSizeY 4
#define SubPixel 1
const uint32_t Ecols = (LED_COLS / PSizeX);
const uint32_t Erows = (LED_ROWS / PSizeY);
const bool Ca = (LED_COLS % PSizeX)? 1 : 0;
const bool Ra = (LED_ROWS % PSizeY)? 1 : 0;
const int PCols = round(Ecols) + Ca;
const int PRows = round(Erows) + Ra;

byte puzzle[PCols][PRows];
byte tmpp;
byte z_dot[2];
byte etap;
int8_t move[2];
#define shspeed 64 //0.25
int shift[2] = { 0, 0 };
bool XorY;

wu_pixel(uint32_t x, uint32_t y, CRGB col) {      //awesome wu_pixel procedure by reddit u/sutaburosu
    // extract the fractional parts and derive their inverses
    uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
    // calculate the intensities for each affected pixel
    #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
    uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                    WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
    // multiply the intensities by the colour, and saturating-add them to the pixels
    for (uint8_t i = 0; i < 4; i++) {
      uint16_t xn = (x >> 8) + (i & 1); uint16_t yn = (y >> 8) + ((i >> 1) & 1);
      CRGB clr = leds[XY(xn, yn)];
      clr.r = qadd8(clr.r, (col.r * wu[i]) >> 8);
      clr.g = qadd8(clr.g, (col.g * wu[i]) >> 8);
      clr.b = qadd8(clr.b, (col.b * wu[i]) >> 8);

      leds[XY(xn, yn)] = clr;
    }
    #undef WU_WEIGHT
}

void draw_square(byte x1, byte y1, byte x2, byte y2, byte col) {
  for (byte x = x1; x < x2; x++) {
    for (byte y = y1; y < y2; y++) {
      if (col == 0) { leds[XY(x, y)] = CRGB(0, 0, 0); } else if ((x == x1 || x == x2 - 1) || (y == y1 || y == y2 - 1))
        leds[XY(x, y)] = ColorFromPalette(HeatColors_p, col - 16);
      else leds[XY(x, y)] = ColorFromPalette(HeatColors_p, col);
    }
  }
}

void draw_wu_square(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, byte col) {
  uint32_t Lx = x2 - x1;
  uint32_t Ly = y2 - y1;
  for (uint32_t x = 0; x < Lx; x+=256) {
    for (uint32_t y = 0; y < Ly; y+=256) {
      if (col == 0) { leds[XY(x, y)] = CRGB(0, 0, 0); } else if ((x == 0 || x == Lx - 256) || (y == 0 || y == Ly - 256))
        wu_pixel(x1 + x, y1 + y, ColorFromPalette(HeatColors_p, col - 16));
      else wu_pixel(x1 + x, y1 + y, ColorFromPalette(HeatColors_p, col));
    }
  }
}

bool setup = true;
void draw() {
  if (setup) {
    etap = 0;
    for (byte x = 0; x < PCols; x++) {
      for (byte y = 0; y < PRows; y++) { puzzle[x][y] = random8(16, 255); }
    }
    z_dot[0] = random(0, PCols);
    z_dot[1] = random(0, PRows);
    puzzle[z_dot[0]][z_dot[1]] = 0;
    setup = false;
  }
  
  for (byte x = 0; x < PCols; x++) {
    for (byte y = 0; y < PRows; y++) {
      draw_square(x * PSizeX, y * PSizeY, (x + 1) * PSizeX, (y + 1) * PSizeY, puzzle[x][y]);
    }
  }
  switch (etap) {
    case 0:
      XorY = !XorY;
      if (XorY) {
        if (z_dot[0] == PCols - 1)
          move[0] = -1;
        else if (z_dot[0] == 0) move[0] = 1;
        else move[0] = (move[0] == 0) ? (random() % 2) * 2 - 1 : move[0];
      } else {
        if (z_dot[1] == PRows - 1)
          move[1] = -1;
        else if (z_dot[1] == 0) move[1] = 1;
        else move[1] = (move[1] == 0) ? (random() % 2) * 2 - 1 : move[1];
      }
      move[(XorY) ? 1 : 0] = 0;
      etap = 1;
      break;
    case 1:
      tmpp = puzzle[z_dot[0] + move[0]][z_dot[1] + move[1]];
      puzzle[z_dot[0] + move[0]][z_dot[1] + move[1]] = 0;
      etap = 2;
      break;
    case 2:
    if(SubPixel){
      draw_wu_square((((z_dot[0] + move[0]) * PSizeX) << 8) + shift[0], (((z_dot[1] + move[1]) * PSizeY) << 8) + shift[1], (((z_dot[0] + move[0] + 1) * PSizeX) << 8) + shift[0], (((z_dot[1] + move[1] + 1) * PSizeY) << 8) + shift[1], tmpp);
    }
    else{
      draw_square(((z_dot[0] + move[0]) * PSizeX) + (shift[0] >> 8), ((z_dot[1] + move[1]) * PSizeY) + (shift[1] >> 8), ((z_dot[0] + move[0] + 1) * PSizeX) + (shift[0] >> 8), (z_dot[1] + move[1] + 1) * PSizeY + (shift[1] >> 8), tmpp);
    }
      shift[0] -= (move[0] * shspeed);
      shift[1] -= (move[1] * shspeed);
      if ((abs(shift[0]) >= (LED_COLS / PCols) << 8) || (abs(shift[1]) >= (LED_ROWS / PRows) << 8)) {
        shift[0] = 0;
        shift[1] = 0;
        puzzle[z_dot[0]][z_dot[1]] = tmpp;
        etap = 3;
      }
      break;
    case 3:
      z_dot[0] += move[0];
      z_dot[1] += move[1];
      etap = 0;
      break;
  }
}