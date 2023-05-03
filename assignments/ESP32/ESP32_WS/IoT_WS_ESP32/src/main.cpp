/**
 * @file main.cpp
 * @author Dan Blanchette
 * @brief  This program will run a web sever on the ESP32 Core 0.
 * RTOS tasks for 2 I2C devices(temp/hum sensor, sunlight sensor) and the stepper motor on ESP32's processor
 * are ran on core 1.
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

// HDC1080 Class Object
ClosedCube_HDC1080 hdc1080;

// current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in miliseconds
const long timeoutTime = 2000;

// HTTPClient object
HTTPClient http;

// Queue Handles for stepper direction
static QueueHandle_t xStateQueue = NULL;

// States for debouncing buttons
int lastState = LOW;
int currentState;

// Web Server Setup
// AsyncWebServer server(80);
WiFiServer server(80);
String header;

// Auxiliar variables to store current output state
String output13State = "off";
// String output27State = "off";


// detect server IP address
String serverDet = "http://52.23.160.25:5000/IOTAPI/DetectServer";
String serverReg = "http://52.23.160.25:5000/IOTAPI/RegisterWithServer";

// Home WiFi credentials
// censor this before submitting or pushing to git
const char *ssid = "sending-stone";
const char *password = "4579W!$hThis#";


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
      int rec_val;
      // NOTE: step_dir(bool dir) has vTaskDelay(10 / portTICK_PERIOD_MS)
      // Flag Directions (True = CW, False = CCW)
      xQueueReceive(xStateQueue, &rec_val, 0U);
      // Serial.print("Rec Val: ");
      // Serial.println(rec_val);
      step_dir(rec_val);
   }
}



void Task_HDC1080(void *parameter)
{
  while (1)
  {
    // convert celsius to Fahr
    float celsius = hdc1080.readTemperature();
    float fahr = ((celsius * 1.8) + 32);

    Serial.print("T=");
    Serial.print(hdc1080.readTemperature());
    Serial.println("C");

    Serial.print("T=");
    Serial.print(fahr);
    Serial.print("F RH=");
    Serial.print(hdc1080.readHumidity());
    Serial.println("%");
    // read once every 3 seconds per port tick
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

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
   /****************RTOS DEVICES***********/
   // Stepper Motor Setup
   setup_stepper();
   // D13 LED Setup
   d13_setup();
  
  /*****************HDC1080(Temp/Humidity Sensor) AND TSL2591(Sunlight Sensor)***************/

  // Enable communication with the I2C Bus
  Wire.begin(SDA, SCL);

  /********HDC1080****************/
  // setup defaults for HDC1080
  hdc1080.begin(0x40);
  Serial.print("Manufacturer ID=0x");
  Serial.println(hdc1080.readManufacturerId(), HEX);
  Serial.print("Device ID=0x");
  Serial.println(hdc1080.readDeviceId(), HEX);


  /**********QUEUE INSTANTIATION********/
  xStateQueue = xQueueCreate(1, sizeof(int));

  // ESP.restart();


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

   WiFiClient client;

   // Create the state queue
   xStateQueue = xQueueCreate(1, sizeof(int));

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
   // xTaskCreatePinnedToCore(Task_IoT_Server, "Task_IoT_Server", 10000, NULL, 4, &iotServerTask, core_zero);

   // RTOS Tasks for Connected Devices
   xTaskCreatePinnedToCore(Task_Stepper, "Task_Stepper", 10000, NULL, 4, &RTOS_Tasks, core_one);
   xTaskCreatePinnedToCore(Task_HDC1080, "Task_HDC1080", 10000, NULL, 2, &RTOS_Tasks, core_one);
   // xTaskCreatePinnedToCore(Task_sunSensor, "Task_sunSensor", 10000, NULL, 2, &RTOS_Tasks, core_one);
}

void loop()
{
  WiFiClient client = server.available();   // Listen for incoming clients
  
  int state;

  if (client) 
  {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) 
    {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) 
      {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') 
        {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) 
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /13/on") >= 0) 
            {
              // Serial.println("GPIO 13 on");
              output13State = "on";
              // Serial.println("Sending 1 to Queue.. value: ");
              state = 1;
              xQueueSend(xStateQueue, &state, 0U);
              delay(50);
              // Serial.print("State Value from server ");
              // Serial.println(state);
              // Turn D13 on to indicate CW motion on the stepper
              digitalWrite(output13, HIGH);
            } 
            else if (header.indexOf("GET /13/off") >= 0) 
            {
              //Serial.println("GPIO 13 off");
              output13State = "off";
              // Serial.println("Sending 1 to Queue.. value: ");
              state = 0;
              xQueueSend(xStateQueue, &state, 0U);
              delay(50);
              // Turn D13 off to indicate CCW motion on stepper
              digitalWrite(output13, LOW);
              // Serial.print("State Value from server ");
              // Serial.println(state);
            } 
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p> D13 LED - State " + output13State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output13State=="off") 
            {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">CW</button></a></p>");
            } 
            else 
            {
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">CCW</button></a></p>");
            } 
                          
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } 
          else 
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}