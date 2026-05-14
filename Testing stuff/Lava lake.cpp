void draw() {
  for (byte x = 0; x < LED_COLS; x++) {
    for (byte y = 0; y < LED_ROWS; y++) {
      leds[XY(x, y)] = ColorFromPalette(LavaColors_p, inoise8((x * y) - (y*LED_COLS), (y * 50) + millis() / 100, millis() / 10), 255 - y * (255 / LED_ROWS));
      
    }
  }
}