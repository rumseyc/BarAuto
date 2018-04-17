#include "LedControl.h"

 CRGB sLedArr[LED_STRIP_NUM_LEDS];
 CRGB cLedArr[CUP_LED_NUM_LEDS];
 
 CRGBPalette16 currentPalette;
 TBlendType    currentBlending;

 static uint32_t prevLedTrackingIdx = 0;
 static CRGB prevLedTrackVal[NUM_TRACK_LEDS];

void init_LedComponents()
{
  FastLED.addLeds<WS2812B, LED_STRIP_CTRL_PIN, GRB>(sLedArr, LED_STRIP_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812B, CUP_LED_CTRL_PIN, GRB>(cLedArr, CUP_LED_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  
  for(int i = 0; i < NUM_TRACK_LEDS; i++)
  {
    prevLedTrackVal[i] = CRGB::Black;
  }
}

void track_glass(uint32_t motor_pos, CRGB color)
{ 
  int i = 0;
  if(motor_pos > MAX_STEP_MOTOR_VAL)
  {
    motor_pos = MAX_STEP_MOTOR_VAL;
  }
  
  for(i = 0; i < NUM_TRACK_LEDS; i++)
  {
    sLedArr[prevLedTrackingIdx+i] = prevLedTrackVal[i];
  }
  
  prevLedTrackingIdx = map(motor_pos, 0, MAX_STEP_MOTOR_VAL, 0, LED_STRIP_NUM_LEDS - NUM_TRACK_LEDS);

  for(i = 0; i < NUM_TRACK_LEDS; i++)
  {
      prevLedTrackVal[i] = sLedArr[prevLedTrackingIdx + i];
      sLedArr[prevLedTrackingIdx + i] = color;
  }
}
 
void FillLEDsFromPaletteColors( uint8_t colorIndex, CRGB* buf, int len )
{
    uint8_t brightness = DEFAULT_BRIGHTNESS;
 
    for( int i = 0; i < len; i++) {
        buf[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
 
 
// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.
 
void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
 
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
    }
}
 
// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}
 
// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
 
}
 
// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
 
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

void stripDrinkComplete(CRGB color, bool disable_stale_leds)
{
  int i = 0;

  if(!disable_stale_leds)
  {
    for(i = 0; i < NUM_TRACK_LEDS; i++)
    {
      sLedArr[prevLedTrackingIdx+i] = CRGB::Black;
      sLedArr[(LED_STRIP_NUM_LEDS - 1) - (prevLedTrackingIdx+i)] = CRGB::Black;   
    }
  }
  
  prevLedTrackingIdx += 1;
  if(prevLedTrackingIdx >= (LED_STRIP_NUM_LEDS/2) - (NUM_TRACK_LEDS/2))
  {
    prevLedTrackingIdx = 0;
    if(disable_stale_leds)
    {
      for(i = 0; i < LED_STRIP_NUM_LEDS; i++)
      {
        sLedArr[i] = CRGB::Black;
      }
    }

  }

  for(i = 0; i < NUM_TRACK_LEDS; i++)
  {
      sLedArr[prevLedTrackingIdx + i] = color;
      sLedArr[(LED_STRIP_NUM_LEDS - 1) - (prevLedTrackingIdx+i)] = color;

  }
}

void clearLedIdxVal()
{
  prevLedTrackingIdx = 0;
}

void setSolidColorRange(CRGB color, CRGB* buf, uint16_t startIdx, uint16_t len, uint8_t bufIdx, uint8_t brightness)
{
  if(buf == NULL || bufIdx > 1)
  {
    return;
  }
  
  for(int i = startIdx; i < (startIdx + len); i++)
  {
    buf[i] = color;
  }
  FastLED[bufIdx].showLeds(brightness);
}

