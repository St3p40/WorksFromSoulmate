uint8_t scale = 10;
uint8_t hue;
uint16_t t;
void draw() {
  hue++;
  t++;
  //scale = beatsin8(1,0,20);
  float x_factor = sin(radians(t));
  float y_factor = cos(radians(t));
  for(uint8_t x = 0; x < LED_COLS; x++){
    for(uint8_t y = 0; y < LED_ROWS; y++){
      leds[XY(x,y)] = CHSV((((x*x_factor)+(y*y_factor))+hue)*scale,255,255);
    }
  }
}