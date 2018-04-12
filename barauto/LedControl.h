#ifndef __LedControl_h__
#define __LedControl_h__
#include "FastLED.h"

#define LED_STRIP_NUM_LEDS      60
#define LED_STRIP_CTRL_PIN      5
 
#define CUP_LED_NUM_LEDS        12
#define CUP_LED_CTRL_PIN        6
 
#define DEFAULT_BRIGHTNESS  100

#define NUM_TRACK_LEDS      8

#define MAX_STEP_MOTOR_VAL  6000
#define MOTOR_CENTER_VAL    (MAX_STEP_MOTOR_VAL/2)


extern CRGB sLedArr[LED_STRIP_NUM_LEDS];
extern CRGB cLedArr[CUP_LED_NUM_LEDS];
 
extern CRGBPalette16 currentPalette;
extern TBlendType    currentBlending;

void init_LedComponents();
void track_glass(uint32_t motor_pos, CRGB color);
void FillLEDsFromPaletteColors( uint8_t colorIndex, CRGB* buf, int len );
void ChangePalettePeriodically();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();
void stripDrinkComplete(CRGB color, bool disable_stale_leds);
void clearLedIdxVal();

#endif
