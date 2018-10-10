bool pulseIsFadingIn = false;
uint8_t pulseArray[NUM_LEDS];
uint8_t pulseHue, pulseSaturation;
unsigned long pulseDelay = 0;
unsigned long lastPulseTimer = 0;

void quadwavePulses() {
  for(uint8_t i=0; i<NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(1);
  }
  if(pulseIsFadingIn) {
    CHSV hsv;
    hsv.h = pulseHue;
    hsv.s = pulseSaturation;
    for(uint8_t i=0; i<NUM_LEDS; i++) {
      if(pulseArray[i]) {
        hsv.v = pulseArray[i]*.35+1;
        leds[i] += hsv;
        if(pulseArray[i]>=250 && (leds[i].r==255 || leds[i].g==255 || leds[i].b==255) ){
          pulseIsFadingIn = false;
          lastPulseTimer = millis();
          pulseDelay = random(100, 1000);
        }
      }
    }
  }
  if(!pulseIsFadingIn && millis()-lastPulseTimer > pulseDelay) {
    addRandomisedPulse();
  }
}

void addRandomisedPulse() {
  
  uint8_t centre = findDarkPixel();
  addPulse( centre, random(30,60), random8(), 255-exp(random8()/46) );
  
}

void addPulse(uint8_t centre, uint8_t width, uint8_t h, uint8_t s) {
  pulseHue = h;
  pulseSaturation = s;
  pulseIsFadingIn = true;
  
  for(uint8_t i=0; i<NUM_LEDS; i++) {
    pulseArray[i]=0;
  }
  
  for(uint8_t i=0; i<width+1; i++) {
    if(centre-width/2+i < 0) i=width/2-centre;
    if(centre-width/2+i >= NUM_LEDS) return;

    pulseArray[centre-width/2+i] = quadwave8(map(i, 0, width, 0, 255));
  }
}

void generateQuadPulse(uint8_t *arr, uint8_t width) {
  for(uint8_t i; i<width; i++) {
    arr[i] = quadwave8(map(i, 0, width, 0, 255));
  }
}


uint8_t findDarkPixel() {
  #define ATTEMPTS 5
  uint8_t min = 255;
  uint8_t min_index=0;
  
  for(uint8_t i=0; i<ATTEMPTS; i++) {
    uint8_t index = random(0, NUM_LEDS);

    uint8_t value = leds[index].r/3 + leds[index].g/3 + leds[index].b/3;
    if(value <= min) {
      min = value;
      min_index = index;
    }
  }
  return min_index;
}

