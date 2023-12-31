#include "IO-layer.h"

Servo myMotorTurret;

uint initMotors(int timer)
{
    uint returnValue = 0;
    
    ESP32PWM::allocateTimer(timer);
    myMotorTurret.setPeriodHertz(50);
    myMotorTurret.attach(motorTurretPWM, 500, 2500);
    
    digitalWrite(motorRechtsPWM, LOW);  // set speed to 0
    digitalWrite(motorRechtsDIR, HIGH); // set direction to forward
    digitalWrite(motorLinksPWM, LOW);   // set speed to 0
    digitalWrite(motorLinksDIR, HIGH);  // set direction to forward

    pinMode(motorRechtsPWM, OUTPUT);
    pinMode(motorRechtsDIR, OUTPUT);
    pinMode(motorLinksPWM, OUTPUT);
    pinMode(motorLinksDIR, OUTPUT);

    return returnValue;
}

uint updateMotor(motor currentMotor, int motorPower)
{
    uint returnValue = 0;
    bool direction = (0 <= motorPower);

    // check if motorPower exceeds it's bounds of 1000 to -1000, if so change returnValue accordingly
    if(-1024 > motorPower || 1024 < motorPower)
    {
        if(0 == direction)
        {
            motorPower = -1024;
            //returnValue = updateMotor_exceeded_lower_bound_of_motorPower;
        }
        else
        {
            motorPower = 1024;
            //returnValue = updateMotor_exceeded_upper_bound_of_motorPower;
        }
    }

    // any motor added to this switch case should also be added to the enum "motor in IO-layer.h"
    switch(currentMotor)
    {
        case motorRechts:
            analogWrite(motorRechtsPWM, abs(motorPower) / 4);
            digitalWrite(motorRechtsDIR, direction); // might need to add ! to invert the direction
            break;
        case motorLinks:
            analogWrite(motorLinksPWM, abs(motorPower) / 4);
            digitalWrite(motorLinksDIR, direction); // might need to add ! to invert the direction
            break;
        case motorTurret:
            myMotorTurret.write(1500 + (motorPower));
            break;
        default:
            //returnValue = updateMotor_invalid_value_for_currentMotor;
            break;
    }
    return returnValue;
}

struct PS4 IO_Layer_Besturing()
{
    struct PS4 PS4Inputs;
  if (PS4.isConnected()) 
  {
    PS4Inputs.Cirkelknop = PS4.Circle();
    PS4Inputs.R2 = PS4.R2Value();
    PS4Inputs.L2 = PS4.L2Value();
    PS4Inputs.Linkerjoystick_x = PS4.LStickX();
    PS4Inputs.Linkerjoystick_y = PS4.LStickY();
    PS4Inputs.Rechterjoystick_x = PS4.RStickX();
    PS4Inputs.Rechterjoystick_y = PS4.RStickY();
    return PS4Inputs;
   }       

    return PS4Inputs; 
}
