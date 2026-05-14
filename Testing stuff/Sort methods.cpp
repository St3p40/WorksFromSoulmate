byte method = 0;
byte a[LED_COLS];
void remakeMassive() {
  for (byte x = 0; x < LED_COLS; x++) {
    a[x] = random(1, LED_ROWS);
  }
}
void drawMassive() {
  FastLED.clear();
  for (byte x = 0; x < LED_COLS; x++) {
    for (byte y = 0; y < LED_ROWS; y++) {
      if (a[x] > y) {
        leds[XY(x, y)] = CHSV(a[x] * 16, 255, 255);
      }
    }
  }
  FastLED.show();
  delay(10);
}

void Change(byte FID, byte SID) {
  byte temp = a[FID];
  a[FID] = a[SID];
  a[SID] = temp;
}

void BubbleSort() {
  for (byte y = 0; y < LED_COLS - 1; y++) {
    for (byte x = 0; x < LED_COLS - (y + 1); x++) {
      if (a[x] > a[x + 1]) {
        Change(x, x + 1);
      }
    }
    drawMassive();
  }
}
void SelectionSort() {
  for (byte y = 0; y < LED_COLS; y++) {
    byte min = 255;
    byte minID;
    for (byte x = y; x < LED_COLS; x++) {
      if (a[x] < min) {
        min = a[x];
        minID = x;
      }
    }
    Change(y, minID);
    drawMassive();
  }
}

void InsertionSort() {
  for (byte y = 1; y < LED_COLS; y++) {
    for (byte x = y; y > 0; x--) {
      if (a[x - 1] > a[x]) {
        Change(x, x - 1);
      } else break;
    }
    drawMassive();
  }
}

void sortPart(byte from_, byte to_) {
  for (byte y = from_; y < to_; y++) {
    byte min = 255;
    byte minID;
    for (byte x = y; x < to_; x++) {
      if (a[x] < min) {
        min = a[x];
        minID = x;
      }
    }
    Change(y, minID);
  }  
}

double log2(double num) {
  double number = log(num) / log(2);
  return (number);
}

void MergeSort() {
  byte steps = log2(LED_COLS) + 1;
  for (byte a = 0; a < steps; a++) {
    byte step2 = pow(2, a + 1);
    for (byte ap = 0; ap <= LED_COLS; ap += step2) {
      sortPart(ap, constrain(ap + step2, 0, LED_COLS));
      drawMassive();
    }
  }
}

void draw() {
  remakeMassive();
  drawMassive();
  switch (method) {
    case 0:
      BubbleSort();
      break;
    case 1:
      SelectionSort();
      break;
    case 2:
      InsertionSort();
      break;
    case 3:
      MergeSort();
    break;
  }
  if (method == 3)
    method = 0;
  else
    method++;
  delay(500);
}