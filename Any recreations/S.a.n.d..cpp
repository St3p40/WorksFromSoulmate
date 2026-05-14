uint16_t speed = 196;
bool var = 0;

byte FF[LED_COLS][LED_ROWS];
byte SF[LED_COLS][LED_ROWS];
uint16_t shift;

void changeFrame() {
  memcpy(FF, SF, LED_COLS * LED_ROWS);
  bool checked = 0;
  for(byte x = 0; x < LED_COLS; x++){checked = 1; if(!SF[x][LED_ROWS/2-LED_ROWS/8]){checked = 0; break;}
   }
  if(checked){
    for (byte y = 0; y < LED_ROWS; y++) {
    for (byte x = 0; x < LED_COLS; x++) {
      if(random()%3) SF[x][y] = 0;
    }
  }
}
  for (byte y = 0; y < LED_ROWS; y++) {
    for (byte x = 0; x < LED_COLS; x++) {
      if (!SF[x][y]) continue;
      if (SF[x][y] && !SF[x][y - 1] && y) {SF[x][y - 1] = SF[x][y]; SF[x][y] = 0; }
      else if (SF[x][y] && SF[x][y - 1] && !SF[x + 1][y - 1]  && !(x==LED_COLS-1) && y) {SF[x + 1][y - 1] = SF[x][y]; SF[x][y] = 0; }
       else if (SF[x][y] && SF[x][y - 1] && !SF[x - 1][y - 1] && x && y) {SF[x - 1][y - 1] = SF[x][y]; SF[x][y] = 0; }
    }
  }
  if (!(random() % 4)) SF[(var)?random(1,LED_COLS-1):LED_COLS/2][LED_ROWS - 1] = random(10, 255);
}
void draw() {
  shift += speed;
  if (shift >= 255) {
    changeFrame();
    shift%=256;
  }
  for (byte x = 0; x < LED_COLS; x++) {
    for (byte y = 0; y < LED_ROWS; y++) {
      CRGB col1 = CHSV(FF[x][y], 255, (FF[x][y]) ? 255 : 0);
      leds[XY(x, y)] = nblend(col1, CHSV(SF[x][y], 255, (SF[x][y]) ? 255 : 0), shift);
    }
  }
}