int8_t a[NUM_LEDS];
void draw() {
  for (uint32_t i = 0; i < NUM_LEDS; i++) {
    if(leds[i]){
     uint8_t max = max(leds[i].r,max(leds[i].g,leds[i].b));
     CRGB colAdd = CRGB(map(leds[i].r,0,max,0,abs(a[i])),map(leds[i].g,0,max,0,abs(a[i])),map(leds[i].b,0,max,0,abs(a[i])));
     if(a[i] > 0 )leds[i] += colAdd;
     else leds[i] -= colAdd;
     if(max + a[i] > 255) a[i] = -a[i];
    }
    else if(random()%128 == 0) {a[i] = pow(2,random()%2)+2; leds[i] = CHSV(random(),255, a[i]); }
  }
}