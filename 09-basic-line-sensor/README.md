# Basic Line Sensor

UKBARSBOT has several sensor board options. This example uses the Basic Line Sensor. The board has a pair of detectors that sit over either edge of the line and are illuminated by a single, central LED emitter. When the sensor is correctly located over the line, both detectors will read very close to the same value. You should adjust the emitter LED and the detectors until this is true.

At either end of the sensor board are a pair of emitters and detectors. These are angled outwards and are designed to be able to see the markers on the line follower and drag racer course. The right-most sensor detects the start and finish markers and the left-most sensor detects the turn markers. These sensors need to be adjusted so that they give a good signal when the marker is underneath them. It is not so important how big that signal is but is must be very distinct from what is returned when there is no marker.

## Cross Track Error

When following a line, the robot must be able to measure its position relative to the line. Only the middle, line-tracking detectors are used for this, the outside marker sensors are not used for line tracking. The line sensor board gives you the information needed for that and your line follower software must use that information to produce an error signal, called the Cross Track Error (CTE). When the sensor is positioned exactly central over the line, the CTE will be zero. If the sensor moves to the right, the error will become negative and if the sensor moves to the left, the error will become positive. Your life will become a lot easier if you can arrange for the CTE to be a number that is close to the distance, in mm, that the sensor is offset from the line centre.

That is all well and good for small errors but it should be clear that, if the sensor is a long way off the line, neither detector will get a reflection and they will read about the same value. That will make the CTE look like it is zero. Your line follower program must take that into account. One approach might be to remember the last reading when the line is lost.

There is an easy way to tell if the sensor can see the line and that is to add up the values from each of the detectors to get the sensor sum. With no line, both sensors will give a small reading so the robot can use that information to tell if the line cannot be seen. When the sum is small, there is no line, when it is above some magic value (```LINE_DETECT_THRESHOLD```), your robot can see the line. You will need to work out what value works best for your robot. You need to calculate this for the sum:

    gSensorSum = gSensorRight + gSensorLeft;

So, you can tell if the line is there, how about the ross track error, CTE? Well, the easiest aproach is to just take the difference between the two detectors. Subtract the left detector value from the right detector value. Imagine the sensor board moving off to the right a little. The left detector reading will be bigger than the right detector reading and so the difference (right-left) will be negative. Great! Just what we want.

    gSensorDifference = gSensorRight - gSensorLeft;

There is a problem with this simple appoach though. Although the CTE will be negative for errors to the right and positive for errors to the left, the **size** of that error will depend on how bright the line is and how powerful the emitter is. That is not good from a control point of view. What you need is a value for CTE that is independant of the brighness and which is *only* related to the actual error.

Luckily, you have already calculated the total brightness when you decided if you can see even the line. All that is needed for you to compensate for the sensor brightness is to divide the difference by the sum and you have a value for CTE that is exactly what we want. A little thought might show that the value of CTE that you get in this way will vary from about  -1 to +1 but earlier, I said it would be better if the CTE were expressed in millimeters. Easy to fix, just multiply the CTE by a suitable constant for your robot so that you get a number that is correct, or as close as you can get. Now your CTE calculation might look like this:

    gSensorCTE - LINE_WIDTH * (gSensordifference/gSensorSum);

You may wonder why it is important to try and get CTE in millimeters. The main reason is that you cannot hope to try and compare the performance and setup of your robot with that of other builders unless you are using the same units. It would be like trying to buy three cakes only to find that the baker thought you meant three boxes of cakes - a nice surprise until you have to pay for them.

## Example Code

The example sketch in this folder reads the Basic Line Sensor 500 times per second using the systick approach explained in step 05. Fast conversions are also used, as described in step 08. The results are stored in some global variables and sent five times per second to the Serial Monitor on the PC. In the full robot contest code, it is important that the sensor readings are made very frequently and at regular intervals. Here, they are sent to the serial monitor less often just to help overloading the serial port. All four sensor readings are sent as well as the difference, sum and CTE.

There are two things that you should note carefully.

* First, the sensors are read twice - once with the emitter off and once with the emitter on. This almost eliminates the effect of ambient illumination and give much more consistent and reliable results.

* Second, the sensor on this board produce values that are **smaller** when they see the line. The sample code ignores that so that the results are consistent with the discussion above.

Use this sketch to adjust the position of the individual sensors and the values of the resistors in the emitter and detector circuits. You aim should be to get line detector sensor reading of no bigger than about 500 and marker sensor readings of at least 500. The robot I used for these examples has over-sensitive detectors.

You will probably notice that, as you move the sensor to the left and right of the line, the CTE will increase in magnitude to some maximum value when the actual error is about +/-10mm and then start to get smaller again until the line is lost. This is normal. A more advanced sensor arrangement will give a greater range of error values. Experiment with the constant ```LINE_WIDTH``` until you get a maximum value for CTE of about +/- 10. Do not worry too much if the readings are not symmetrical but try adjusting the position of the LED and detectors to get them as similar as possible.

You should find that the sensor is very sensitive to small errors and it may even be difficult to get it to read zero when it looks correctly positioned. Also, there will be some small variation in the error even when the robot is completely stationary. These things are normal.

The sketch will light the LEDs on the sensor board when it sees a marker. Adjust the marker threshold values (```LEFT_MARKER_THRESHOLD``` and ```RIGHT_MARKER_THRESHOLD```) so that you can reliably detect a marker. A good starting point for the threshold values might be twice the reading shown when the sensor is over a black section of floor.

## Eye Safety

If you are using high-power Infra Red emitters like the SFH4550, do not look into the emitters while they are operating. The light is invisibale and can, under some circumstances, present a risk of damage to the eye. Use low power emitters or visible light emitters for better results and greater safety.
