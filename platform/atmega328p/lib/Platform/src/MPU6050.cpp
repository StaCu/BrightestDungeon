#include "Platform.h"

#ifdef MPU6050_CONTROLLER

#include <I2Cdev.h>
#include <MPU6050.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 MPU;
int8_t angle_buffer[2];

void controller_init() {    
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
    #endif
    MPU.initialize();

    if (MPU.testConnection()) {
        LOG_LN(F("Controller detected"));
    } else {
        LOG_LN(F("Controller not detected"));
    }
}

void controller_callibrate() {
  
}

int8_t controller_poll_angle() {
    int8_t angle;
    int16_t a[3], g[3];
    MPU.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]);
    angle = a[MPU6050_CONTROLLER_ORIENTATION] / 160;

    // find median angle
    int8_t median_angle = angle;
    if (angle_buffer[0] < median_angle) {
        if (angle_buffer[1] < median_angle) {
            median_angle = angle_buffer[0] > angle_buffer[1] ? angle_buffer[0] : angle_buffer[1];
        }
    } else if (angle_buffer[1] > median_angle) {
        median_angle = angle_buffer[0] < angle_buffer[1] ? angle_buffer[0] : angle_buffer[1];
    }

    // add new sample to angle_buffer
    angle_buffer[1] = angle_buffer[0];
    angle_buffer[0] = angle;

    angle = median_angle;

    if (angle > MPU6050_CONTROLLER_DEADZONE) {
        angle -= MPU6050_CONTROLLER_DEADZONE;
    } else if (angle < -MPU6050_CONTROLLER_DEADZONE) {
        angle += MPU6050_CONTROLLER_DEADZONE;
    } else {
        angle = 0;
    }

    return angle;
}

#endif
