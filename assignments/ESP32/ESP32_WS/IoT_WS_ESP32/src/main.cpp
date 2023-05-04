/**
 * @file main.cpp
 * @author Dan Blanchette
 * @brief  This program will run a web sever on the ESP32 Core 0 (Uses Loop Function).
 * RTOS tasks for 2 I2C devices(temp/hum sensor, sunlight sensor, and the stepper motor on ESP32's processor
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

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

void displaySensorDetails(void);

// Task handles
TaskHandle_t webServerTask;
TaskHandle_t iotServerTask;
TaskHandle_t RTOS_Tasks;

// HDC1080 Class Object
ClosedCube_HDC1080 hdc1080;

/******************WEB CLIENT VARIABLES*******************/
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
// Queue Handle for Sunlight Sensor
static QueueHandle_t xVisibleQueue = NULL;
static QueueHandle_t xInfraredQueue = NULL;
static QueueHandle_t xFullSpectQueue = NULL;
// Queue Handle for Temp/Hum Sensor
// celsius values
static QueueHandle_t xCtempQueue = NULL;
// fahrenheit values
static QueueHandle_t xFtempQueue = NULL;
static QueueHandle_t xRhQueue = NULL;

// Web Server Setup
WiFiServer server(80);
String header;

// Auxiliar variables to store current output state
String output13State = "off";

// detect server IP address
String serverDet = "http://52.23.160.25:5000/IOTAPI/DetectServer";
String serverReg = "http://52.23.160.25:5000/IOTAPI/RegisterWithServer";
String serverData = "http://52.23.160.25:5000//IOTAPI/IOTData";
String serverQueryForCommands = "http://52.23.160.25:5000//IOTAPI/QueryServerForCommands";

// Home WiFi credentials
// censor this before submitting or pushing to git
const char *ssid = "some-network";
const char *password = "some-password";
// test

/***********************TSL2591**********************/
void configureSensor(void)
{
  tsl.setGain(TSL2591_GAIN_MED);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);

  Serial.println(F("------------------------------------"));
  Serial.print(F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch (gain)
  {
  case TSL2591_GAIN_LOW:
    Serial.println(F("1x (Low)"));
    break;
  case TSL2591_GAIN_MED:
    Serial.println(F("25x (Medium)"));
    break;
  case TSL2591_GAIN_HIGH:
    Serial.println(F("428x (High)"));
    break;
  case TSL2591_GAIN_MAX:
    Serial.println(F("9876x (Max)"));
    break;
  }
  Serial.print(F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC);
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}



/**
 * @brief   // Simple data read example. Just read the infrared, fullspecrtrum diode
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
 *
 */
void simpleRead(void)
{
  uint16_t vis = tsl.getLuminosity(TSL2591_VISIBLE);
  uint16_t fs = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
  uint16_t ir = tsl.getLuminosity(TSL2591_INFRARED);
  xQueueSend(xVisibleQueue, &vis, 0U);
  vTaskDelay(20 / portTICK_PERIOD_MS);
  xQueueSend(xInfraredQueue, &ir, 0U);
  vTaskDelay(20 / portTICK_PERIOD_MS);
  xQueueSend(xFullSpectQueue, &fs, 0U);
  vTaskDelay(20 / portTICK_PERIOD_MS);
}

/**
 * @brief   // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
 *
 */
void advancedRead(void)
{

  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print(F("[ "));
  Serial.print(millis());
  Serial.print(F(" ms ] "));
  Serial.print(F("IR: "));
  Serial.print(ir);
  Serial.print(F("  "));
  Serial.print(F("Full: "));
  Serial.print(full);
  Serial.print(F("  "));
  Serial.print(F("Visible: "));
  Serial.print(full - ir);
  Serial.print(F("  "));
  Serial.print(F("Lux: "));
  Serial.println(tsl.calculateLux(full, ir), 6);
}

/********************RTOS TASKS****************************/

void Task_IoT_Server(void *parameter)
{
  while (1)
  {
    float cel_val, fahr_val, rh_val;
    // get the celsius value from the sensor
    xQueueReceive(xFtempQueue, &fahr_val, 0U);
    // get the humidity from the sensor
    xQueueReceive(xRhQueue, &rh_val, 0U);

    http.addHeader("Content-Type", "application/json");

    // send HTTP POST and Store Response(POST return value)
    // int httpResponseVal = http.POST("{\"key\":\"2436e8c114aa64ee\"}");
    // String response = http.getString();

    // Serial.print("HTTP Response code: ");
    // Serial.println(httpResponseVal);
    // Serial.println(response);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
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
    // NOTE: step_dir(int dir) has vTaskDelay(10 / portTICK_PERIOD_MS)
    // Flag Directions (True = CW, False = CCW)
    xQueueReceive(xStateQueue, &rec_val, 0U);
    step_dir(rec_val);
  }
}

/**
 * @brief Task that reads the temperature and relative humidity from the HDC1080 Sensor
 *
 * @param parameter
 */
void Task_HDC1080(void *parameter)
{
  while (1)
  {
    // convert celsius to Fahr
    float celsius = hdc1080.readTemperature();
    float fahr = ((celsius * 1.8) + 32);
    // get relative humidity
    float rh = hdc1080.readHumidity();

    xQueueSend(xCtempQueue, &celsius, 0U);
    xQueueSend(xFtempQueue, &fahr, 0U);
    xQueueSend(xRhQueue, &rh, 0U);

    // Serial.print("T=");
    // Serial.print(hdc1080.readTemperature());
    // Serial.println("C");

    // Serial.print("T=");
    // Serial.print(fahr);
    // Serial.print("F RH=");
    // Serial.print(rh);
    // Serial.println("%");
    // read once every 3 seconds per port tick
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

/**
 * @brief Task Reads Sun Sensor Data TR2591
 *
 * @param Parameters
 */
void Task_sunSensor(void *Parameters)
{
  while (1)
  {
    simpleRead();
  }
}

/**
 * @brief Arduino Device Setup Function
 *
 */
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

  /************SETUP TSL2591*******************/
  // displaySensorDetails();
  configureSensor();

  /**********QUEUE INSTANTIATION********/
  // stepper
  xStateQueue = xQueueCreate(1, sizeof(int));

  // photosensor
  xVisibleQueue = xQueueCreate(1, sizeof(uint16_t));
  xInfraredQueue = xQueueCreate(1, sizeof(uint16_t));
  xFullSpectQueue = xQueueCreate(1, sizeof(uint16_t));

  // temp/hum sensor
  xCtempQueue = xQueueCreate(1, sizeof(float));
  xFtempQueue = xQueueCreate(1, sizeof(float));
  xRhQueue = xQueueCreate(1, sizeof(float));

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

  // Start up an ESP32 Web Server
  server.begin();

  /*************IOT Setup*****************/

  // Begin new connection to cloud website
  // http.begin("http://52.23.160.25:5000/");
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

  /*********************** Create RTOS Tasks *******************************************************************/

  // Web Server and IoT RTOS Server Tasks
  xTaskCreatePinnedToCore(Task_IoT_Server, "Task_IoT_Server", 10000, NULL, 4, &iotServerTask, core_zero);

  // RTOS Tasks for Connected Devices
  xTaskCreatePinnedToCore(Task_Stepper, "Task_Stepper", 10000, NULL, 4, &RTOS_Tasks, core_one);
  xTaskCreatePinnedToCore(Task_HDC1080, "Task_HDC1080", 10000, NULL, 3, &RTOS_Tasks, core_one);
  xTaskCreatePinnedToCore(Task_sunSensor, "Task_sunSensor", 10000, NULL, 2, &RTOS_Tasks, core_one);
}

// Handles ESP32 local web client server requests and user/client interactions with the stepper motor
/**
 * @brief Arduino Loop Function
 *
 */
void loop()
{
  WiFiClient client = server.available(); // Listen for incoming clients

  int step_direction;
  uint16_t vis_val;
  uint16_t ir_val;
  uint16_t fullSpec_val;

  if (client)
  { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
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
              step_direction = 1;
              xQueueSend(xStateQueue, &step_direction, 0U);
              delay(50);
              // Turn D13 on to indicate CW motion on the stepper
              digitalWrite(output13, HIGH);
            }
            else if (header.indexOf("GET /13/off") >= 0)
            {
              // Serial.println("GPIO 13 off");
              output13State = "off";
              // Serial.println("Sending 1 to Queue.. value: ");
              step_direction = 0;
              xQueueSend(xStateQueue, &step_direction, 0U);
              delay(50);
              // Turn D13 off to indicate CCW motion on stepper
              digitalWrite(output13, LOW);
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
            client.println("<body><h2> Stepper Motor Direction </h2>");
            client.println("<p>D13_LED = off: Stepper = CCW.");
            client.println("<p>When D13_LED = on: Stepper = CW. </p>");
            client.println("<p>Default State: D13_LED off, Stepper = CCW");
            client.println("<body><h3> D13 LED - State: " + output13State + "</h3>");

            if (output13State == "off")
            {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">CW</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">CCW</button></a></p>");
            }

            client.println("<body><h3>TR2591 Photo Sensor Data</h3>");
            // Get the Visible Light Reading From the Sensor and update to webpage
            xQueueReceive(xVisibleQueue, &vis_val, 0U);
            client.print("<p>Visible: ");
            client.print(vis_val, DEC);
            client.println(" Lumen(s)</p>");
            // Get the Infrared Reading From the Sensor and update to webpage
            xQueueReceive(xInfraredQueue, &ir_val, 0U);
            client.print("<p>Infrared: ");
            client.print(ir_val, DEC);
            client.println(" micron(s)");
            // Get the Full Spectrum Reading and update to webpage
            xQueueReceive(xFullSpectQueue, &fullSpec_val, 0U);
            client.print("<p>Full Spectrum: ");
            client.print(fullSpec_val, DEC);
            client.println(" Angstrom(s)");
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
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
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