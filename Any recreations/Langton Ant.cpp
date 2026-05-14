#define n (LED_COLS+LED_ROWS)/8
#define restart 60 // sec
bool setUp = true;
int8_t pos[2][n];
byte dir[n];
byte hue;
bool fade = false;

void move(byte id) {
  switch (dir[id]) {
    case 0:
      pos[1][id]++;
      break; //up
    case 1:
      pos[0][id]++;
      break; //right
    case 2:
      pos[1][id]--;
      break; //down
    case 3:
      pos[0][id]--;
      break; //left
  }
}
void check2(byte id) {
  if (leds[XY((pos[0][id]) % LED_COLS,(pos[1][id]) % LED_ROWS)] == CRGB(0,0,0)) dir[id]++;
  else dir[id]--;
  if (dir[id] > 3) dir[id] = 0;
  else if (dir[id] < 0) dir[id] = 3;
}
void check1(byte id) {
  if (leds[XY((pos[0][id]) % LED_COLS, pos[1][id] % LED_ROWS)] == CRGB(0,0,0)) leds[XY((pos[0][id]) % LED_COLS, (pos[1][id]) % LED_ROWS)] = CHSV(hue, 255, 255);
  else leds[XY((pos[0][id]) % LED_COLS, (pos[1][id]) % LED_ROWS)] = CRGB(0,0,0);
}
void draw() {
  if (setUp) {
    setUp = false;
    FastLED.clear();
    for (byte i = 0; i < n; i++) {
      pos[0][i] = random(0, LED_COLS-1);
      pos[1][i] = random(0, LED_ROWS-1);
      dir[i] = random(0, 3);
    }
  }
  for (byte i = 0; i < n; i++) {
    check1(i);
    move(i);
    check2(i);
  }
  hue++;
  if (fade) fadeToBlackBy(leds, NUM_LEDS, 5);
  if(restart){EVERY_N_SECONDS(restart) {
    setUp = true;
  }}
}