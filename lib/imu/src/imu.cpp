#include "imu.h"

float imu_magHoz(float const magX, float const magY) {
    return atan2(magX, magY) * 180 / PI;
}