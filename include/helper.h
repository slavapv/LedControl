#include <Arduino.h>

enum ColorMode {};

struct LedPins {
  public:
  int red;
  int green;
  int blue;
  public:
  LedPins(int r = 0, int g = 0, int b = 0) : red(r), green(g), blue(b) {}
};

struct Color {
    float h, s, b;
    Color(float hue = 0.0f, float saturation = 0.0f, float brightness = 0.0f) :
    h(hue), s(saturation), b(brightness) {}
};

class LedStrip {
  public:
  LedStrip(LedPins ledPins) {
    setupPins(ledPins);
  }

  protected:
  void setupPins(LedPins pins) {
    ledPins = pins;
    pinMode(ledPins.red, OUTPUT);
    pinMode(ledPins.green, OUTPUT);
    pinMode(ledPins.blue, OUTPUT);
  }

  float GetH() {
    return this->hue;
  }

  void SetH(float hue) {
    if (hue <= 360.0f && hue >= 0.0f) {
      this->hue = hue;
      this->UpdateColor();
    }
  }

  float GetS() {
    return this->saturation;
  }

  void SetS(float saturation) {
    if (saturation <= 100.0f && saturation >= 0.0f) {
      this->saturation = saturation;
      this->UpdateColor();
    }
  }
  
  float GetB() {
    return this->brightness;
  }

  void SetB(float brightness) {
    if (brightness <= 100.0f && brightness >= 0.0f) {
      this->brightness = brightness;
      this->UpdateColor();
    }
  }

  void setHSB(float h, float s, float b) {
    if ((h <= 360.0f && h >= 0.0f)
    && (s <= 100.0f && s >= 0.0f) 
    && (b <= 100.0f && b >= 0.0f)) {
      this->hue = h;
      this->saturation = s;
      this->brightness = b;
      this->UpdateColor();
    }
  }

  private:
  float hue = 0.0f;
  float saturation = 0.0f;
  float brightness = 0.0f;
  LedPins ledPins;

  void UpdateColor() {
    int *color;
    color = HSBtoRGB();
    analogWrite(this->ledPins.red, color[0]);
    analogWrite(this->ledPins.green, color[1]);
    analogWrite(this->ledPins.blue, color[2]);
  }

  int* HSBtoRGB() {
    float r, g, b;
    float h = this->hue;
    float s = this->saturation/100;
    float v = this->brightness/100;
    float cc = s * v;
    float hh = h/60.0;
    float x = cc * (1-abs(fmod(hh, 2)-1));
    float m = v - cc;
    
    if (h >= 0 && h < 60) {
      r = cc, g = x, b = 0;
    }
    else if (h >= 60 && h < 120) {
      r = x, g = cc, b = 0;
    }
    else if (h >= 120 && h < 180) {
      r = 0, g = cc, b = x;
    }
    else if (h >= 180 && h < 240) {
      r = 0, g = x,b = cc;
    }
    else if (h >= 240 && h < 300) {
      r = x, g = 0, b = cc;
    }
    else if (h >= 300 && h < 360) {
      r = cc, g = 0, b = x;
    }

    static int rgbColor[3];
    rgbColor[0] = (byte)((r + m) * 255);
    rgbColor[1] = (byte)((g + m) * 255);
    rgbColor[2] = (byte)((b + m) * 255);

    return rgbColor;

  }
};

class LedControl : public LedStrip {
  public:
  LedControl(LedPins ledPins) : LedStrip(ledPins) {}

  Color getColor() {
    return Color(this->GetH(), this->GetS(), this->GetB());
  }

  void SetColor(Color color) {
    this->setHSB(color.h, color.s, color.b);
  }

  void HueUp(uint16_t step = 1) {
    float h = this->GetH();
    if (step > 360 || step < 0) { Serial.println(); return;};
    
    if ((h + step) >= 360) 
      h = abs((h + step) - 360);
    else
      h+=step;

    this->SetH(h);
  }

  void HueDown(uint16_t step = 1) {
    float h = this->GetH();
    if ((h - step) <= 0) 
      h = 360 - abs(h - step);
    else
      h-=step;

    this->SetH(h);
  }

  void SaturDown(uint8_t step = 1) {
    float saturation = this->GetS();
    if ((saturation - step) <= 0) 
      saturation = 0;
    else 
      --saturation;

    this->SetS(saturation);
  }

  void SaturUp(uint8_t step = 1) {
    float saturation = this->GetS();
    if ((saturation + step) >= 100)
      saturation = 100;
    else 
      saturation += step;
    this->SetS(saturation);
  }

  void BrightDown(uint8_t step = 1) {
    float brightness = this->GetB();
    if ((brightness - step) <= 0 ) 
      brightness = 0;
    else
      --brightness;

    this->SetB(brightness);
  }

  void BrightUp(uint8_t step = 1) {
    float brightness = this->GetB();
    if ((brightness + step) >= 100)
      brightness = 100;
    else
      ++brightness;

    this->SetB(brightness);
  }

};

class Effect {
  private:
  unsigned long interval;

  protected:
  LedControl *ledC;

  public:
  bool isAnimated;

  Effect(LedControl *ledControl, 
        bool isAnimated = false,
        unsigned long interval = 50) {
    this->ledC = ledControl;
    this->interval = interval;
    this->isAnimated = isAnimated;
  }
  
  float GetInterval() {
    return this->interval;
  }

  void SetInterval(unsigned long interval) {
    this->interval = interval;
  }

  void SpeedDown(uint8_t step = 1) {
    this->interval += step;
  }

  void SpeedUp(uint8_t step = 1) {
    if ((this->interval - step) <= 0)
      this->interval = 0;
    else
      this->interval-=step;
  }

  virtual void BrightUp(uint8_t step = 1) {
    this->ledC->BrightUp(step);
  }

  virtual void BrightDown(uint8_t step = 1) {
    this->ledC->BrightDown(step);
  }

  virtual void SaturUp(uint8_t step = 1) {
    this->ledC->SaturUp(step);
  }

  virtual void SaturDown(uint8_t step = 1) {
    this->ledC->SaturDown(step);
  }

  virtual void Animate();

};
