#define WIDTH LED_COLS
#define HEIGHT LED_ROWS

//// ----------------------------- Torch ------------------------------
//(c)stepko

#define HitPoints 0
#define Color 40
#define MAP_W WIDTH * 2
#define MAP_H HEIGHT * 2
#define ConstWR 256 / WIDTH
#define ConstHR 256 / WIDTH


bool act, loadingFlag = true;
uint16_t lightersPosX, lightersPosY, alpha;
uint8_t lightersSpeed, PosX, PosY, CenterX, CenterY, period;
float angle;
int8_t rot;

#define POV 75
#define PoV POV / 2
#define povAngle 1
bool mape[WIDTH][HEIGHT];

void draw() {
  if (loadingFlag) {
    lightersPosX = random(0, MAP_W * 10);
    lightersPosY = random(0, MAP_H * 10);
    alpha = 0;
    CenterX = (1+sin(radians(alpha))) * WIDTH;
    CenterY = (1+cos(radians(alpha))) * HEIGHT;
    rot = random(-10, 10);
    act = true;
    period = 2;
    lightersSpeed = 0;
    for (byte i = 0; i < MAP_H * MAP_W / 64; i++) {
      mape[random(0, WIDTH - 1)][random(0, HEIGHT - 1)] = 1;
    }
    loadingFlag = false;
  }
  FastLED.clear();
  if (!act) {
    alpha += rot;
    CenterX = sin8(map(alpha % 360, 0, 360, 0, 255)) / (ConstWR);
    CenterY = cos8(map(alpha % 360, 0, 360, 0, 255)) / (ConstHR);
    angle = radians(alpha);
  } else {
    lightersPosX += sin(angle) * lightersSpeed;
    lightersPosY += cos(angle) * lightersSpeed;
    if (lightersPosX <= 0) lightersPosX = (MAP_W - 1) * 10;
    if (lightersPosX > (MAP_W - 1) * 10) lightersPosX = 1;
    if (lightersPosY <= 0) lightersPosY = (MAP_H - 1) * 10;
    if (lightersPosY > (MAP_H - 1) * 10) lightersPosY = 1;
    PosX = lightersPosX / 10;
    PosY = lightersPosY / 10;
  }
  for (uint16_t pov = 0; pov < POV + 1; pov+= povAngle) {
    float end_angle = radians(alpha + pov - PoV - 1);
    float a = sin(end_angle);
    float b = cos(end_angle);
    for (uint8_t i = 0; i < WIDTH + HEIGHT; i++) {
      byte ENDPosX = PosX + (i * a);
      byte ENDPosY = PosY + (i * b);
      if (mape[(ENDPosX) % MAP_W][(ENDPosY) % MAP_H] || CenterX - (i * a) < 0 || CenterX - (i * a) >= MAP_W || CenterY - (i * b) < 0 || CenterY - (i * b) >= MAP_H) { if (HitPoints) { leds[XY(CenterX - ((i - 1) * a), CenterY - ((i - 1) * b))] += CHSV(0, 0, 100); } break; } else leds[XY(CenterX - (i * a), CenterY - (i * b))] += CHSV(Color, 200, 100);
    }
  }
  EVERY_N_SECONDS(period) {
    if (act) {
      rot = random(-10, 10);
      lightersSpeed = 0;
      period = random(2, 5);
      act = false;
    } else {
      lightersSpeed = random(2, 20);
      rot = 0;
      period = random(5, 10);
      act = true;
    }
  }
  delay(16);
}