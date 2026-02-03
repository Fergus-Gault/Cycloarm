#pragma once

#include <stdint.h>
#include <Arduino.h>

constexpr uint8_t NUM_JOINTS = 3;

constexpr uint8_t EN_PIN = 8;

constexpr uint8_t STEP_PINS[3] = {2,  // Base yaw
                                  3,  // Shoulder
                                  4}; // Elbow

constexpr uint8_t DIR_PINS[3] = {5,  // Base yaw
                                 6,  // Shoulder
                                 7}; // Elbow

constexpr int8_t ENDSTOP_PINS[3] = {-1,  // Base yaw (no endstop)
                                    10,  // Shoulder
                                    11}; // Elbow

constexpr int GEAR_RATIO[3] = {25,
                               45,
                               45};