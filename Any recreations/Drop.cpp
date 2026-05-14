//// ----------------------------- Drop ------------------------------
//(c)stepko

#define Sat (255)
#define Hue (150)

#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

#define MaxRad WIDTH+HEIGHT

void fillAll(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}


void drawPixelXY(uint8_t x, uint8_t y, CRGB color)
{
  if (x < 0 || x > (WIDTH - 1) || y < 0 || y > (HEIGHT - 1)) return;
    leds[XY(x,y)] += color;}
    
void drawCircle(int x0, int y0, int radius, const CRGB &color){
  int a = radius, b = 0;
  int radiusError = 1 - a;

  if (radius == 0) {
    drawPixelXY(x0, y0, color);
    return;
  }
  while (a >= b)  {
    drawPixelXY(a + x0, b + y0, color);
    drawPixelXY(b + x0, a + y0, color);
    drawPixelXY(-a + x0, b + y0, color);
    drawPixelXY(-b + x0, a + y0, color);
    drawPixelXY(-a + x0, -b + y0, color);
    drawPixelXY(-b + x0, -a + y0, color);
    drawPixelXY(a + x0, -b + y0, color);
    drawPixelXY(b + x0, -a + y0, color);
    b++;
    if (radiusError < 0)
      radiusError += 2 * b + 1;
    else
    {
      a--;
      radiusError += 2 * (b - a + 1);
    }
  }
  }


bool loadingFlag = true;

CRGBPalette16 currentPalette(PartyColors_p);
uint8_t hue;

int rad[(HEIGHT+WIDTH)/8];
byte posx[(HEIGHT+WIDTH)/8],posy[(HEIGHT+WIDTH)/8];

void draw() {
  if (loadingFlag)
  {loadingFlag = false;
    hue = Hue;
      for (int i = 0; i < ((HEIGHT+WIDTH)/8)-1; i++)
  {
    posx[i]=random(WIDTH-1);
    posy[i]=random(HEIGHT-1);
    rad[i] = random(-1,MaxRad);
    }
  }
    fill_solid( currentPalette, 16, CHSV(hue,Sat,230));
    currentPalette[10] = CHSV(hue,Sat-60,255);
    currentPalette[9] = CHSV(hue,255-Sat,210);
    currentPalette[8] = CHSV(hue,255-Sat,210);
    currentPalette[7] = CHSV(hue,Sat-60,255);
    fillAll(ColorFromPalette(currentPalette,1));
     for (uint8_t i = ((HEIGHT+WIDTH)/8)-1; i > 0 ; i--){
    drawCircle(posx[i],posy[i], rad[i],ColorFromPalette(currentPalette,(256/16)*8.5-rad[i]));
    drawCircle(posx[i],posy[i], rad[i]-1,ColorFromPalette(currentPalette,(256/16)*7.5-rad[i]));
    if(rad[i] >= MaxRad){
    rad[i]=-1;
    posx[i]=random(WIDTH);
    posy[i]=random(HEIGHT);}
    else
    rad[i]++;
}
if(Hue == 0)
hue++;
blur2d(leds, WIDTH, HEIGHT, 64);
delay(16);}
