#include <Arduino.h>
#include <math.h>
#include <Thread.h>
#include <ThreadController.h>


#include "IRpindefenitionsAndMore.h"
#define IR_INPUT_PIN 2
#include <TinyIRReceiver.cpp.h>
#include <effects.h>

LedPins pins = LedPins(10,11,6);
LedControl firstLedControl = LedControl(pins);
RangeGradient rangeGradient = RangeGradient(&firstLedControl, true, 100);
StaticColor staticColor = StaticColor(&firstLedControl);
RandomColor randomColor = RandomColor(&firstLedControl);
Effect *effect = &rangeGradient;
Animator firstAnimator = Animator(effect);


LedPins pinsWhite = LedPins(5,9,3);
LedControl secondLedControl = LedControl(pinsWhite);
StaticColor standaloneWhite = StaticColor(&secondLedControl);
Effect *effectWhite = &standaloneWhite;
Animator secondAnimator = Animator(effectWhite);

void setup() {
  Serial.begin(9600);
  initPCIInterruptForTinyReceiver();
}

void loop() {
  firstAnimator.Run();
  secondAnimator.Run();
}


//
//remote control section
//
void RCForGradientRange(uint8_t aCommand, bool isRepeat) {
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

void RCForStaticWhite(uint8_t aCommand, bool isRepeat) {
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

void RCForStaticColor(uint8_t aCommand, bool isRepeat) {
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

void RCForRandomColor(uint8_t aCommand, bool isRepeat) {
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

void RCForStandaloneWhite(uint8_t aCommand, bool isRepeat) { 
switch (aCommand)
  {
  case 0xf:
    standaloneWhite.Reset();
  break;
  case 0x1:
    standaloneWhite.SetColor(Color(17,100,100));
  break;
  case 0x2:
    standaloneWhite.SetColor(Color(17,97,100));
  break;
  case 0x3:
    standaloneWhite.SetColor(Color(17,91,100));
  break;
  case 0x4:
    standaloneWhite.SetColor(Color(17,79,100));
  break;
  case 0x5:
    standaloneWhite.SetColor(Color(17,64,100));
  break;
  case 0x6:
    standaloneWhite.SetColor(Color(17,51,100));
  break;
  case 0x7:
    standaloneWhite.SetColor(Color(17,0,100));
  break;
  case 0xd:
    staticColor.Reset();
  break;
  case 0xb:
    standaloneWhite.HueUp();
    Serial.println(standaloneWhite.getHue());
  break;
  case 0xe:
    standaloneWhite.HueDown();
    Serial.println(standaloneWhite.getHue());
  break;
  case 0x16:
      effect->SaturDown();
      Serial.println(standaloneWhite.getSatur());
    break;
    case 0x19:
      effect->SaturUp();
      Serial.println(standaloneWhite.getSatur());
    break;
    case 0x18:
      effect->BrightDown();
      Serial.println(standaloneWhite.getBright());
    break;
    case 0x12:
      effect->BrightUp();
      Serial.println(standaloneWhite.getBright());
    break;
  default:
    break;
  }
}

enum class RCModes {RandomColor, RangeGradient, StaticColor, StaticWhite, StandaloneWhite};
RCModes currentMode;
void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat) { 
  Serial.println(aCommand);
  if (aCommand == 0x53) {
    currentMode = RCModes::RangeGradient;
    firstAnimator.setEffect(&rangeGradient);
    }
  if (aCommand == 0x5b) {
    currentMode = RCModes::StaticColor;
    firstAnimator.setEffect(&staticColor);
    }
  if (aCommand == 0x57) {
    currentMode = RCModes::RandomColor;
    firstAnimator.setEffect(&randomColor);
    }
  if (aCommand == 0x54) {
    currentMode = RCModes::StaticWhite;
    firstAnimator.setEffect(&staticColor);
    }
  if (aCommand == 0x1b) {
    currentMode = RCModes::StandaloneWhite;
    }

  switch (currentMode)
  {
  case RCModes::RangeGradient:   RCForGradientRange(aCommand, isRepeat); break;
  case RCModes::StaticColor:     RCForStaticWhite(aCommand, isRepeat); break;
  case RCModes::RandomColor:     RCForRandomColor(aCommand, isRepeat); break;
  case RCModes::StaticWhite:     RCForStaticColor(aCommand, isRepeat); break;
  case RCModes::StandaloneWhite: RCForStandaloneWhite(aCommand, isRepeat); break;
  default: break;
  }
}