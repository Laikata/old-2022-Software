#include "imu.h"

float imu_magHoz(float magX, float magY) {
    return atan2(magX, magY) * 180 / PI;
}