/**
 * @file main.cpp
 * @author Dan Blanchette
 * @brief  This program will run a web sever on the ESP32 Core 0.
 * RTOS tasks for 2 I2C devices(temp/hum sensor, sunlight sensor) and the stepper motor on ESP32's processor
 * are ran on core 1.a
 * Credit: James Lasso for help with RESTful and IoT server functionality.
 * @version 0.1
 * @date 2023-04-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "devices.h"
#include <string.h>

// Task handles
TaskHandle_t webServerTask;
TaskHandle_t iotServerTask;
TaskHandle_t RTOS_Tasks;

// Web Server Setup
AsyncWebServer server(80);

// HTTPClient object
HTTPClient http;

// Home WiFi credentials
// censor this before submitting or pushing to git
const char *ssid = "TP_LINK_822A";
const char *password = "86624107";

// detect server IP address
String serverDet = "http://52.23.160.25:5000/IOTAPI/DetectServer";
String serverReg = "http://52.23.160.25:5000/IOTAPI/RegisterWithServer";

// Task 1 function
void Task_ESP32_Serv(void *parameter)
{
   while (1)
   {

      // printf("Task 1 is running...\n");
      // Set up routes for web server
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(200, "text/plain", "Hello, world"); });
      server.on("/hdc-temp", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(200, "text/plain", "Temp = 68 F"); });
      vTaskDelay(10000 / portTICK_PERIOD_MS);
   }
}

void Task_IoT_Server(void *parameter)
{
   while (1)
   {
      http.addHeader("Content-Type", "application/json");

      // send HTTP POST and Store Response(POST return value)
      int httpResponseVal = http.POST("{\"key\":\"2436e8c114aa64ee\"}");
      String response = http.getString();

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseVal);
      Serial.println(response);
   }
}

/**
 * @brief Stepper Motor Task
 *
 * @param parameter
 */
void Task_Stepper(void *parameter)
{
   while (1)
   {
      // Serial.println(WiFi.localIP());
      Serial.println("Stepper is running CCW...\n");
      // CCW
      for (int i = 1; i < 9; i++)
      {
         stepper_move(i);
         vTaskDelay(10 / portTICK_PERIOD_MS);
      }
   }
}

// void Task_HDC1080(void *parameter)
// {
//   while (1)
//   {

//     vTaskDelay(2000 / portTICK_PERIOD_MS);
//   }
// }

// void Task_sunSensor(void *Parameters)
// {
//    while (1)
//    {
//       vTaskDelay(5000 / portTICK_PERIOD_MS);
//    }
// }

void setup()
{
   Serial.begin(115200);
   // init stepper pins
   setup_stepper();

   /************** WIFI SETUP ******************/
   // Connect to WiFi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
      delay(1000);
      Serial.println("Connecting to WiFi...");
   }
   Serial.println("Connected to WiFi");
   Serial.println(WiFi.localIP());

   // WiFiClient client;


   // Start up an ESP32 Web Server
   server.begin();

   // Begin new connection to cloud website
   //    http.begin("http://52.23.160.25:5000/");
   // int detServer = http.begin(serverDet);
   int regServer = http.begin(serverReg);
   Serial.println("IoT Server Connection: 1 = connected, 0 = error connecting: ");
   Serial.printf("Connection Status: %d\n", regServer);

   // Register Device with the server
   http.addHeader("Content-Type", "application/json");
   // Send POST code for registration
   int postCode = http.POST("{\"key\":\"2436e8c114aa64ee\",\"iotid\":1001}");
   String response = http.getString();
   Serial.print("HTTP Response Code: ");
   Serial.println(postCode);
   Serial.println(response);

   /** Create Tasks **/

   // Web Server and IoT RTOS Server Tasks
   // xTaskCreatePinnedToCore(Task_ESP32_Serv, "Task_ESP32_Serv", 10000, NULL, 3, &webServerTask, core_zero);
   // xTaskCreatePinnedToCore(Task_IoT_Server, "Task_IoT_Server", 10000, NULL, 4, &iotServerTask, core_zero);

   // RTOS Tasks for Connected Devices  
   xTaskCreatePinnedToCore(Task_Stepper, "Task_Stepper", 10000, NULL, 4, &RTOS_Tasks, core_one);
   // xTaskCreatePinnedToCore(Task_HDC1080, "Task_HDC1080", 10000, NULL, 2, &RTOS_Tasks, core_one);
   // xTaskCreatePinnedToCore(Task_sunSensor, "Task_sunSensor", 10000, NULL, 2, &RTOS_Tasks, core_one);
}

void loop()
{
   // Nothing to do here
}
