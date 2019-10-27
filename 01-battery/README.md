# Reading The Battery Voltage

UKMARSBOT has a way to measure the battery voltage. This is important for two main reasons.

The first is so that the robot can detect a failing battery and shut itself down or notify the handler. A low battery can make the robot behave unpredictably but, more importantly, some types of battery can be damaged if the voltage gets too low.

The second reason is that, when driving the motors of a robot, you really want to be able to get consistent results - even if the battery voltage changes. Not only will the battery voltage change as the charge is used up, different batteries and battery types have different voltages. Unless you take this into account, you will be forever trying to work out why performance is inconsistent. It is very frustrating when a day's work setting up the perfect parameters is all destroyed just because the replacement battery is a NiMh type instead of a LiIon or Alkaline battery. The voltage is different and the robot will not behave the same way.

The question of how to compensate for these changes is dealt with elsewhere. Here the only concern is how to measure the battery voltage.

## Details

On UKMARSBOT, there is a diode between the battery positive terminal and the battery supply input, Vin, to the Arduino controller. That diode prevents damage in case the battery is connected in reverse. The diode causes a small loss in voltge from the battery but that will not be significant in normal use.

The Vin supply is then is connected to a voltage divider made up of two resistors. The output of that voltage divider is then fed into the Arduino analogue channel A7 where it can be read with the built in ```analogueRead()``` function.

This function returns a number between 0 and 1023 inclusive from the Analogue to Digital Converter (ADC) which corresponds to a voltage between 0 and 5 Volts on the pin. If more than 5 Volts is applied to the pin, the processor is likely to be damaged so the voltage divider on the Vin supply divides the voltage by two. Thus, if Vin is at 8 Volts, there will be 4 Volts on the A7 pin and the ADC value will be about 818. Since the voltage divider ratio is known, it is easy enough to convert the ADC reading into a Voltage. This sketch uses a floating-point calculation to show the Vin voltage directly in Volts.

It is always better, where possible, to use real, natural units for measurements and parameters.

## Example Code

When you run the program on your robot, you can use the serial monitor to see the output. The program repeatedly sends lines displaying the reading from the ADC and the calculated voltage on the Vin pin of the Arduino. It is normal for the reading to fluctuate a little as the analogueRead() function gives a result that is only acurate to within a couple of digits.

With only USB power applied, you can expect to see about 4.4 Volts on Vin because it is supplied through the on-board regulator being driven backwards. With the battery connected and turned on, you would see a little bit less than the actual battery voltage because of the protection diode in the suppply circuit.
