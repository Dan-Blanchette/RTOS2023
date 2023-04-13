#ifndef DEVICES_H
#define DEVICES_H
// ESP32 Web Server Libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
// Device Libraries
#include <Arduino.h>

#define STEP_IN1 13
#define STEP_IN2 12
#define STEP_IN3 18
#define STEP_IN4 19

// ESP32 Core Assignment
// webserver core
static int core_zero = 0;
// RTOS core
static int core_one = 1;



// Setup Functions
void setup_HDC1080();
void setup_stepper();
void setup_light_sensor();

// Device Programs
void stepper_move(int step);

#endif