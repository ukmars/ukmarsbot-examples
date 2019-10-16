# Better PWM

The basic PWM setting for motors works just fine but there is a way to improve it significantly.

Electric motors like those used in UKMARSBOT run at a speed that is determined by the Voltage applied to the motor terminals. In fact, an ideal motor will have a linear relationship between the applied voltage and the motor speed. Thus, applying 2 Volts will make the motor run twice as fast as applying 1 Volt. In practice, real motors and drive systems have differences due to things like the load on the motor, frition losses and the available drive current. Even so, it is much better to drive the motor by applying a specific, known voltage. In that way, the behaviour is more predictable.

In a robot like UKMARSBOT, the motor is driven using a PWM signal that switches the battery voltage. The duty cycle of the PWM sets the proportion of the battery voltage that the motor gets. So, if the battery voltage is 8 Volts and the PWM is set to 50%, the motor behaves as it it is connected to a 4 Volt supply.

But what happens if the battery voltage changes? Different battery types have different voltages and, as the battery discharges, the voltage naturally drops. That means that just using a 50% PWM duty cycle will apply a different voltage to the motors when you change batteries - either for a different type or for a freshly charged battery.

You can see the effect of this quite often at robot events. Builders struggle to make their robot behave consistently and performance declines as the battery discharges. They may set everything up carefully and then, when they plug in a fresh battery, everything goes wrong.

If you try to develop good motor control without compensating for the battery voltage, you are only making the task more difficult.

## Voltage Control

On UKMARSBOT, it is possible to measure the current battery voltage. That makes it possible to adjust the PWM duty cycle to compensate for changes in battery voltage. Suppose you want to set the motor voltage at 3 Volts. With a new, alkaline battery, the supply voltage may be 9 Volts so we need to set the PWM duty cycle to (3/9) * 255 = 85.

If the battery is then changed for a LiIon type with a voltage of only 7.5 Volts, the PWM duty must be increased to (3/7.5)*255 = 102  to compensate for the reduced supply.

Fortunately, the robot software can easily make these calculations whenever they are needed and you can set the motor voltage rather than setting the PWM and be confident that the motor is running at the speed you asked for rather than being dependent on the state of the battery.

Clearly, you cannot ask for more than the battery can supply and there will come a point where the battery discharges and cannot supply your demands. It is important to monitor the battery voltage while the robot is running.

To avoid the robot running out of steam, you should set a maximum voltage that can be applied to the motor. For the UKMARSBOT motors this would be 6 Volts if you want them to behave the same as the data sheet suggests. You can, however, set a higher maximum voltage for higher performance. Just take care that you set the maximum motor voltage to a value that is smaller than the minimum expected battery voltage.

## Example Code

This sketch behaves exactly like the basic-pwm example - the robot performs various actions depending on the setting of the function switch.

The difference is that the motor speeds are now specified in terms of Voltages instead of PWM duty. You will probably not see any difference when doing simple tests but this sketch will produce the same results with batteries of different voltages. In your contest code, you should definitely drive your motors by specifying voltages rather than duty cycles if you want consistent results.


|Switches|Function| Action                              |
|:------:|:------:|-------------------------------------|
|  0000  |    0   | Motors Off                          |
|  0001  |    1   | Forward 25%                         |
|  0010  |    2   | Forward 50%                         |
|  0011  |    3   | Forward 75%                         |
|  0100  |    4   | Reverse 25%                         |
|  0101  |    5   | Reverse 50%                         |
|  0110  |    6   | Reverse 75%                         |
|  0111  |    7   | Spin Left 25%                       |
|  1000  |    8   | Spin Left 50%                       |
|  1001  |    9   | Spin Right 25%                      |
|  1010  |   10   | Spin Right 50%                      |
|  1011  |   11   | Pivot Left 25%                      |
|  1100  |   12   | Pivot right 25%                     |
|  1101  |   13   | Curve - Left 25%, Right 50%         |
|  1110  |   14   | Curve - Left 50%, Right 25%         |
|  1111  |   15   | Curve - Left 75%, Right 50%         |


For each motor, a single function takes the requested motor voltage and calculates the required PWM from the current battery voltage. Once the duty has been calculated, that value is used when calling the basic PWM setting.

The current battery voltage must be calculated continuously if the robot is to compensate for battery voltage changes while running. In this example the current battery voltage is kept in a global variable, ```gBatteryVoltage```, that must be updated regularly.

For the left motor, the setting function is this.

    void setLeftMotorVolts(float volts) {
      volts = constrain(volts, -MAX_MOTOR_VOLTS, MAX_MOTOR_VOLTS);
      int motorPWM = (int)((255.0f * volts) / gBatteryVolts);
      setLeftMotorPWM(motorPWM);
    }

Notice that the motor voltage is a floating point value. A similar function is used for the right motor. There is also a convenience function that sets both motors together:

    void setMotorVolts(float left, float right){
      setLeftMotorVolts(left);
      setRightMotorVolts(right);
    }







