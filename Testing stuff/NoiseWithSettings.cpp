//Lighter variant then https://github.com/FastLED/FastLED/blob/master/examples/NoisePlusPalette/NoisePlusPalette.ino
//Stepko
byte Speed = 10;
byte Scale = 30;
byte hue;
static uint16_t x, y, z;

void FillNoise(bool ShiftX, bool ShiftY, bool ShiftZ, CRGBPalette16 palette, bool ShiftHue, bool BriNoise) {
  z += (ShiftZ) ? Speed : 0;
  x += (ShiftX) ? Speed : ((ShiftZ) ? Speed * 0.125 : 0); 
  y -= (ShiftY) ? Speed : ((ShiftZ) ? Speed * 0.0625: 0);
  for (uint8_t i = 0; i < LED_COLS; i++) {
    for (uint8_t j = 0; j < LED_ROWS; j++) {
      byte noise = inoise8((i * Scale) + x, (j * Scale) + y, z);
      byte Inoise = inoise8((j * Scale) + y, (i * Scale) + x, z);
      leds[XY(i, j)] = ColorFromPalette(palette, noise + hue,(!BriNoise || Inoise > 127)? 255 : dim8_raw(Inoise * 2));
    }
  }
  if (ShiftHue) { hue++; } else { hue = 0; }
}

void draw() {
  FillNoise(0, 0, 1, CloudColors_p, 0, 1);
}