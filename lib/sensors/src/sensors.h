#pragma one



class sensors
{
    public:
        sensors(int AM2302Pin);
        float bmpAltitude();
        float bmpPressure();
        float bmpSealevelPressure();
        float amTemperature();
        float amHumidity();
        void mpuCalibrateAccel();
        void mpuCalibrateMag();
        float mpuAccelerometer();
        float mpuGyro();
        float mpuMagnetometer();
};