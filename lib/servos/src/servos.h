#pragma once

class Servos
{
  public:
    Servos(int rightPin, int leftPin, int ServosVelocity);
    void angleRight(int angle);
    void angleLeft(int angle);
    void attach();
    void detach();
  private:
    int lastAngleRight;
    int lastAngleLeft;
    int _rightPin;
    int _lefttPin;
    unsigned long actualTime;
    unsigned long lastTimeLeft;
    unsigned long lastTimeRight;
    unsigned long interval;
};