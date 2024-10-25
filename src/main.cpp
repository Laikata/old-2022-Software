#include <vector.h>
#include <gps.h>
#include <servos.h>
#include <dir.h>
#include <Arduino.h>
#include <Servo.h>
#include <comms.h>
#include <Adafruit_BMP085.h>
#include <ErriezDHT22.h>
#include <MPU9250.h>
#include <imu.h>
#include "eeprom_utils.h"
#include <LowPass.h>
#include <FastLED.h>
#include <SoftwareSerial.h>

#define DHT22_PIN D4

#define IMU_SEND_INTERVAL 200
#define IMU_READ_INTERVAL 10         //ms between magnetometer reads
#define SENSOR_READ_INTERVAL 500      //ms between environment sensor readout

// LAT, LON, HEI
//#define DEST_COORDS  0.103451, 50.0, 0  
#define DEST_COORDS  -3.683591, 37.29819, 0  
#define SEA_LEVEL_PRESSURE 101325
//0.103451, 38.8345
//#define ACT_COORDS 38.892754502280750, 0.0, 0
#define NUM_LEDS 8
#define DATA_PIN D5
#define BLINK_COOLDOWN 2000
#define BLINK_DURATION 25 
#define ERROR_MULTIPLICATION 0.8
/////IMPORTANTE IR CAMBIANDO PARA ENCONTRAR LA BUENA YERAY
#define KP 1.5
#define KD 0.5


Servos servo(45);
MPU9250 mpu;
TinyGPSPlus gps;
Adafruit_BMP085 bmp;
DHT22 dht22(DHT22_PIN);
vec3_t g_destCord = {0, 0, 0};
static CRGB leds[NUM_LEDS];
bool isInSky = false;

void setup(){
    
    Serial.begin(9600);
    EEPROM.begin(0x80);
    Wire.begin();
    delay(2000); //Pausa para que no se asuste el IMU
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    //BRG,BGR GBR RGB
    //RGB
    //


    #ifdef DEBUG
    mpu.verbose(true); //TODO: remove this for final build
    #endif

    while (!mpu.setup(0x68)) {
        comms_debug("MPU connection failed. Will try again in 2s.");
        delay(2000);
    }

    dht22.begin();
    bmp.begin();
    gps_init();
    servo.attach();

    comms_debug("Starting Program");;

    loadCalibration();
    printCalibration();
    delay(1000);
    
}

void moveServos(dvec3_t *gps_pos, float mag_hoz);

void loop(){
    unsigned long static ptimep = millis(); // Used to time loop
    unsigned long gpstime = millis();

    // Read GPS
    static dvec3_t can_position = {0, 0, 0};
    gps_update();
    static uint32_t last_gps_read = millis();
    static uint32_t next_gps_read = millis();

    if(gps.location.isUpdated()) {
        can_position = gps_position();
        comms_gps(can_position.x, can_position.y, can_position.z);
        last_gps_read = millis();
        leds[2] = CRGB::Green;
    } else if ( millis() > last_gps_read + 3000){
        leds[2] = CRGB::Red;
        //last_gps_read = millis() + 99999;
    }
    
    //comms_debug("PosGPS: (%g, %g, %g)\n", can_position.x, can_position.y, can_position.z);
    gpstime = millis() - gpstime;

    static float north_dir = 0;

    // Read IMU
    static unsigned long next_imu_read = millis();
    static unsigned long next_imu_send = millis();

    unsigned long IMUtime = millis();
    if (next_imu_read <= millis()) {
        if(mpu.update()) {
            mpu.update();
            next_imu_read = millis() + IMU_READ_INTERVAL;
            vec3_t magnetometer = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
            north_dir = imu_magHoz(magnetometer.x, magnetometer.y);
            vec3_t gyroscope = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
            vec3_t accelerometer = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};
            
            if(next_imu_send <= millis()) {
                next_imu_send = millis() + IMU_SEND_INTERVAL;
                comms_imu(magnetometer, accelerometer, gyroscope, north_dir);
            }

            #ifdef DEBUG
            Serial.printf("MAGN: (%g, %g, %g)\n", magnetometer.x, magnetometer.y, magnetometer.z);
            Serial.printf("NORTHDIR: %g\n", north_dir);
            #endif
            
            leds[4] = CRGB::Green;
        } else if(millis() > next_imu_read + 2000){
            leds[4] = CRGB::Red;
        }
    }

    IMUtime = millis() - IMUtime;
    unsigned long SERVOStime = millis();
    moveServos(&can_position, north_dir);
    //float rotation = nav_angle(&can_position, &g_destCord, north_dir);
    SERVOStime = millis() - SERVOStime;
    
    static unsigned long next_sensors_read = millis();
    static uint32_t last_dht_read = millis();
    static float BatV = -1.f;
    unsigned long Sensortime = millis();

    if(millis() >= next_sensors_read) {
        if(servo.isAttached() == true){
            leds[7] = CRGB::Green;
        }else{
            leds[7] = CRGB::Red;
        }
        next_sensors_read = millis() + SENSOR_READ_INTERVAL;
        float VRead = analogRead(A0);
        BatV = VRead * 4.7/1023; //150k resistor in series  
        comms_bat(BatV);
        
        if (dht22.available() == true){
            last_dht_read = millis();
            leds[6] = CRGB::Green;
        }else if(millis() > last_dht_read + 5000){
            leds[6] = CRGB::Red;
            //last_dht_read += 99999999;
        }

        float temperature = dht22.readTemperature();
        float pressure = bmp.readPressure();
        float humidity = dht22.readHumidity();
        //float altitude = 44330 * (1 - pow(pressure/SEA_LEVEL_PRESSURE, 1/5.255));
        comms_env(temperature, humidity, pressure);
    }
    Sensortime = millis() - Sensortime;

    // LEDs will blink for BLINK_DURATION, then set off for BLINK_COOLDOWN
    static unsigned long tNextBlink = millis();
    static unsigned long tNextOff;

    unsigned long LEDtime = millis();
    if (millis() >= tNextBlink) {
        tNextBlink = millis() + BLINK_COOLDOWN;
        if (BatV >= 3.8) {
            leds[0] = CRGB::Green;
        }else if (BatV > 3.6) {
            leds[0] = CRGB::Orange;
        }else {
            leds[0] = CRGB::Red;
        }
        FastLED.show();
        tNextOff = millis() + BLINK_DURATION;
    }
    if (millis() >= tNextOff){
        for (int i = 0; i < 8; i++)
        {
            leds[i] = CRGB::Black;
        }
        FastLED.show();
        tNextOff += 9999999;
    }
    LEDtime = millis() - LEDtime;

    #ifdef DEBUG
    unsigned long looptime = millis() - ptimep;
    ptimep = millis();
    Serial.printf("GPS %lu\n", gpstime);
    Serial.printf("IMU %lu\n", IMUtime);
    Serial.printf("SENSORS %lu\n", Sensortime);
    Serial.printf("LED %lu\n", LEDtime);
    Serial.printf("Servos %lu\n",SERVOStime);
    Serial.printf("Loop took %lu ms\n", looptime);
    #endif
    unsigned long looptime = millis() - ptimep;
    ptimep = millis();
    //Serial.printf("Loop took %lu ms\n", looptime);
}

float ErrorDireccion(float bearing, float target){
    float error = bearing - target;
    if(error == 0) return 0;
    if(error > 180) error -= 360;
    else if(error < -180) error += 360;
    return error;
}

float PDError (float direction, float realDirection){
    float error = 0;
    float dev = 0;
    float pastError = 0;
    error = ErrorDireccion(direction, realDirection);
    dev = error - pastError;
    pastError = error;
    return error * KP + dev * KD;
}

void moveServos(dvec3_t *gps_pos, float mag_hoz){
    float direction;
    float realDirection = 0;
    float error = 0;
    static LowPassFilter lowPass;

    calculate_direction(&direction, DEST_COORDS, gps_pos->x, gps_pos->y, 0);
    
    realDirection = lowPass.low_pass(mag_hoz);
    //direction = map(direction, 0, 2 * PI, 0, 360);

    error = -ErrorDireccion(realDirection, direction);
    

    #ifdef DEBUG
    Serial.printf("ERROR: %g\n", error);
    Serial.printf("DIR: %g\n", direction);
    Serial.printf("REALDIR: %g\n", realDirection);
    Serial.printf("PosGPS: (%g, %g, %g)\n", gps_pos->x, gps_pos->y, gps_pos->z);
    #endif


    float mappedDirection = error * KP;

    ///comms_debug("MAPPEDDIR %g\n", mappedDirection);

    servo.angleRight(100 + (int)mappedDirection);
    servo.angleLeft(100 - (int)mappedDirection);

    ///comms_debug("%f	", direction);
}
