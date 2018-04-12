//#include "LedControl.h"
// 
//void setup()
//{
//  Serial.begin(9600);
//  delay(1000); // power-up safety delay
// 
//  init_LedComponents();
//
//}
// 
//void loop() 
//{
// 
//}
//



#include "LedControl.h"
 
void setup()
{
  Serial.begin(9600);
  delay(1000); // power-up safety delay
 
  init_LedComponents();

}
 
void loop() 
{
  static uint32_t spd = 0;
  static uint8_t startIndex = 0;
  static bool flag = false;
  static int testVal = 3;
  static int delay_val = 100;
  static uint32_t prevTime = 0;

  ChangePalettePeriodically();
  startIndex = startIndex + 1; /* motion speed */
  
  switch(testVal)
  {
    case(0):
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      FillLEDsFromPaletteColors(startIndex, sLedArr, LED_STRIP_NUM_LEDS);
      FillLEDsFromPaletteColors(startIndex, cLedArr, CUP_LED_NUM_LEDS);
      track_glass(spd, CRGB::White);
      break;
    case(1):
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      FillLEDsFromPaletteColors(startIndex, sLedArr, LED_STRIP_NUM_LEDS);
      FillLEDsFromPaletteColors(startIndex, cLedArr, CUP_LED_NUM_LEDS);
      track_glass(spd, CRGB::Purple);
      break;
    case(2):
      SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND;
      FillLEDsFromPaletteColors(startIndex, sLedArr, LED_STRIP_NUM_LEDS);
      FillLEDsFromPaletteColors(startIndex, cLedArr, CUP_LED_NUM_LEDS);
      break;
    case(3):
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      FillLEDsFromPaletteColors(startIndex, cLedArr, CUP_LED_NUM_LEDS);
      break;
    case(4):
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      FillLEDsFromPaletteColors(startIndex, cLedArr, CUP_LED_NUM_LEDS);
      break;
    case(5):
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
      FillLEDsFromPaletteColors(startIndex, cLedArr, CUP_LED_NUM_LEDS);
      break;
  }

  if(millis() - prevTime >= delay_val)
  {
    prevTime = millis();
    
    if(testVal == 3)
    {
        stripDrinkComplete(CRGB::White, false);
    }
    else if(testVal == 4)
    {
        stripDrinkComplete(CRGB::Purple, false);
    }
    else if(testVal == 5)
    {
        stripDrinkComplete(CRGB::Purple, true);
    }
    FastLED[0].showLeds(DEFAULT_BRIGHTNESS);
    FastLED[1].showLeds(DEFAULT_BRIGHTNESS);
  
    if(spd == MAX_STEP_MOTOR_VAL)
      flag = true;
    else if(spd == 0)
      flag = false;
    if(flag == false)
      spd +=10;
    else
      spd -=10;
  }

  char c;
  if(Serial.available()) {
    c = Serial.read();
    if (c == '0') {  // forward
      testVal = 0;
    }
    else if (c == '1') {  // reverse
      testVal = 1;
    }
    else if (c == '2') {  // reverse
      testVal = 2;
    }
    else if (c == '3') {  // reverse
      testVal = 3;
      FastLED.clearData();
    }
    else if (c == '4') {  // reverse
      testVal = 4;
    }    
    else if (c == '5') {  // reverse
      testVal = 5;
    }
    else if (c == '6') {  // reverse
      testVal = 6;
    }
    else if (c == 'i') {  // reverse
      delay_val += 10;
      Serial.println(delay_val);
    }
    else if (c == 'd') {  // reverse
      delay_val -= 10;
      if(delay_val < 1)
        delay_val = 1;
      Serial.println(delay_val);
    }
  }
 
}


