//Minesweeper
//@st3p40(aka stepko)
//05.12.25

const uint8_t mines_am = float(NUM_LEDS) / 100.0 * 12.0;

struct coord {
  int16_t x;
  int16_t y;
}
mine[mines_am];
bool game_over = 0;

static
const CRGB coordColors[9] = {
  CRGB(32, 32, 32),
  CRGB(0, 0, 255),
  CRGB(0, 128, 0),
  CRGB(255, 0, 0),
  CRGB(0, 0, 128),
  CRGB(128, 0, 0),
  CRGB(0, 128, 128),
  CRGB(0, 0, 32),
  CRGB(128, 128, 128)
};

void animate_pix(coord crd, CRGB clr, uint8_t spd) {
  CRGB temp = leds[XY(crd.x, crd.y)];
  for (uint16_t i = 0; i < 256; i += spd) {
    leds[XY(crd.x, crd.y)] = blend(temp, clr, i);
    FastLED.show();
  }
}

/*void check_dot(coord crd) {
  if (crd.x >= 0 && crd.y >= 0 && crd.x < LED_COLS && crd.y < LED_ROWS && !leds[XY(crd.x, crd.y)]) {
    uint8_t neighbours = 0;
    for (uint8_t i = 0; i < mines_am; i++) {
      if (crd.x == mine[i].x && crd.y == mine[i].y) {
        animate_pix(crd, CRGB(255, 255, 255), 16);
        game_over = 1;
        return;
      }
      if (abs(crd.x - mine[i].x) <= 1 &&
        abs(crd.y - mine[i].y) <= 1) {
        neighbours++;
      }
    }
    
    if (!game_over) {
      if (neighbours)
        animate_pix(crd, coordColors[neighbours], 16);
      else
        animate_pix(crd, coordColors[neighbours], 127);
    }
    
    if (!neighbours) {
      for (int8_t dx = -1; dx <= 1; dx++) {
        for (int8_t dy = -1; dy <= 1; dy++) {
          if (dx || dy) {
            check_dot({ crd.x + dx, crd.y + dy });
          }
        }
      }
    }
  }
}*/ //recursive variant

int check_this_dot(coord crd){
        uint8_t neighbours = 0;
    for (uint8_t i = 0; i < mines_am; i++) {
      if (crd.x == mine[i].x && crd.y == mine[i].y) {
        animate_pix(crd, CRGB(255, 255, 255), 16);
        game_over = 1;
        return;
      }
      if (abs(crd.x - mine[i].x) <= 1 &&
        abs(crd.y - mine[i].y) <= 1) {
        neighbours++;
      }
    }
    
    if (!game_over) {
      if (neighbours)
        animate_pix(crd, coordColors[neighbours], 16);
      else
        animate_pix(crd, coordColors[neighbours], 127);
    }
    return neighbours;
}

void check_dot(coord start) {
    coord stack[NUM_LEDS];
    int sp = 0;

    stack[sp++] = start;

    while (sp > 0 && !game_over) {
        coord crd = stack[--sp];
        if (leds[XY(crd.x, crd.y)])
            continue;

        int neighbours = check_this_dot(crd);
        if (game_over) return;

        if (neighbours > 0)
            continue;
        coord n;

        n = {crd.x+1, crd.y};
        if (n.x >= 0 && n.y >= 0 && n.x < LED_COLS && n.y < LED_ROWS)
            stack[sp++] = n;

        n = {crd.x-1, crd.y};
        if (n.x >= 0 && n.y >= 0 && n.x < LED_COLS && n.y < LED_ROWS)
            stack[sp++] = n;

        n = {crd.x, crd.y+1};
        if (n.x >= 0 && n.y >= 0 && n.x < LED_COLS && n.y < LED_ROWS)
            stack[sp++] = n;

        n = {crd.x, crd.y-1};
        if (n.x >= 0 && n.y >= 0 && n.x < LED_COLS && n.y < LED_ROWS)
            stack[sp++] = n;
    }
}
void draw() {
  if (game_over) {
    for (uint8_t j = 0; j < 32; j++) {
      for (uint8_t i = 0; i < mines_am; i++) {
        leds[XY(mine[i].x, mine[i].y)] += CRGB(8, 4, 0);
      }
      FastLED.show();
    }
    for (uint8_t j = 0; j < 32; j++) {
      blur2d(leds, LED_COLS, LED_ROWS, 64);
      FastLED.show();
    }
  }
  FastLED.clear();
  game_over = 0;
  for (uint8_t i = 0; i < mines_am; i++) {
    mine[i].x = random() % LED_COLS;
    mine[i].y = random() % LED_ROWS;
    while (leds[XY(mine[i].x, mine[i].y)]) {
      mine[i].x = random() % LED_COLS;
      mine[i].y = random() % LED_ROWS;
      leds[XY(mine[i].x, mine[i].y)] = CRGB(255, 0, 0);
    }
  }
  //FastLED.clear();
  do {
    coord choose;
    choose.x = random() % LED_COLS;
    choose.y = random() % LED_ROWS;
    while (leds[XY(choose.x, choose.y)]) {
      choose.x = random() % LED_COLS;
      choose.y = random() % LED_ROWS;
    };
    check_dot(choose);
  }
  while (!game_over);
}