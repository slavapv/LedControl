#include <helper.h>

class StaticColor : public Effect {
  public:
  StaticColor(LedControl *ledControl,
              bool isAnimated = true,
              unsigned long interval = 20,
              int hueStep = 1,
              Color Color = Color(0,100,100)
              ) :
  Effect(ledControl, isAnimated, interval)  {
  }

//    currentColor = static_cast<ColorMode>(color);

  void Reset() {
    this->ledC->SetColor(Color(0,100,100));
  }

  void SetColor(Color color) {
    // float brightness = this->ledC->getColor().b;
    // float saturation = this->ledC->getColor().s;
    this->ledC->SetColor(color);
  }

  void HueUp(uint16_t step = 1) {
    this->ledC->HueUp(step);
  }

  void HueDown(uint16_t step = 1) {
    this->ledC->HueDown(step);
  }

  float getHue() {
    return this->ledC->getColor().h;
  }
  
  float getSatur() {
    return this->ledC->getColor().s;
  }

  float getBright() {
    return this->ledC->getColor().b;
  }

  virtual void Animate() {
    //this->ledC->BrightDown
  }

  private:
  //bool fade;
};

class RandomColor : public Effect {
  public:
  bool isFadeing = false;
  float initialInterval;
  RandomColor(LedControl *ledControl,
              bool isAnimated = true,
              unsigned long interval = 500
              ) :
  Effect(ledControl, isAnimated, interval)  {
  }

  void fadeOut() {
    this->SetInterval(2);
    this->ledC->BrightDown();
    if (this->ledC->getColor().b == 0) {
      isFadingOut = false;

      int randomHue = random(0,360);
      float s = this->ledC->getColor().s;
      float b = this->ledC->getColor().b;
      this->ledC->SetColor(Color(randomHue, s, b));
    }
  }

  void fadeIn() {

    this->ledC->BrightUp();
    if (this->ledC->getColor().b == 100) {
      this->SetInterval(initialInterval);
      isFadingOut = true;
    }
  }

  virtual void Animate() {
    if (isFadingOut)
      fadeOut();
    else
      fadeIn();
    // float saturation = this->ledC->getColor().s;
    // float brightness = this->ledC->getColor().b;
    // int randomHue = random(0,360);
    
    // Serial.println("rand:");
    // Serial.println(randomHue);
    // this->ledC->SetColor(Color(randomHue, saturation, brightness));
  }

  private:
  bool isFadingOut = true;
};

class RangeGradient : public Effect {
  private:
  enum Direction {forward, backward};
  Direction direction;
  Color startColor;
  Color endColor;
  uint16_t HueStep;
  byte maxStep = 60;
  uint16_t i = 0;
  float gradientRange;

  void calculateGradientRange() {
      if (startColor.h > endColor.h) 
        this->gradientRange = (360 - startColor.h) + endColor.h;
      else if (startColor.h < endColor.h)
        this->gradientRange = endColor.h - startColor.h;
      else 
        this->gradientRange = 0;
  }

  public:
  RangeGradient(LedControl *ledControl,
                bool isAnimated = true,
                unsigned long interval = 100,
                int hueStep = 1,
                Color startColor = Color(120,100,100),
                Color endColor = Color(160,100,100)
                ) :
  Effect(ledControl, isAnimated, interval)  {
    this->direction = forward;
    this->SetStartEndColors(startColor, endColor);
    this->SetHueStep(hueStep);
  }

  void ResetLed() {
    this->BrightUp(100);
    this->SaturUp(100);
    this->ApplyFullColorRange();
    this->SetHueStep(1);
  }

  void SetStartEndColors(Color start, Color end) {
      this->startColor = start;
      this->endColor = end;
      this->ledC->SetColor(start);
      
      this->calculateGradientRange();
  }

  void SetStartEndHue(float startHue, float endHue) {
      float s = this->ledC->getColor().s;
      float b = this->ledC->getColor().b;
      this->startColor = Color(startHue, s, b);
      this->endColor = Color(endHue, s, b);
      this->ledC->SetColor(this->startColor);
      
      this->calculateGradientRange();
  }

  void SetHueStep(byte step) {
    if (step <= maxStep && step >= 0)
      this->HueStep = step;
  }

  void ApplyFullColorRange() {this->SetStartEndHue(0.0f,359.0f);}
  void ApplyRed()            {this->SetStartEndHue(344.0f,20.0f);}
  void ApplyVoilet()         {this->SetStartEndHue(258.0f,325.0f);}
  void ApplyLightblue()      {this->SetStartEndHue(182.0f,205.0f);}
  void ApplyBlue()           {this->SetStartEndHue(244.0f,252.0f);}
  void ApplyLightgreen()     {this->SetStartEndHue(152.0f,160.0f);}
  void ApplyGreen()          {this->SetStartEndHue(78.0f,140.0f);}
  void ApplyYellow()         {this->SetStartEndHue(47.0f,62.0f);}
  void ApplyOrange()         {this->SetStartEndHue(20.0f,47.0f);}
  void ApplyWhile()          {this->SetStartEndHue(47.0f,62.0f);}

  virtual void Animate() {
    //Serial.println(i);
    // Serial.println("colorsRange");
    // Serial.println(colorsRange);
    // Serial.println("actual color");
    // Serial.println(ledC->getColor().h);
    uint16_t lefttBorder = 0;
    float rightBorder = this->gradientRange;
    if (i > rightBorder) {
      this->direction = backward;
    } else if (i <= lefttBorder) {
      this->direction = forward;
    }
    if (this->direction == forward) {
      i += this->HueStep;
      this->ledC->HueUp(this->HueStep);
    }
    else  {
      i -= this->HueStep;
      this->ledC->HueDown(this->HueStep);
    }
  }
};

class Animator {
  public:
  Animator(Effect *effect) {
    
    if (effect != NULL)
      this->setEffect(effect);
  }

  Effect* getEffect() {
    return this->effect;
  }

  void setEffect(Effect *effect) {
    this->effect = effect;
  }

  void Run() {
    if (this->effect->isAnimated && effect != NULL) {
      if ((this->lastRun + this->effect->GetInterval()) <= millis()) {
        this->lastRun = millis();
        this->effect->Animate();
      }
    }
  }

  private:
  Effect *effect;
  unsigned long lastRun = 0.0f;
};
