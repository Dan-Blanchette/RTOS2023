#include "devices.h"

/**
 * @brief Set the up stepper motor pins
 * 
 */
void setup_stepper()
{
   pinMode(STEP_IN1, OUTPUT);
   pinMode(STEP_IN2, OUTPUT);
   pinMode(STEP_IN3, OUTPUT);
   pinMode(STEP_IN4, OUTPUT);
}


// void setup_buttons()
// {
//    pinMode(BUTTON_LEFT, INPUT);
//    pinMode(BUTTON_RIGHT, INPUT);
//    pinMode(BUTTON_MID, INPUT);

// }

/**
 * @brief Moves the stepper motor
 * 
 * @param step picks a state. If a state does not exist defaults to reset/initial state
 */
void stepper_move(int step)
{
   switch(step)
   {
      case 1:
         digitalWrite(STEP_IN4, 1);
         digitalWrite(STEP_IN3, 0);
         digitalWrite(STEP_IN2, 0);
         digitalWrite(STEP_IN1, 0);
         break;

      case 2:
         digitalWrite(STEP_IN4, 1);
         digitalWrite(STEP_IN3, 1);
         digitalWrite(STEP_IN2, 0);
         digitalWrite(STEP_IN1, 0);
         break;

      case 3:
         digitalWrite(STEP_IN4, 0);
         digitalWrite(STEP_IN3, 1);
         digitalWrite(STEP_IN2, 0);
         digitalWrite(STEP_IN1, 0);
         break;

      case 4:
         digitalWrite(STEP_IN4, 0);
         digitalWrite(STEP_IN3, 1);
         digitalWrite(STEP_IN2, 1);
         digitalWrite(STEP_IN1, 0);
         break;

      case 5:
         digitalWrite(STEP_IN4, 0);
         digitalWrite(STEP_IN3, 0);
         digitalWrite(STEP_IN2, 1);
         digitalWrite(STEP_IN1, 0);
         break;

      case 6:
         digitalWrite(STEP_IN4, 0);
         digitalWrite(STEP_IN3, 0);
         digitalWrite(STEP_IN2, 1);
         digitalWrite(STEP_IN1, 1);
         break;

      case 7:
         digitalWrite(STEP_IN4, 0);
         digitalWrite(STEP_IN3, 0);
         digitalWrite(STEP_IN2, 0);
         digitalWrite(STEP_IN1, 1);
         break;

      case 8:
         digitalWrite(STEP_IN4, 1);
         digitalWrite(STEP_IN3, 0);
         digitalWrite(STEP_IN2, 0);
         digitalWrite(STEP_IN1, 1);
         break;

      default:
         digitalWrite(STEP_IN4, 1);
         digitalWrite(STEP_IN3, 0);
         digitalWrite(STEP_IN2, 0);
         digitalWrite(STEP_IN1, 1);
         break;
   }
}

/**
 * @brief This function will get the true for flase flag from a button toggle on the ESP32 local webserver.
 * If true the stepper motor will rotate in a clockwise direction. Default is Counter Clockwise 
 * @param dir 
 *
 */
void step_dir(int direction)
{
   if (direction == 1)
   {
      for (int i = 9; i > 1; i--)
      {
         stepper_move(i);
         vTaskDelay(10 / portTICK_PERIOD_MS);
      }
   }
   else if (direction == 0)
   {
      for (int i = 1; i < 9; i++)
      {
         stepper_move(i);
         vTaskDelay(10 / portTICK_PERIOD_MS);
      }
   }
}

/**
 * @brief test for web integration and device control
 * 
 */
void d13_setup()
{
   pinMode(output13, OUTPUT);
   pinMode(output27, OUTPUT);

   digitalWrite(output13, LOW);
   digitalWrite(output27, LOW);
}