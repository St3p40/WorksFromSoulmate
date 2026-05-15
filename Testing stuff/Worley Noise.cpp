//Worley Noise
//@st3p40 (aka Stepko)
//15.05.26

const uint32_t CELL_AM = max(LED_COLS, LED_ROWS);
struct {
  int16_t x;
  int16_t y;
  int16_t vx;
  int16_t vy;
} cell [CELL_AM];
bool setup = 1;
void draw() {
  if (setup)
  {
    for(uint16_t i = 0; i < CELL_AM; i++)
    {
      cell[i].x = random(0, LED_COLS << 8);
      cell[i].y = random(0, LED_ROWS << 8);
      cell[i].vx = random(-30, 30);
      cell[i].vy = random(-30, 30);
    }
    setup = 0;
  }

  for(uint16_t i = 0; i < CELL_AM; i++)
  {
    cell[i].x += cell[i].vx;
    cell[i].y += cell[i].vy;

    if (cell[i].x < 0) { cell[i].x = 0; cell[i].vx = -cell[i].vx; }
    if (cell[i].x > (LED_COLS-1) << 8) { cell[i].x = (LED_COLS-1) << 8; cell[i].vx = -cell[i].vx; }
    if (cell[i].y < 0) { cell[i].y = 0; cell[i].vy = -cell[i].vy; }
    if (cell[i].y > (LED_ROWS-1) << 8) { cell[i].y = (LED_ROWS-1) << 8; cell[i].vy = -cell[i].vy; }
  }

  for (uint16_t x = 0; x < LED_COLS; x++)
  {
    int32_t x8 = (int32_t)x << 8;
    for (uint16_t y = 0; y < LED_ROWS; y++)
    {
      int32_t y8 = (int32_t)y << 8;
      uint32_t m_dist1 = 0xffffffff, m_dist2 = 0xffffffff;
      for (uint16_t i = 0; i < CELL_AM; i++) {
        int32_t dx = (int32_t)cell[i].x - x8;
        int32_t dy = (int32_t)cell[i].y - y8;
        uint32_t d2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);

        if (d2 < m_dist1) {
          m_dist2 = m_dist1;
          m_dist1 = d2;
        } else if (d2 < m_dist2) {
          m_dist2 = d2;
        }
      }
      uint8_t val = constrain((int32_t)((sqrt(m_dist2) - sqrt(m_dist1))) >> 1, 0, 255);
      leds[XY(x,y)] = CHSV(150, val, 255);
    }
  }
  /*for(uint16_t i = 0; i < CELL_AM; i++)
  {
    leds[XY(cell[i].x >> 8,cell[i].y >> 8)] = CHSV(0, 255, 255); //testing
  }*/
}