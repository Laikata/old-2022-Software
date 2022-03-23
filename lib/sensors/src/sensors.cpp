
#include <sensors.h>

#include <Arduino.h>
#include <Adafruit_BMP085.h>
#include <ErriezDHT22.h>
#include <MPU9250.h>

DHT22 dht22;

Adafruit_BMP085 bmp;
MPU9250 mpu;

float sensors_bmpAltitude(){
    return(bmp.readAltitude());
}

float sensors_bmpPressure(){
    return(bmp.readPressure());
}

float sensors_bmpSealevelPressure(){
    return(bmp.readSealevelPressure());
}

float sensors_amTemperature(){
    return(dht22.readTemperature());
}

float sensors_amHumidity(){
    return(dht22.readHumidity());
}

//Leave the cansat on a flat plane
void sensors_mpuCalibrateAccel(){
    mpu.calibrateAccelGyro();
}

//Wave cansat in a figure eight until done
void sensors_mpuCalibrateMag(){
    mpu.calibrateMag();
}

float sensors_mpuAccelerometer(){
    float _accelerometer[3] = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};
    return (_accelerometer[3]);
}

float sensors_mpuGyro(){
    float _gyro[3] = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
    return (_gyro[3]);
}

float sensors_mpuMagnetometer(){
    float _Magnetometer[3] = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
    return (_Magnetometer[3]);
}
