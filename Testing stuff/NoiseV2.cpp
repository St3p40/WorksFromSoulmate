void draw() {
 unsigned long t = millis() / 100;
  for (byte x = 0; x < LED_COLS; x++) {
    for (byte y = 0; y < LED_ROWS; y++) {
      byte col = inoise8(x * 16 + t, y * 16 + t);
      //
      int8_t cx = x - beatsin8(5, 0, LED_COLS - 1);
      int8_t cy = y - beatsin8(6, 0, LED_ROWS - 1, 0, 64);
      col += sin8(16*sqrt16(cx*cx+cy*cy));
      //
      cx = x - beatsin8(5, 0, LED_COLS - 1,0,64);
      cy = y - beatsin8(6, 0, LED_ROWS - 1);
      col += sin8(16*sqrt16(cx*cx+cy*cy));
      //
      col += sin8(cx - t);col += cos8(cy - t);
      //
      uint8_t bri = col;
      leds[XY(x, y)] = ColorFromPalette(LavaColors_p, col, bri);
    }
  }
}