# Sensor Basics

UKMARSBOT has inputs for up to six analogue sensor channels. These are connected to pins A0 to A5 and canbe used for any kind of a sensor you like. The two main kinds of sensor board used in typical contests are the line sensor board and the wall sensor board.

For both of these sensor boards, there will be a phototransistor that can measure the amount of light that it sees. Associated with these are emitters which are either visible or infra-red light LEDs. Both type work well and you are free to choose whatever devices are most suited to your problem and available resources.

The simplest possible use of the sensors is to have the emitter turned on all the time so that you can measure the light seen by the detector whenever you want. This method is not very reliable though since the result will vary when the ambient illumination changes.

A much better scheme is to read the sensor value twice. First, it is read with the emitters off. This measures the amount of background light. Then the emitter is turned on again and the detector read a second time. By subtracting the first reading from the second, you end up with a value that is fairly independent of the ambient illumination and so is a much better indicator of what the sensor can see.

## An Example

This example sketch reads a single sensor channel using the technique described above. It will work with either the wall sensor or the line sensor and you can modify it for whatever channel your like.

Although it is a very simple example, this sketch can be a valuable tool for testing out diferent sensor devices and configurations. You can experiment with changes to the emitter LED type, the current limit resistor, the phototransistor type and the phototransistor load resistor. Also, try changing the orientation of the sensor and its distance from the surface or wall.

## Details

Here only channel A0 is used. On the wall sensor board, this may be the right wall distance sensor. On the line follower, it will be the right marker sensor. A single emitter enale pin turns on all the emitter LEDS so, if you are experimenting with only a single channel, be careful that any LEDs on the other channels are not affecting the results.

Only the standard Arduino functions are used. Notice that there is a short delay between turning on the emitter LED and reading the phototransistor. It takes a small amount of time for the phototransistor to respond and it is important to wait for this to happen. Without the delay, you will find that the sensor results are not consistent and will definitely be different from channel to channel.

The assumption in this sketch is that the sensor value after illumination is brighter than without illumination. This is true of the wall sensor but the basic line sensor is reversed. The more light it sees, the smaller is the answer. For that, you just need to reverse the sign of the difference.

The sketch will repeatedly activate and read the sensor channel and display the dark, lit and difference values. Examine the values while the ambient illumination changes. You should be able to see that the difference does not change much even though the dark and lit values do. An old-fashioned indandescent flashlight is good for these tests. Modern LED lamps may not affect the sensors. You can be sure that sunlight will though.

Occasionally, the lit value will be less than the dark velue (or more for the line sensor) so watch out for surprising results.

You should also note that it is best to put a barrier between the emitter and the detector so that the detector cannot see the emitter directly.

Take care to always turn off the emitters when the sensors are not in use.