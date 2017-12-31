#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 3

#define RAINBOW 0
#define BLUE    1
#define RED     2
#define GREEN   3
#define YELLOW  4
#define WHITE   5

int mode = RAINBOW;


#define INTENSITE1 23
#define INTENSITE2 52
#define INTENSITE3 81
#define INTENSITE4 110
#define INTENSITE5 139 
#define INTENSITE6 168
#define INTENSITE7 197
#define INTENSITE8 226
#define INTENSITE9 255

int intensiteLumineuse = INTENSITE5;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(148, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  pinMode(8,INPUT);
  digitalWrite(8,HIGH);


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  colorConstant(strip.Color(0, 0, 0));
  
  Serial.begin(115200);               //initial the Serial
}



int allume = 0;

void loop() {

  
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 102), 50); // Blue
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue

  RTCheck();
  
  if (allume) {
     switch (mode) {
      case RAINBOW:
         rainbowCycle();
         break;
      case RED:
         colorConstant(strip.Color(intensiteLumineuse,0,0));
         break;   
      case BLUE:
         colorConstant(strip.Color(0,0,intensiteLumineuse));
         break;   
      case GREEN:
         colorConstant(strip.Color(0,intensiteLumineuse,0));
         break;   
      case YELLOW:
         colorConstant(strip.Color(intensiteLumineuse/2,intensiteLumineuse/3,0));
         break;   
      case WHITE:
         colorConstant(strip.Color(intensiteLumineuse/2,intensiteLumineuse/2,intensiteLumineuse/2));
         break;   
     } 
  } else {
    colorConstant(strip.Color(0, 0, 0));
  }

  //rainbow(20);
  //theaterChaseRainbow(50);
}


// fonction d'écoute et mise à jour guirlande en fonction de la reception bluetooth...
void RTCheck() {
  
      if(Serial.available())  {
        char received = Serial.read();
        allume = 1; 
        switch (received) {
          case '0':
             allume = 0;
             break;
          case 'r':
             mode = RED;
             break;
          case 'w':
             mode = RAINBOW;
             break;
          case 't':
             mode = WHITE;
             break;
          case 'b':
             mode = BLUE;
             break;
          case 'v':
             mode = GREEN;
             break;
          case 'j':
             mode = YELLOW ;
             break;
          case '1':
             intensiteLumineuse = INTENSITE1 ;
             break;
          case '2':
             intensiteLumineuse = INTENSITE2 ;
             break;
          case '3':
             intensiteLumineuse = INTENSITE3 ;
             break;
          case '4':
             intensiteLumineuse = INTENSITE4 ;
             break;
          case '5':
             intensiteLumineuse = INTENSITE5 ;
             break;
          case '6':
             intensiteLumineuse = INTENSITE6 ;
             break;
          case '7':
             intensiteLumineuse = INTENSITE7 ;
             break;
          case '8':
             intensiteLumineuse = INTENSITE8 ;
             break;
          case '9':
             intensiteLumineuse = INTENSITE9 ;
             break;
          default:
          break;
       }
    }
}



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void colorConstant(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    int variateur =analogRead(A0);
    int wait = map(variateur,0,1024,0,50);

    RTCheck();

    if ( (!allume) || (mode!=RAINBOW)) {
      return;
    }
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(((255 - WheelPos * 3) *intensiteLumineuse)/255 , 0, (WheelPos * 3 *intensiteLumineuse)/255);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3 *intensiteLumineuse)/255, ((255 - WheelPos * 3) *intensiteLumineuse)/255);
  }
  WheelPos -= 170;
  return strip.Color((WheelPos * 3 *intensiteLumineuse)/255, ((255 - WheelPos * 3) *intensiteLumineuse)/255, 0);
}
