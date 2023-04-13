#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Task handles
TaskHandle_t task1Handle;
TaskHandle_t task2Handle;

// WiFi credentials
const char* ssid = "sending-stone";
const char* password = "4579W!$hThis#";

AsyncWebServer server(80);

// Task 1 function
void task1(void* parameter) {
  while(1) {
    // printf("Task 1 is running...\n");
    // Set up routes for web server
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello, world");
  });

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

// Task 2 function
void task2(void* parameter) {
  while(1) {
    printf("Task 2 is running...\n");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the server
  server.begin();

  // Create tasks
  xTaskCreate(task1, "Task 1", 10000, NULL, 2, &task1Handle);
  xTaskCreate(task2, "Task 2", 10000, NULL, 1, &task2Handle);
}

void loop() {
  // Nothing to do here
}