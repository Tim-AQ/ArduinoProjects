#include <FastLED.h>
#include <EEPROM.h>

#define DATA_PIN   6
#define BUTTON_PIN 2


#define NUM_LEDS          114
#define BRIGHTNESS        196
#define FRAMES_PER_SECOND  60
CRGB leds[NUM_LEDS];

bool buttonState;

#define SAVE_PATTERN_DELAY 60000
bool patternChanged = false;
unsigned long lastChangeTimer = 0;

uint8_t currentPatternNumber = 0;
uint8_t hue = 0; // rotating "base color" used by many of the patterns

#define NUM_PATTERNS 3
void (*patterns[])() = { &quadwavePulses, &redLivingLight, &rainbow };


void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  delay(1);
  pinMode(BUTTON_PIN, INPUT);
  buttonState = digitalRead(BUTTON_PIN);

  currentPatternNumber = EEPROM.read(0);
  //currentPatternNumber = 0;
  
  // Strange fix for rainbow-issue
  CHSV hsv;
  CRGB rgb;
  rgb = hsv;
  
  // analogRead was not sufficiently random. This amplifies the impact of small variations in analogRead.
  randomSeed(analogRead(A0));
  randomSeed(random(0,2147483647));
  
  Serial.println( F("Window") );
}

  
void loop() {
  patterns[currentPatternNumber]();
  FastLED.show();

  
  FastLED.delay(1000/FRAMES_PER_SECOND);
  
  checkButton();
}

void checkButton() {
  static uint8_t cntr = 0;
  if( digitalRead(BUTTON_PIN) != buttonState ) {
    if( ++cntr > 5) {
      nextPattern();
      buttonState = !buttonState;

      
      cntr = 0;
    }
  } else {
    cntr = 0;
  }

  if( patternChanged && millis()-lastChangeTimer > SAVE_PATTERN_DELAY ) {
    Serial.print( F("Saving pattern number: ") );
    Serial.println(currentPatternNumber);
    EEPROM.update(0, currentPatternNumber);
    patternChanged = false;
  }
}

void nextPattern() {
  currentPatternNumber = (currentPatternNumber + 1) % NUM_PATTERNS;
  
  patternChanged = true;
  lastChangeTimer = millis();
      
  Serial.print( F("Pattern number changed to: "));
  Serial.println(currentPatternNumber);
}


