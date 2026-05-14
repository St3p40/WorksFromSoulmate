 uint8_t regime = 2;
 uint8_t CodeR(byte x, byte y, double t) {
   switch (regime) {
     case 0:
       return sin8(x * y) - t;
       break;
     case 1:
       return sin8((x - 8) * cos8((y + 20) * 4) / 4 + t);
       break;
     case 2:
       return inoise8(x * 20, y * 20, t);
       break;
     case 3:
       float X = x + (sin((double) t / 100) * LED_COLS - 1) - LED_COLS / 2;
       float Y = y + (cos((double) t / 99) * LED_ROWS - 1) - LED_ROWS / 2;
       return sin8(16 * sqrt16(Y * Y + X * X));
       break;
     case 4:
       return inoise8(x * 20, (y * 20) - t);
       break;
   }
 }
 uint8_t CodeG(byte x, byte y, double t) {
   switch (regime) {
     case 0:
       return sin8((x * x + y * y) + t);
       break;
     case 1:
       return (sin8(x * 16 + t / 3) + cos8(y * 8 + t / 2)) / 2;
       break;
     case 2:
       return inoise8(x * 20, y * 20 + t);
       break;
     case 3:
       float X = x + (sin((double) t / 196) * LED_COLS - 1) - LED_COLS / 2;
       float Y = y + (cos((double) t / 100) * LED_ROWS - 1) - LED_ROWS / 2;
       return sin8(16 * sqrt16(Y * Y + X * X));
       break;
     case 4:
       return inoise8(x * 20, (y * 20) - t);
       break;
   }
 }
 uint8_t CodeB(byte x, byte y, double t) {
   switch (regime) {
     case 0:
       return sin8(x * 5 + sin8(y * 5 + t) - cos8(y));
       break;
     case 1:
       return sin8(cos8(x * 8 + t / 3) + sin8(y * 8 + t / 4) + t);
       break;
     case 2:
       return inoise8(x * 20 + t, y * 20);
       break;
     case 3:
       float X = x + (sin((double) t / 99) * LED_COLS - 1) - LED_COLS / 2;
       float Y = y + (cos((double) t / 98) * LED_ROWS - 1) - LED_ROWS / 2;
       return sin8(16 * sqrt16(Y * Y + X * X));
       break;
     case 4:
       return inoise8(x * 20, (y * 20) - t);
       break;
   }
 }

 void draw() {
   double t = millis() / 10;
   for (byte x = 0; x < LED_COLS; x++) {
     for (byte y = 0; y < LED_ROWS; y++) {
       leds[XY(x, y)].r = CodeR(x, y, t);
       leds[XY(x, y)].g = CodeG(x, y, t);
       leds[XY(x, y)].b = CodeB(x, y, t);
     }
   }
 }