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

#define magReadInterval 125         //ms between magnetometer reads
#define sensorReadInterval 500      //ms between environment sensor readout 
// LAT, LON, HEI
#define DEST_COORDS 38.831541514133754, 0.10366977616384526, 0
#define NUM_LEDS 8
#define DATA_PIN D5
#define PeriodoParpadeo 2000
#define TiempoParpadeo 25 


Servos servo(5);
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
    //TODO: Add calibrate branch to main repo
    //mpu.setup(0x68);
    mpu.verbose(true);

    while (!mpu.setup(0x68)) {
        comms_debug("MPU connection failed. Will try again in 2s.");
        delay(2000);
    }
    
    //PWR_MGMT_1

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

    unsigned long ptimep = millis();
    vec3_t can_position = gps_position();
    comms_gps(can_position.x, can_position.y, can_position.z);
    comms_debug("PosGPS: (%g, %g, %g)\n", can_position.x, can_position.y, can_position.z);

    // Recibir datos sensores

    static float north_dir = 0;

    static uint32_t next_mag_read = millis() + magReadInterval;
    static uint32_t last_mag_read = millis();
    if (millis() > next_mag_read) {
        if(mpu.update()) {
            next_mag_read = millis() + magReadInterval;
            vec3_t magnetometer = {mpu.getMagX(), mpu.getMagY(), mpu.getMagZ()};
            north_dir = imu_magHoz(magnetometer.x, magnetometer.y);
            vec3_t gyroscope = {mpu.getGyroX(), mpu.getGyroY(), mpu.getGyroZ()};
            vec3_t accelerometer = {mpu.getAccX(), mpu.getAccY(), mpu.getAccZ()};
            comms_debug("Acc: (%g, %g, %g)\n", accelerometer.x, accelerometer.y, accelerometer.z);
            comms_debug("MAGN: (%g, %g, %g)\n", magnetometer.x, magnetometer.y, magnetometer.z);
            //TODO: Pass pointers
            comms_imu(magnetometer, gyroscope, accelerometer, north_dir);
            last_mag_read = millis();
        }
        if(millis()> last_mag_read + 2000){
            leds[4] = CRGB::Red;
        }
        else {
            leds[4] = CRGB::Green;
        }
        
    }


    
    moveServos(&can_position, north_dir);
    //float rotation = nav_angle(&can_position, &g_destCord, north_dir);

    //   1. Voltaje bateria
    //   2. Temperatura
    //   3. Presión
    // TODO: Read battery voltage


    static float VBat = -1.f;
    static bool VDht22 = true;
    static uint32_t next_sensors_read = millis() + sensorReadInterval;
    static uint32_t last_dht_read = millis();
    if(millis() > next_sensors_read) {  
        float VRead = analogRead(A0);
        VBat = VRead * 4.7/1023; //150k resistor in series
        VDht22 = dht22.available();      
        if (VDht22 == true){
            last_dht_read = millis();
            leds[6] = CRGB::Green;
        }else if(millis() > last_dht_read + 5000){
            leds[6] = CRGB::Red;
        }

        float temperature = dht22.readTemperature(); // Do we read temp from the BMP085 or the DHT22?
        float pressure = bmp.readPressure();
        float humidity = dht22.readHumidity();
        comms_env(temperature, humidity, pressure);
        //hacer tx bateria
        Serial.printf("Sensores: Temp/press/hum/VBat(%g, %g, %g, %g)", temperature, pressure, humidity,VBat);
        next_sensors_read = millis() + sensorReadInterval;
    }
    static unsigned long tNextLed = millis();
    static unsigned long tNextOff;
    
    // TODO: Read battery voltage
    unsigned long looptime = millis() - ptimep;
    Serial.print("Tiempo LOOP: ");
    Serial.println(looptime);

    
    if (millis() > tNextLed) {
        tNextLed = millis() + PeriodoParpadeo;
        if (VBat >= 3.8) {
            leds[0] = CRGB::Green;
        }else if (VBat > 3.6) {
            leds[0] = CRGB::Orange;
        }else {
            leds[0] = CRGB::Red;
        }
        FastLED.show();
        tNextOff = millis() + TiempoParpadeo;

    }
    if (millis()>tNextOff){
        for (int i = 0; i < 8; i++)
        {
            leds[i] = CRGB::Black;
        }
        FastLED.show();
        tNextOff = millis()+ 999999999;
      
        next_sensors_read = millis() + sensorReadInterval;
    }
    // TODO: Read battery voltage
}



void moveServos(vec3_t *gps_pos, float mag_hoz){
    float direction;
    float realDirection = 0;
    static LowPassFilter lowPass;

    calculate_direction(&direction, DEST_COORDS, gps_pos->x, gps_pos->y, 0);
    
    realDirection = lowPass.low_pass(mag_hoz);
    direction = map(direction, 0, 2 * PI, 0, 360);

    direction = direction - realDirection;
    comms_debug("DIR: %g\n", direction);
    comms_debug("REALDIR: %g\n", realDirection);
    comms_debug("MAGHOZ: %g\n", mag_hoz);

    float mappedDirection = map(direction, -180, 180, -50, 50);

    servo.angleRight(50 + mappedDirection);
    servo.angleLeft(50 - mappedDirection);

    comms_debug("%f	", direction);
}
