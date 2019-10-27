# Reading the Function Select Switches

UKMARSBOT has a pushbutton and a four pole dip switch so that the user can select up to 16 different settings or functions as well as detecting the pressing of the pushbutton.

These switches are connected together with a set of resistors so that the various combinations of the switches generate a unique voltage on one of the Arduino analogue input pins.

A simple ```analogRead(FUNCTION_PIN)``` will measure that voltage and return a value in the range 0 to 1023.

Every combination of the dip switches causes a different value to be read. With all the switches off, the reading will be near zero;

The push button overrides the switches and always generates a large value close to 1023

## Decoding the switches

Because components all have a small tolerance and so are never exactly the value printed on them, you cannot rely on a particular combination of switch settings to always give the same reading. Even if the components were exact, the Arduino analogue to digital converter has a small tolerance and will give slightly different readings over time.

To overcome this problem, the resistor values have been chosen to give the largest possible difference between any two switch settings. The code in this example shows how you can read the analogue voltage from the function channel and then compare that reading with a list of preset values. In this way, you can convert the raw analogue reading into a simple stable number from 0 to 15 corresponding to the switch positions. If the pushbutton is pressed, the function always gives the value 16.

NOTE that the switches should have been installed so that they ar ```ON``` when in the 'up' position as viewed from the rear of the robot. The switch positions then correspond to a binary number with 'up' representing a '1' and 'down' representing a '0'. For example if the switches are DOWN, UP, DOWN, UP, that would represent the binary number 0101 or 5 in decimal. All switches UP is 15 and all DOWN is zero.

Run the sketch and try different switch settings - the analogue reading and the function number will be displayed on the serial monitor. You should be able to see that the raw analogue reading is not constant but varies a little over time. Pressing the pushbutton should also light up the built in LED on the Arduino.

## Customisation

Your particular robot may not have the same analog readings as mine even with nominally the same resistor values. You will then need to adjust the threshold values in the program to take that into account. These variations are most likely to show up with the function settings of 0, 1, 2 and 3.

It is easy enough to customise the code for your robot.

Run the program and set the switches to each value in turn, making a note of the analogue readingfor each setting. Then record the values corresponding to 0001, 0010, 0011 etc into the declaration of ```const int adcReading[]```. When you rebuild and run the code again, you should have reliable detection of each of the switch settings

