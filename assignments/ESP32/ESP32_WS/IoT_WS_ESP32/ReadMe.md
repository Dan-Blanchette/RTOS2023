# ReadMe

## Brief
This program is for my final project in RTOS for Spring 2023.
## Goals
- Register the ESP32 with the RTOS class cloud server.
- Receive data from the I2C devices.
- Post Data from the HDC1080 ( temperature and humidity sensor [I2C Device]) to the cloud.
- Post Data from the (Sunlight Sensor [I2C Device]) to the cloud.
- Control the direction of a stepper motor via buttons on the cloud server.
- Use freeRTOS to schedule I2C bus priorities and manage the stepper motor task.
