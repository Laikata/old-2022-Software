#pragma once

class Servos
{
  public:
    Servos(int ServosVelocity);
    void angleRight(int angle);
    void angleLeft(int angle);
    void attach();
    void detach();
  private:
    int lastAngleRight;
    int lastAngleLeft;
    unsigned long actualTime;
    unsigned long lastTimeLeft;
    unsigned long lastTimeRight;
    unsigned long interval;
};