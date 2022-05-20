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

#define MAG_READ_INTERVAL 125         //ms between magnetometer reads
#define SENSOR_READ_INTERVAL 500      //ms between environment sensor readout 
// LAT, LON, HEI
#define DEST_COORDS 38.831541514133754, 0.10366977616384526, 0
#define NUM_LEDS 8
#define DATA_PIN D5
#define BLINK_COOLDOWN 2000
#define BLINK_DURATION 25 


Servos servo(45);
MPU9250 mpu;
Adafruit_BMP085 bmp;
DHT22 dht22(DHT22_PIN);
vec3_t g_destCord = {0, 0, 0};
static CRGB leds[NUM_LEDS];

void setup(){
    
    Serial.begin(9600);
    EEPROM.begin(0x80);
    Wire.begin();
    delay(2000); //Pausa para que no se asuste el IMU
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);

    mpu.verbose(true); //TODO: remove this for final build

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
    mpu.verbose(false);
}

void moveServos(vec3_t *gps_pos, float mag_hoz);

void loop(){
    unsigned long ptimep = millis(); // Used to time loop

    // Read GPS
    vec3_t can_position = gps_position();
    comms_gps(can_position.x, can_position.y, can_position.z);
    //comms_debug("PosGPS: (%g, %g, %g)\n", can_position.x, can_position.y, can_position.z);

    static float north_dir = 0;

    // Read IMU
    static unsigned long next_mag_read = millis();
    if (next_mag_read <= millis()) {
        if(mpu.update()) {
            next_mag_read = millis() + MAG_READ_INTERVAL;
            vec3_t magnetometer = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
            north_dir = imu_magHoz(magnetometer.x, magnetometer.y);
            vec3_t gyroscope = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
            vec3_t accelerometer = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};
            comms_debug("Acc: (%g, %g, %g)\n", accelerometer.x, accelerometer.y, accelerometer.z);
            comms_debug("MAGN: (%g, %g, %g)\n", magnetometer.x, magnetometer.y, magnetometer.z);
            //TODO: Pass pointers
            comms_imu(magnetometer, gyroscope, accelerometer, north_dir);
            leds[4] = CRGB::Green;
        } else if(millis() > next_mag_read + 2000){
            leds[4] = CRGB::Red;
        }
    }
    

    moveServos(&can_position, north_dir);
    //float rotation = nav_angle(&can_position, &g_destCord, north_dir);

    
    static float BatV = 0;
    static unsigned long next_sensors_read = millis();
    
    if(millis() >= next_sensors_read) {
        //Read DHT22
        static unsigned long dht22_last_read = 0;
        if(dht22.available()) {
            float temperature = dht22.readTemperature();
            float pressure = bmp.readPressure();
            float humidity = dht22.readHumidity();
            comms_env(temperature, humidity, pressure);
            leds[6] = CRGB::Green;
            dht22_last_read = millis();
        } 
        else if(millis() > dht22_last_read + 2000){
            // If DHT22 hasn't read in a long time we set the LED to red
            leds[6] = CRGB::Red;
        }

        //Read battery voltage
        BatV = analogRead(A0) * 4.7/1023; //150k resistor in series
        comms_bat(BatV);

        next_sensors_read = millis() + SENSOR_READ_INTERVAL;
    }

    // LEDs will blink for BLINK_DURATION, then set off for BLINK_COOLDOWN
    static unsigned long tNextBlink = millis();
    static unsigned long tNextOff;
    
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
    }

    unsigned long looptime = millis() - ptimep;
    comms_debug("Loop took %lu ms", looptime);
}

void moveServos(vec3_t *gps_pos, float mag_hoz){
    float direction;
    float realDirection = 0;
    float error = 0;
    static LowPassFilter lowPass;

    calculate_direction(&direction, DEST_COORDS, gps_pos->x, gps_pos->y, 0);
    
    realDirection = lowPass.low_pass(mag_hoz);
    //direction = map(direction, 0, 2 * PI, 0, 360);

    error = ErrorDireccion(direction, realDirection);

    /*comms_debug("DIR: %g\n", direction);
    comms_debug("REALDIR: %g\n", realDirection);
    comms_debug("MAGHOZ: %g\n", mag_hoz);*/
    Serial.printf("DIR: %g\n", direction);
    Serial.printf("REALDIR: %g\n", realDirection);
    ///comms_debug("MAGHOZ: %g\n", mag_hoz);
    Serial.printf("ERROR: %g\n", error);

    float mappedDirection = error * 0.5;
    //qwertyuiop

    ///comms_debug("MAPPEDDIR %g\n", mappedDirection);

    servo.angleRight(50 + int(mappedDirection));
    servo.angleLeft(50 - int(mappedDirection));

    ///comms_debug("%f	", direction);
}
