# Basic Wall Sensor

UKBARSBOT has several sensor board options. This example uses the Basic Wall Sensor. The board has three emitters and detectors. On either side is an angled emiter/detector pair and a single, central emitter/detector pair points straight ahead.

The side sensors perform two functions in a wall following robot.

* When the robot is moving, the signals from these sensors can be used to correct steering so that the robot is able to move accurately down the middle of a corridor. If there is only one wall visible, the robot will need to try and stay a fixed distance away from that wall.
* When navigating its way around a maze using a wall-following algorithm, the side sensors also alow the robot to detect exactly when a wall 'disappears'. This can be used to trigger a turn and ensures that turns are accurate and repeatable.

You will notice that the sie sensors re mounted at an angle. Not only does this improve the reliability of the sensor, it ensures that the robot gets plnty of advance warning about the presence or absence of walls.

Even when following a left wall, the robot may need to turn right or even turn around completely if there is a wall in front. The front sensor lets the robot detect that wall and measure its distance. Just as with the side sensors, this positioning information allows the robot to make accurate turns and avoid collisions.

On this sensor board all three emitters are connected together so that the emitters all come on at the same time. This greatly simplified the circuit and the software but it may not be a good option for a full maze-solving robot with more complex sensor arrangements.

There are also two indicator LEDs. Typically, these will be used to tell the user when walls are visible but they may be used for anything you like as they are software controlled.

## Sensor Operation

#### Response

Just as with the line sensor, the reliable operation of the wall sensors needs two readings for each detector. First the detector is read with the emitter off (the 'dark' reading), then the emitter is turned on and the sensor read a second time (the 'lit' reading). Finally, the emitter is turned off again. The first reading lets the robot measure the ambient light level and the difference between the two readings should be almost entirely a function of the distance to the wall. In this way, much of the effect of ambient illumination can be eliminated. That said, high ambient light levels mean that the response from the wall sits on top of the ambient level.

#### Effect of ambient illumintion

The full range of values available to the processor is 0 to 1023. Suppose that the dark reading is 200, now there is only a range of 200 to 1023 available for the lit reading. As the ambient illumination increases. the available range for the wall response will get less. For this reason, the sensor response is designed so that the maximum response when the sensor is close to the wall will be limited to around 600 or so.

#### Effect of distance on side sensor

The sensors are very non-linear. That means that the response falls off very quickly as the distance increases. For the basic wall sensor board, set up to have a maximum side sensor response of 600, the measured response when the robot is centered in the maze cell might only be around 40 or 50.

#### Front sensor

Because the front sensor points more directlly at the wall, the responses will be bigger but will still fall off very quickly with distance. For example, with the robot centered in a maze cell, the front sensor may have a response of about 300. With the robot backed up against the rear wall of a cell and with a wall ahead, the response will be reduced to about 40 to 50.

#### Sensors close to wall

The detectors for these sensor are underneath the PCB so that they are shielded as much as possible from ambient illumination and direct light from the emitters. While this allows them to be more reliable in normal operation, it does have one disadvantage. As the sensor gets within 10mm or so of the wall, the detector can no longer see the spot of light from the emitter and so the response drops off. By the time the sensor PCB touches the wall, the respose will be zero. In normal operation, this is not a problem but you should watch out for misreadings if your robot runs into a wall.

#### Side Wall Detection

The side sensors are angled so that the spot of light from the emitters points at the far edge of the post when the robot is centred in a cell. It seems reasonable then that if half the spot is on the post, the reading from the sensor will be half the value when the spot is fully on a wall. This is indeed the case so it is convenient to set the threshold for wall detection at 50% of the value the sensor normally sees when centred between walls.

#### Front Wall detection

Perhaps the most important requirement for detecting walls is that they are seen in plenty of time for the robot to come to a halt without crashing - or at least, in time to perform a turn when running around in the maze. What you choose for a front wall detection threshold will depend on your needs. Consider setting up the robot so that it is backed up against a wall with another wall ahead. Then look at the front sensor reading. A reasonable starting point for the front detection threshold might be 25% of that value. That should let you reliably see a wall when the wheels cross the threshold into a new cell.

This 'back wall' reference can also be used to set the the reference value when the robot is centred in a cell and facing a wall. A good starting point might be 8 times the 'back wall' reference.

## Example Sketch

This is a long README! for an apparently simple sensor but good setup is important for reliable running. The example sets up the sensors to be sampled and read automatically 500 times per second. then the main loop displays the key values about 10 times per second on the serial terminal. The values of all three sensors are displayed along with the Cross Track Error. Like the line sensor, the CTE is a value that can be used to steer the robot by following the wall. In this example, the CTE is calculated in a similar way to the line sensor in that the magnitude of the response is cancelled out during the calculation. You might like to note that the code decides which wall is closest and uses that to calculate the error.

The code will also light the indicator LEDs to show when a wall is detected. The Arduino LED is used to show a wall in front.

Run the sketch with your robot. Examine the readings using the serial monitor and adjust the threshold values at the top of the program to get the results that work for you. This example can also be used to calibrate the sensors and adjust their orientation so that wall detection happens at the correct place.

## Eye Safety

If you are using high-power Infra Red emitters like the SFH4550, do not look into the emitters while they are operating. The light is invisibale and can, under some circumstances, present a risk of damage to the eye. Use low power emitters or visible light emitters for better results and greater safety.
