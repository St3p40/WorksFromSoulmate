void draw() {
  float t = millis()/100.;
for(byte x =0; x <LED_COLS; x++){
  for(byte y =0; y <LED_ROWS; y++){
    leds[XY(x,y)]=ColorFromPalette(HeatColors_p,((sin8((x*10)+sin8(y*5+t*5.))+cos8(y*10))+1)+t);
}}
}