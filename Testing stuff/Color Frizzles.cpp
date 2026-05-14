// Matrix size
#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

//// ----------------------------- Color Frizzles ------------------------------
//(c)stepko
//04.02.21

void draw() {
  fadeToBlackBy(leds, NUM_LEDS, 16);
  for(byte i=8; i > 0;i--){leds[XY(beatsin8(12+i,0,WIDTH-1),beatsin8(15-i,0,HEIGHT-1))]+=CHSV(beatsin8(12,0,255),255,255);
  if(LED_COLS >24 || LED_ROWS >24){
    leds[XY(beatsin8(12+i,0,WIDTH-1)+1,beatsin8(15-i,0,HEIGHT-1))]+=CHSV(beatsin8(12,0,255),255,255);
    leds[XY(beatsin8(12+i,0,WIDTH-1)-1,beatsin8(15-i,0,HEIGHT-1))]+=CHSV(beatsin8(12,0,255),255,255);
    leds[XY(beatsin8(12+i,0,WIDTH-1),beatsin8(15-i,0,HEIGHT-1)+1)]+=CHSV(beatsin8(12,0,255),255,255);
    leds[XY(beatsin8(12+i,0,WIDTH-1),beatsin8(15-i,0,HEIGHT-1)-1)]+=CHSV(beatsin8(12,0,255),255,255);
  }
  }blur2d(leds,WIDTH,HEIGHT,16);
  delay(16);
} //loop