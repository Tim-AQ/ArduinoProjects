#define MAX_SPEED 2
#define MAX_GRADIANT 10

DEFINE_GRADIENT_PALETTE( whiteglow_gp ) {
  0,    255,  225,  190,
  64,  255,  0,  190,
  128, 0, 225,  190,
  192, 255,  225,  0,
  255,  255,  225,  190
};
DEFINE_GRADIENT_PALETTE( rainbowglow_gp ) {
  0,    255,  225,  190,
  64,  255,  25,  19,
  128, 30, 225,  19,
  192, 30,  25,  190,
  255,  255,  225,  190
};
DEFINE_GRADIENT_PALETTE( redglow_gp ) {
  0,    155,  155,  0,
  48,  180,  125,  0,
  80,  155,  55,  0,
  128, 10,  5,  0,
  176, 155,  55,  0,
  208, 180,  125,  0,
  255,  155,  155,  0
};
DEFINE_GRADIENT_PALETTE( yellowglow_gp ) {
  0,    0,  0,  0,
  64,  155,  100,  25,
  128, 0, 0,  0,
  192, 155,  75,  0,
  255,  0,  0,  0
};
CRGBPalette16 yellowglow = yellowglow_gp;
CRGBPalette16 redglow = redglow_gp;
CRGBPalette16 whiteglow = whiteglow_gp;
CRGBPalette16 rainbowglow = rainbowglow_gp;


uint8_t colourIndex[NUM_LEDS];
int8_t  colourSpeed[NUM_LEDS];


void redLivingLight() {
  livingLight(redglow);
}
void yellowLivingLight() {
  livingLight(yellowglow);
}

void livingLight(CRGBPalette16 palette) {
  randomiseSpeed();
  incrementColourIndex();

  for(uint8_t i=0; i<NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, colourIndex[i], 255, LINEARBLEND);
  }
}


void randomiseSpeed() {
  if(random8()<64) {
      colourSpeed[0] = constrain( colourSpeed[0] + random(-1,2), -MAX_SPEED, MAX_SPEED);
  }
  if(random8()<64) {
    colourSpeed[NUM_LEDS-1] = constrain( colourSpeed[NUM_LEDS-1] + random(-1,2), -MAX_SPEED, MAX_SPEED);
  }
  for(uint8_t i=1; i<NUM_LEDS-1; i++) {
    int deviation = colourIndex[i-1] + colourIndex[i+1] - colourIndex[i]*2;
    if( deviation > MAX_GRADIANT ) {
      colourSpeed[i] = constrain( colourSpeed[i]+random(0,3), -MAX_SPEED+1, MAX_SPEED+1);
    } else if( deviation < -MAX_GRADIANT ) {
      colourSpeed[i] = constrain( colourSpeed[i]+random(-2,1), -MAX_SPEED, MAX_SPEED);
    } else if(random8()<64) {
      colourSpeed[i] = constrain( colourSpeed[i] + random(-1,2), -MAX_SPEED-1, MAX_SPEED-1);
    }
  }
}

void incrementColourIndex() {
  colourIndex[0] += colourSpeed[0];
  for(uint8_t i=1; i<NUM_LEDS; i++) {
    colourIndex[i] = colourIndex[i] + colourSpeed[i];
  }
}


void resetLivingLightArrays() {
  for(uint8_t i=0; i<NUM_LEDS; i++) {
    colourIndex[i] = colourSpeed[i] = 0;
  }
}

