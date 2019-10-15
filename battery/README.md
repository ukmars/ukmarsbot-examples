# Reading The Battery Voltage

UKMARSBOT has a way to measure the battery voltage. This is important for two main reasons.

The first is so that the robot can detect a failing battery and shut itself don or notify the handler. A low battery can make the robot behave unpredictably but, more importantly, some types of battery can be damaged if the voltage gets too low.

The second reason is that, when driving the motors of a robot, you really want to be able to get consistent results - even if the battery voltage changes. Not only will the battery voltage change as the charge is used up, different batteries and batery types have different voltages. Unless you take this into account, you will be forever trying to work out why performance is inconsistent.It is very frustrating when a day's work setting up the perfect parameters is all destroyed just because the replacement batery is a NiMh type instead of a LiIon or Alkaline battey. The voltage is different and the robot will not behave the same way.

The question of how to compensate for these changes is dealt with elsewhere. Here the only concern is how to measure the battery voltage.

## Details

The battery positive terminal is connected to a oltage divider made up of two resistors. The output of that voltage divider is then fed into the Arduino analogue channel A7 where it can be read with the built in ```analogueRead()``` function.

This function returns a number between 0 and 1023 inclusive which corresponds to a voltage between 0 and 5 Volts on the pin. If more than 5 Voltsis applied to the pin, the processor is likely to be damaged so the voltage divider on the battery divides the battery voltage by two. Thus, if the battery is at 8 Volts, there will be 4 Volts on the A7 pin and the ADC value will be about 818. Since the vlotage divider ratio is known, it is easy enough to convert the ADC reading inot a Voltage. This sketch uses a floating point calculation to show the battery voltage directly in Volts.

It is always better, where possible, to use real, natural units for measurements and parameters.