//// ----------------------------- PoolNoise ------------------------------
//(c)stepko
//Water...
//16.10.20
//slowed by Elliot Kember https://editor.soulmatelights.com/gallery/521
bool loadingFlag = true;
#define Sat 255
#define Hue 150
#define Scale 40
#define Bri (0)

CRGBPalette16 currentPalette(PartyColors_p);
uint8_t hue;

void N(){
   for (byte y = 0; y < LED_ROWS; y++) {
        for (byte x = 0; x < LED_COLS; x++) {
          uint8_t pixelHue8 = inoise8 (x*Scale,y*Scale,millis()/16);
 leds[XY(x, y)] = ColorFromPalette(currentPalette, pixelHue8);}
   }blur2d(leds,LED_COLS, LED_ROWS, 32 );}


void draw() {
   if (loadingFlag)
  {loadingFlag = false;
  hue = Hue;
  }fill_solid( currentPalette, 16, CHSV(hue,Sat,230));
    currentPalette[9] = CHSV(hue,Sat-60,255);
    currentPalette[8] = CHSV(hue,255-Sat,210);
    currentPalette[7] = CHSV(hue,255-Sat,210);
    currentPalette[6] = CHSV(hue,Sat-60,255);
  N();
  delay(16);}

/*
  void fill_circle(uint8_t cx, uint8_t cy, uint8_t radius, CRGB col, CRGB col1) {
  for (int16_t y = -radius; y <= radius; y++) {
    for (int16_t x = -radius; x <= radius; x++) {
      if (x * x + y * y < radius * radius)
        leds[XY(cx + x, cy + y)] = col;
        if ((x * x + y * y >= (radius * radius)) && ((x * x + y * y <= (radius * radius) + radius)))
        leds[XY(cx + x, cy + y)] = col1;
    }
  }
}
void fillscreen(CRGB col){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i]=col;
  }
}

void draw(){
  fillscreen(CRGB(255,255,255));
  for(byte i = 1; i < max(LED_COLS,LED_ROWS) + 1; i++){fill_circle(beatsin8(15 + i*0.9,0,LED_COLS-1,0,~(i)*20),beatsin8(10 + i*0.12,0,LED_ROWS-1,0,64+i*20),min(LED_COLS,LED_ROWS)/4,CHSV(Hue,Sat,255),CRGB(255,255,255));
  }blur2d(leds,LED_COLS, LED_ROWS, 32 );delay(16);
}*/