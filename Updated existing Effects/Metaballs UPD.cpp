/* Metaballs proof of concept by Stefan Petrick
...very rough 8bit math here...
read more about the concept of isosurfaces and metaballs:
https://www.gamedev.net/articles/programming/graphics/exploring-metaballs-and-isosurfaces-in-2d-r2556*/
#define scale 160
#define hormap (256 / LED_COLS)
#define vermap (256 / LED_ROWS)
float speed = 0.5;

uint8_t distance(byte x0, byte y0, byte x1, byte y1) {
  uint8_t dx = abs(x1 - x0);
  uint8_t dy = abs(y1 - y0);
  uint8_t dist = 2 * sqrt16((dx * dx) + (dy * dy));
  return dist;
}

void draw() {
  unsigned long t = (float)millis() * speed;
  // get some 3 random moving points
  //uint8_t x1 = beatsin88(23 * speed, 0, LED_COLS - 1);//V1
  //uint8_t y1 = beatsin88(28 * speed, 0, LED_ROWS - 1);

  uint8_t x1 = inoise8(t, 12355, 85) / hormap;// V2
  uint8_t y1 = inoise8(t, 5, 685) / vermap;
  
  uint8_t x2 = inoise8(t, 25355, 685) / hormap;
  uint8_t y2 = inoise8(t, 355, 11685) / vermap;
  
  uint8_t x3 = inoise8(t, 55355, 6685) / hormap;
  uint8_t y3 = inoise8(t, 25355, 22685) / vermap;
  
  for (uint8_t y = 0; y < LED_ROWS; y++) {
    for (uint8_t x = 0; x < LED_COLS; x++) {
      
      // calculate distances of the 3 points from actual pixel
      // and add them together with weightening
      
      uint8_t dist = distance(x1, y1, x, y);
      dist += distance(x2, y2, x, y);
      dist += distance(x3, y3, x, y);
      
      // inverse result
      byte color = scale * 4 / (dist == 0 ? 1 : dist);
      
      // map color between thresholds
      if (color > 0 and color < 60) {
        leds[XY(x, y)] = ColorFromPalette(RainbowColors_p, color * 9);
      } else {
        leds[XY(x, y)] = ColorFromPalette(RainbowColors_p, 0);
      }
      // show the 3 points, too
      leds[XY(x1, y1)] = CRGB::White;
      leds[XY(x2, y2)] = CRGB::White;
      leds[XY(x3, y3)] = CRGB::White;
    }
  }
}