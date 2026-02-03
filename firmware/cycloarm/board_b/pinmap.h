#pragma once

#include <stdint.h>
#include <Arduino.h>

constexpr uint8_t NUM_JOINTS = 3;

constexpr uint8_t EN_PIN = 8;

constexpr uint8_t STEP_PINS[3] = {2,  // Wrist roll
                                  3,  // Wrist left
                                  4}; // Wrist right

constexpr uint8_t DIR_PINS[3] = {5,  // Wrist roll
                                 6,  // Wrist left
                                 7}; // Wrist right

constexpr int8_t ENDSTOP_PINS[3] = {-1,  // Wrist roll (no endstop)
                                    -1,  // Wrist left (no endstop)
                                    -1}; // Wrist right (no endstop)

constexpr int GEAR_RATIO[3] = {20,
                               3,
                               3};