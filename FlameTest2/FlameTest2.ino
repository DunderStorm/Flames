#include <FastLED.h>

#define LED_PIN     17
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    56

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 50
#define SPARKING 200
#define COOLDOWN 6


CRGB leds[NUM_LEDS];
// Array of temperature readings at each simulation cell
uint8_t heat[NUM_LEDS];

void setup() {
  delay(300); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  
  //debug setup
  Serial.begin(57600);
  pinMode(0, OUTPUT);
}

void loop()
{
  int spark = random8();
  int superSpark = 0;

  //if (random8() > 254) 
  //  superSpark = 32000;

  RisingFire(0, 28, false, random8(), superSpark);
  RisingFire(29, 55, true, random8(), superSpark);
  heatToColor();
  FastLED.show(BRIGHTNESS); // display this frame

  digitalWrite(0, HIGH);
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  digitalWrite(0, LOW);
}


void RisingFire(int low, int high, bool inverted, int spark, int superSpark){
  int start;
  
  if (inverted){
    start = high;
    // drift fire uppwards and cool a bit
    for(int i = low-1; i < high; i++) {
        heat[i] = + heat[i+1];
        heat[i] =max(heat[i]-COOLDOWN, 0);
    }
  }
  else{
    start = low;
    // drift fire uppwards and cool a bit
    for(int i = high-1; i > low; i--) {
        heat[i] = + heat[i-1];
        heat[i] =max(heat[i]-COOLDOWN, 0);
    }
  }

  //cool the start cell
  heat[start] =heat[start]>>1;

  if(spark < SPARKING) {
    int y = start;
    int igniteHeat;

    if (superSpark > 0) 
      igniteHeat = superSpark;
    else
      igniteHeat = min(heat[y] + random8(20,70)+0, 255);

    heat[y] = igniteHeat;

    //produce area of effect
    for(int i = 1; i < 10;i++){
      if ((y-i)>=low)
        heat[y-i] = min(heat[y-i]+(igniteHeat>>(i)), 255);
      if ((y+i)< high)
        heat[y+i] = min(heat[y+i]+(igniteHeat>>(i)), 255);
    }
  }
}

void heatToColor(){
    // Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor(heat[j]);
      leds[j] = color;
  }
}

/*
void StaticFire()
{
// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // if super flash
  if (random8() > 254)
  {
    for(int i = 0; i < NUM_LEDS; i++) {
      heat[i] = 255;
    }
  }
  else
  {
    // Cool down every cell a little
    for(int i = 0; i < NUM_LEDS; i++) {
      if (heat[i]> 150)
        heat[i] *= 0.90;
      else if (heat[i]> 100)
        heat[i] *= 0.92;
      else if (heat[i]> 50)
        heat[i] *= 0.95;
      else
        heat[i] *= 0.95;
    }

    // Randomly ignite new 'sparks' of heat 
    for (int j = 0; j < 15;j++)
    {
      uint8_t randomInt = random8();
      uint8_t shift;
      shift = (randomInt % 5)-2;

      if(randomInt < SPARKING ) {
        int y = 10 + 3*j + shift;

        int igniteHeat = min(heat[y] + random8(5,35), 150);
        heat[y] = igniteHeat;

        //produce area of effect
        for(int i = 1; i < 5;i++){
          if ((y-i)>=0)
            heat[y-i] = (heat[y-i]+(igniteHeat>>(i*2)));
          if ((y+i)< NUM_LEDS)
            heat[y+i] = (heat[y+i]+(igniteHeat>>(i*2)));
        }
      }
    }
  }

  // Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor(heat[j]);
      leds[j] = color;
  }
}
*/