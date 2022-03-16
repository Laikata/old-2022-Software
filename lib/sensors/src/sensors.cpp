
#include <sensors.h>

#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <ErriezDHT22.h>
#include <MPU9250.h>

DHT22 dht22;

Adafruit_BMP085 bmp;
MPU9250 mpu;


sensors::sensors(int AM2302Pin)
{
  bmp.begin();
  DHT22 dht22 = DHT22(AM2302Pin);

}

float sensors::bmpAltitude(){
    return(bmp.readAltitude());
}

float sensors::bmpPressure(){
    return(bmp.readPressure());
}

float sensors::bmpSealevelPressure(){
    return(bmp.readSealevelPressure());
}

float sensors::amTemperature(){
    return(dht22.readTemperature());
}

float sensors::amHumidity(){
    return(dht22.readHumidity());
}

//Leave the cansat on a flat plane
void sensors::mpuCalibrateAccel(){
    mpu.calibrateAccelGyro();
}

//Wave cansat in a figure eight until done
void sensors::mpuCalibrateMag(){
    mpu.calibrateMag();
}

float sensors::mpuAccelerometer(){
    float _accelerometer[3] = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};
    return (_accelerometer[3]);
}

float sensors::mpuGyro(){
    float _gyro[3] = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
    return (_gyro[3]);
}

float sensors::mpuMagnetometer(){
    float _Magnetometer[3] = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
    return (_Magnetometer[3]);
}
