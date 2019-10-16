# Basic PWM

UKMARSBOT has two drive motors. The circuits that provide the power for these motors expect two signals each - one pair for the left motor and one pair for the right motor.

One of these signals determines the direction of rotation for the motor. Because you may have wired your motors differently to other builders, it is not possible to say for sure whether the direction signal has to be high or low for forward motion. In th example code, you may have to make small changes to the values for the direction signal to account for the direction.

The other signal needed by the drivers is a Pulse Width Modulation (PWM) input. This is a signal which is rapidly changed between high and low. The more time it spends in the high state, the faster the motor will turn. The proportion of time that the signal is high is called the duty cycle. Fortunately, the Arduino software provides a built in function that lets you generate a PWM signal on specific pins. All you have to do is call the function ```analogWrite()``` with two values - the first is the pin number and the second is the duty cycle.

The duty cycle expected by ```analogWrite()``` is a number in the range 0 to 255 inclusive. It is imprtant to restrict the values to be in this range or the results will be unexpected.

Before trying to drive the motors, the appropriate pins must be set up as outputs and you should take care to ensure that the motors will not turn as soon as the robot is started up. The setup code looks like this:

    void motorSetup(){
      pinMode(MOTOR_LEFT_DIR, OUTPUT);
      pinMode(MOTOR_RIGHT_DIR, OUTPUT);
      pinMode(MOTOR_LEFT_PWM, OUTPUT);
      pinMode(MOTOR_RIGHT_PWM, OUTPUT);
      digitalWrite(MOTOR_LEFT_PWM, 0);
      digitalWrite(MOTOR_LEFT_DIR, 0);
      digitalWrite(MOTOR_RIGHT_PWM, 0);
      digitalWrite(MOTOR_RIGHT_DIR, 0);
    }

By setting all the pins to be zero, you can be sure that the motors are off.

### Speed Setting One Motor

In the main program it is annoying to have to keep setting the direction and duty cycle individually whenever the motor speed must be changed. It is better to have a function for each motor that looks after all the details for you. To account for the direction, you can allow the duty cycle to take a value between -255 and +255 inclusive where a positive value indicated forward motion and a negative value indicates reverse motion. The function can then take care of the direction pin for you. Here is the function for the left motor - a similar function needs to be written for the right motor:

    void setLeftMotorPWM(int pwm) {
      pwm = constrain(pwm, -255, 255);
      if (pwm < 0) {
        digitalWrite(MOTOR_LEFT_DIR, 1);
        analogWrite(MOTOR_LEFT_PWM, -pwm);
      } else {
        digitalWrite(MOTOR_LEFT_DIR, 0);
        analogWrite(MOTOR_LEFT_PWM, pwm);
      }
    }

Notice that the function also ensures that the pwm duty cycle is kept within the required limits. If your motors are wired up in reverse, then simply change the two ```digitalWrite()``` function calls to set the direction pins correctly for your build.

For convenience, a single function can be used to set both motor PWM values at once. This will make your code a little quicker to write and easier to read.

    void setMotorPWM(int left, int right){
      setLeftMotorPWM(left);
      setRightMotorPWM(right);
    }




### Example Sketch

The example sketch sets up the two motors and then waits for the user button to be pressed before running the motors for a short amount of time. The function select switches are used to determine exactly what happens next.

**THINK** before pressing the button. Will the robot go flying off the table? Will it run into something that will be damaged? Will it run into something that will damage the robot? Can you catch it in time? Will the dog be upset? Will she give it back after catching it?

Bear in mind that the stock robot, at full speed, might travel 2 meters in one second.

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


Experiment with various settings for the motors until you understand how the motor speed settings affect the behaviour of the robot.

You will discover that there is some lower limit to the speed setting that does not make the motors turn at all. that is normal for this kind of motor and does not casue any big problems in normal use - but you do have to remember about it.

It is also likely that the robot will not travel perfectly straight when the left and right motors get the same PWM value. This is also normal and can be corrected for in the final program.

Finally, note that when you suddenly apply a drive signal to the motors, the wheels may well slip. Even if both wheels end up at the same speed, one may slip more than the other when starting off. That can cause the robot to set off in an unexected direction.

### Extras

This is a moderately complicated example with several useful features. The timing of the robot action does more than a simple fixed delay. Instead, the code sets a finish time for the action and then loops until that time has expired. In this way, the robot can perform other actions while still moving. For example, you might want to send telemetry data over a bluetooth link or test the sensors for obstacles.

This sketch also shows how you can use the function select switches to choose between different actions for the robot and how you can use the pushbutton to start and stop the robot.


