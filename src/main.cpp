#include <Arduino.h>
#include <math.h>
#include <Thread.h>
#include <ThreadController.h>


#include "IRpindefenitionsAndMore.h"
//#include <IRremote.h>
#define IR_INPUT_PIN 2
#include <TinyIRReceiver.cpp.h>
#include <effects.h>




LedPins pins = LedPins(10,11,6);
LedControl ledControl = LedControl(pins);
RangeGradient rangeGradient = RangeGradient(&ledControl, true, 100);
StaticColor staticColor = StaticColor(&ledControl);
RandomColor randomColor = RandomColor(&ledControl);
Effect *effect = &rangeGradient;
Animator animator = Animator(effect);


LedPins pinsWhite = LedPins(5,9,3);
LedControl ledControlWhile = LedControl(pinsWhite);
StaticColor staticColorWhite = StaticColor(&ledControlWhile);
Effect *effectWhite = &staticColorWhite;
Animator animatorWhite = Animator(effectWhite);

void setup() {
  Serial.begin(9600);
  //rangeGradient.SetHueStep(1);
  //rangeGradient.ApplyFullColorRange();
  //rangeGradient.ApplyVoilet();
  initPCIInterruptForTinyReceiver();
}

void loop() {
  animator.Run();
}

void RangeGradientRC(uint8_t aCommand, bool isRepeat) {
  switch (aCommand)
  {
    case 0xf:
      rangeGradient.ResetLed();
    break;
    case 0x1:
      rangeGradient.ApplyRed();
    break;
    case 0x2:
      rangeGradient.ApplyVoilet();
    break;
    case 0x3:
      rangeGradient.ApplyLightblue();
    break;
    case 0x4:
      rangeGradient.ApplyBlue();
    break;
    case 0x5:
      rangeGradient.ApplyLightgreen();
    break;
    case 0x6:
      rangeGradient.ApplyGreen();
    break;
    case 0x7:
      rangeGradient.ApplyYellow();
    break;
    case 0x8:
      rangeGradient.ApplyOrange();
    break;
    case 0x9:
      rangeGradient.ApplyWhile();
    break;
    case 0x45:
      rangeGradient.ApplyFullColorRange();
    break;
    case 0x15:
      rangeGradient.SpeedUp();
    break;
    case 0x1c:
      rangeGradient.SpeedDown();
    break;
    case 0x16:
      rangeGradient.SaturDown();
    break;
    case 0x19:
      rangeGradient.SaturUp();
    break;
    case 0x18:
      rangeGradient.BrightDown();
    break;
    case 0x12:
      rangeGradient.BrightUp();
    break;
  default:
    break;
  }
}

void StaticWhite(uint8_t aCommand, bool isRepeat) {
  switch (aCommand)
  {
  case 0xf:
    staticColor.Reset();
  break;
  case 0x1:
    staticColor.SetColor(Color(17,100,100));
  break;
  case 0x2:
    staticColor.SetColor(Color(17,97,100));
  break;
  case 0x3:
    staticColor.SetColor(Color(17,91,100));
  break;
  case 0x4:
    staticColor.SetColor(Color(17,79,100));
  break;
  case 0x5:
    staticColor.SetColor(Color(17,64,100));
  break;
  case 0x6:
    staticColor.SetColor(Color(17,51,100));
  break;
  case 0x7:
    staticColor.SetColor(Color(17,0,100));
  break;
  case 0xd:
    staticColor.Reset();
  break;
  case 0xb:
    staticColor.HueUp();
    Serial.println(staticColor.getHue());
  break;
  case 0xe:
    staticColor.HueDown();
    Serial.println(staticColor.getHue());
  break;
  case 0x16:
      effect->SaturDown();
      Serial.println(staticColor.getSatur());
    break;
    case 0x19:
      effect->SaturUp();
      Serial.println(staticColor.getSatur());
    break;
    case 0x18:
      effect->BrightDown();
      Serial.println(staticColor.getBright());
    break;
    case 0x12:
      effect->BrightUp();
      Serial.println(staticColor.getBright());
    break;
  default:
    break;
  }
}

void StaticColorRC(uint8_t aCommand, bool isRepeat) {
  switch (aCommand)
  {
  case 0xf:
    staticColor.Reset();
  break;
  case 0x1:
    staticColor.HueUp(10);
  break;
  case 0x2:
    staticColor.HueUp(30);
  break;
  case 0x3:
    staticColor.HueUp(60);
  break;
  case 0x4:
  staticColor.HueDown(10);
  break;
  case 0x5:
    staticColor.HueDown(30);
  break;
  case 0x6:
    staticColor.HueDown(60);
  break;
  case 0x7:
    staticColor.Reset();
  break;
  case 0x8:
    staticColor.Reset();
  break;
  case 0x9:
    staticColor.Reset();
  break;
  case 0xd:
  staticColor.Reset();
  break;
  case 0xb:
  staticColor.HueUp();
  Serial.println(staticColor.getHue());
  break;
  case 0xe:
  staticColor.HueDown();
  Serial.println(staticColor.getHue());
  break;
  case 0x16:
      effect->SaturDown();
      Serial.println(staticColor.getSatur());
    break;
    case 0x19:
      effect->SaturUp();
      Serial.println(staticColor.getSatur());
    break;
    case 0x18:
      effect->BrightDown();
      Serial.println(staticColor.getBright());
    break;
    case 0x12:
      effect->BrightUp();
      Serial.println(staticColor.getBright());
    break;
  default:
    break;
  }
}

void RandomColorRC(uint8_t aCommand, bool isRepeat) {
  switch (aCommand)
  {
  case 0x15:
    randomColor.SpeedUp();
    break;
  case 0x1c:
    randomColor.SpeedDown();
    break;
  default:
    break;
  }
}

enum class Effects {RandomColor, RangeGradient, StaticColor, StaticWhite};
Effects currentMode;
void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat) { 
  Serial.println(aCommand);
  if (aCommand == 0x53) {
    currentMode = Effects::RangeGradient;
    animator.setEffect(&rangeGradient);
    }
  if (aCommand == 0x5b) {
    currentMode = Effects::StaticColor;
    animator.setEffect(&staticColor);
    }
  if (aCommand == 0x57) {
    currentMode = Effects::RandomColor;
    animator.setEffect(&randomColor);
    }
  if (aCommand == 0x54) {
    currentMode = Effects::StaticWhite;
    animator.setEffect(&staticColor);
    }
  
  switch (currentMode)
  {
  case Effects::RangeGradient :
    RangeGradientRC(aCommand, isRepeat);
    break;
  case Effects::StaticColor :
    StaticColorRC(aCommand, isRepeat);
    break;
  case Effects::RandomColor :
    RandomColorRC(aCommand, isRepeat);
    break;
  case Effects::StaticWhite :
    StaticWhite(aCommand, isRepeat);
    break;
  default:
    break;
  }
}