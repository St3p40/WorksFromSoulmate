//St3p40
void draw(){
  uint16_t t  = millis() / 10;
  for (uint8_t i = 0; i < LED_COLS; i++) {
    for (uint8_t j = 0; j < LED_ROWS; j++) {
      leds[XY(i, LED_ROWS - 1 - j)] = ColorFromPalette(CloudColors_p, ~constrain(inoise8(i*10 + (t >> 3),j * 100 + t, t >> 2) - j * (255 / LED_ROWS),0,255));
    }
  }
}