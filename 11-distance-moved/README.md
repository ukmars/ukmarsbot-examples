# Measuring Robot Speed

If you have worked your way through the other examples, you should now have some knowledge of all the parts that make up the UKMARSBOT robot. Now it is time to start putting some of those parts together and begin to build a working robot. In the 07-encoders example you saw how to keep track of the encoder counts for each wheel. These counters can be set to zero and increase or decrease as the robot's wheels turn. If you know how many counts correspond to a known distance then it is easy to calculate the distance the robot has moved. Because the two wheels are recorded separately, it is easy to keep track of both the amount the robot has moved forward and the amount by which it has turned. Better yet, these two things can be tracked completely independently


## Forward Motion

Classically, robot textbooks and guides will tell you that the forward motion of the robot is best calculated by taking the average of the left and right counts. That is, you add up the left and right encoder counts and then divide by two. In addition, you need to know how many mm of travel correspond to each encoder count. Since the sum of the encoder counts is always divided by 2 in the calculation, it is more convenient to adjust the calibration constant so that it is the distance travelled by each count in the **sum** rather than for each wheel. This value is stored as a constant ```MM_PER_COUNT```:

    encoderSum = encoderRightCount + encoderLeftCount;
    distance = MM_PER_COUNT * encoderSum;

The answer will be in millimeters. Always use real units wherever possible. Only then can you compare results with other people.

#### Explanation

The distance around the outside of each wheel (the circumference) is just pi * diameter. (pi, you will recall is about 3.14159 or approximately 22/7). Suppose your robot has 32mm diameter wheels. The circumference is pi```*```32 = 100.531mm. I have already calculated that the motor gearbox has a ratio of 19.5:1 and that the encoders give 12 counts per rotation of the motor shaft. That means that one rotation of a wheel (100.531mm) will correspond to 12```*```19.5 = 234 encoder counts, So each encoder count corresponds to (100.531/234) = 0.4296mm of movement _of that wheel_.

As described above though, forward motion uses the sum of the two wheel encoder counts and so tghe calibration constant ```MM_PER_COUNT``` will be half the size at 0.2148 mm/count

If you were to make your robot move in a straight line so that the encoder sum is 4655 counts, how far has the robot moved? The answer is just 4655```*```0.2148 = 999.894mm - pretty much exactly 1 meter.

This constant value , 0.2148, is a characteristic of one particular robot and depends on the encoder counts per motor rotation, the gear ratio and the wheel diameter. You robot may well have a different value and you will need to calculate it. You can do that based on the example above or you can use the 07-encoders example. Start the example code, check the counts are zero and push the robot forwards in a straight line for exactly 1000mm. Now the value of the constant is just (1000/count). The two wheels may not match exactly because it is a bit hard to keep the robot straight. Just take the average for the left and right wheels.


## Rotation

Forward movement seems pretty easy then but how can you measure the amount by which the robot has turned?

You can have the robot calculate the angle of turn by keeping track of the difference between the encoder counts and performing a simple calculation. The calculation requires the value of MM_PER_COUNT calculated above and the distance between the robot wheels. Since the distance between the wheels is known when the code is loaded up, you can precalculate a constant, ```DEG_PER_COUNT```:

    encoderDifference = encoderRightCount - encoderLeftCount;
    angle = DEG_PER_COUNT * encoderDifference;


The answer will be in degrees. Always use real units wherever possible. Only then can you compare results with other people.

It is important to be consistent in deciding whether positive angles are clockwise or anticlockwise. It is conventional in mathematics to have positive angles be anticlockwise. Of course, if you have never used an analogue clock, this will all be news to you anyway. Ask an older person to explain :)

#### Explanation

Imagine having the robot spin one complete rotation anti-clockwise on the spot - exactly about a point halfway between the wheels. Now the right wheel has moved forward by some distance but the left wheel has moved backwards by exactly the same amount. Since backward motion produces a negative count, when you add the left and right counts together, you get zero. Of course, that is exactly right - the robot has not moved forwards at all.

Now think about what you get if you start with the right encoder count and **subtract** the left encoder count. For a typical UKMARSBOT with standard motor mounts as in the example above, you might find that the right motor has moved +540 counts and the left motor has moved -540 counts. Now you can calculate the encoder difference:

    encoderDifference = encoderRightCount - encoderLeftCount

In this example, the result would be (540 - (-540)) = 1080 counts. If this was forward motion, 1080 counts would correspond to 1080```*```0.2148 = 232mm. So what?

Look at your robot and measure the distance between the two wheels at the point where they appear to touch the ground. That distance should be about 74mm. Now imagine a circle with its centre halfway between the wheels and a diameter, D, equal to the distance between the wheels. The circumference of that circle would have a length of pi```*```D = 3.14159```*```74 = 232.5mm.

When the robot performed its turn on the spot, each wheel had to move a distance equal to the circumference of the circle. That is almost exactly what the encoders gave when you convert the encoder counts into distance. The small difference is due to the fact that you cannot measure exactly, with a ruler, the distance between the two wheel contact points on the ground. Once you know how many counts correspond to a complete circle (360 degrees), it is easy to work out, from the encoder counts, any angle though which the robot might turn. In this example, the answer would be (360/1080) = 0.333 deg/count.


## Independance

It may not be entirely clear until you play with this a bit but the calculation of forward and rotational motion are independent. That is, the robot could follow a curved path and, at the end of it, you can work out how far forward it has moved as well as the angle that it has turned.


## Example Code

The code in this example is based on that in the 07-encoders example. The difference is that the robot position and angle are displayed regularly on the serial monitor. At the top of the program you will see the calibration constants ```MM_PER_COUNT``` and ```DEG_PER_COUNT```. In this example, the values are calculated from the robot characteristics.  You can change entries to suit your robot. If you prefer, you can work out the values of  ```MM_PER_COUNT``` and ```DEG_PER_COUNT``` directly and type them into the program without having to know the gear ratio, wheel diameter and wheel separation.


Run the program. After a restart, the program will display the current calibration values on the serial monitor. Press the user button and you will see a constant stream of numbers showing the encoder sum and difference along with their equivalents in millimetres and degrees. Try pushing the robot around on the desk or floor. The results should be very close to what you might expect. It is a lot harder than you think to push a robot an exact distance or to turn it by an exact angle. Try holding one wheel still while you rotate the robot a full 360 degrees around that stationary point. Think about the values that you see. You can reset the counters to zero at any time by pressing the pushbutton at the back of the robot.

### Calibration

To change the calibration, start by pushing the robot a fixed distance straight ahead. The further you move it, the more accurate the result will be. Repeat several times to get an average. Compare the displayed distance with the amount you moved the robot. If they are different, make changes to the constant ```WHEEL_DIAMETER``` an re-run the program until they are close. It is unlikely you will get it exactly right so, when the readings are good enough, accept that as good enough and move on.  Good enough here is probably to be within 1mm in 200mm of travel (+/-1%)

Clearly, a very large error might be because you do not have the gear ratio correct for your motors.

Once the forward calibration is done, you can adjust the angle calibration. Zero the counters and rotate the robot by 360 degrees, as accurately as you can - it does not have to be exactly around the centre of the robot. Again, do several tests and get an average. Compare the displayed angle with what you expect and, if they are not close enough, adjust the constant ```WHEEL_SEPARATION``` in the program and re-run until you get a good enough result. Good enough here is probably within 2 degrees of the true answer over a full turn (+/-0.5%).

It is not worth the effort of getting better than 0.5% accuracy. Indeed, it may not be possible on your robot if you have low gear ratios. Generally, +/-1% will be good enough for most purposes.

If you have managed to connect a Bluetooth Serial link to the robot it will save having the cable get in the way all the time.

### DigitalFastWrite

You will see that there is an extra file in this sketch (and the others) - ```digitalFastWrite.h```.

The file is needed to make sure that the encoder interrupts are serviced as fast as possible. The built in ```digitalRead()``` function is not very efficient and the interrupts may arrive at several tens of thousands per second. The code as shown in this example can process each interrupt in under 2.5 microseconds only because it uses ```digitalFastRead()``` instead of the built-in call.

To use this feature you only have to make sure that the ```digitalFastWrite.h``` file is present in the same folder as the sketch file and then add the following line at the top of the main sketch ```.ino``` file:

    #include "digitalWriteFast.h"

The Arduino IDE will take care of the rest.