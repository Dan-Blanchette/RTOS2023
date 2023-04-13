/**
 * @file main.cpp
 * @author Dan Blanchette
 * @brief  This program will run a web sever on the ESP32 Core 0.
 * RTOS tasks for 2 I2C devices(temp/hum sensor, sunlight sensor) and the stepper motor on ESP32's processor
 * are ran on core 1
 * @version 0.1
 * @date 2023-04-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "devices.h"

// Task handles
TaskHandle_t webServerTask;
TaskHandle_t RTOS_Tasks;

// Web Server Setup
AsyncWebServer server(80);

// Home WiFi credentials
// censor this before submitting or pushing to git
const char *ssid = "someones-network";
const char *password = "some-password";

// Task 1 function
void task1(void *parameter)
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
      //  Serial.println("Task 2 is running...\n");
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
   setup_stepper();

   // Connect to WiFi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
      delay(1000);
      Serial.println("Connecting to WiFi...");
   }
   Serial.println("Connected to WiFi");
   Serial.println(WiFi.localIP());

   // Start the server
   server.begin();

   // Create tasks
   xTaskCreatePinnedToCore(task1, "Task 1", 10000, NULL, 4, &webServerTask, core_zero);
   xTaskCreatePinnedToCore(Task_Stepper, "Task_Stepper", 10000, NULL, 4, &RTOS_Tasks, core_one);
   // xTaskCreatePinnedToCore(Task_HDC1080, "Task_HDC1080", 10000, NULL, 2, &RTOS_Tasks, core_one);
   // xTaskCreatePinnedToCore(Task_sunSensor, "Task_sunSensor", 10000, NULL, 2, &RTOS_Tasks, core_one);
}

void loop()
{
   // Nothing to do here
}
