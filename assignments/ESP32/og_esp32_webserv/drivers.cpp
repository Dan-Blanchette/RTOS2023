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