#ifndef DEVICES_H
#define DEVICES_H
// ESP32 Web Server Libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

// #include <WebServer.h>
#include <ESPAsyncWebServer.h>
// Device Libraries
#include <Arduino.h>


// I2C PINS
#define SCL 22
#define SDA 23


// Stepper PINS
#define STEP_IN1 15
#define STEP_IN2 12
#define STEP_IN3 4
#define STEP_IN4 5




//Assign output variabless to GPIO pins
#define output13  13
//#define output27 27


// Vandalino Buttons
// #define BUTTON_LEFT 18
// #define BUTTON_RIGHT 15
// #define BUTTON_MID 32

// ESP32 Core Assignment
// webserver core
static int core_zero = 0;
// RTOS core
static int core_one = 1;

// State Variables for ESP32 Local Webserver
// bool stepCCW = false; 
// bool stepCW = true;


// Setup Functions
void setup_HDC1080();
void setup_stepper();
void d13_setup();
// void setup_buttons();
void setup_light_sensor();

// Device Functions
void stepper_move(int step);
void step_dir(int direction);
void displaySensorDetails(void *parameters);
// int set_state();
// void get_state();



#endif