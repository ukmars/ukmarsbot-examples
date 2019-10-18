# Reading the Encoders

UKMARSBOT can use encoders attached to the drive motors. These encoders generate pulses when the motor shaft rotates. Normally, magnetic encoders are used and these generate 12 counts per rotation of the shaft. This example shows how to decode the signals from the motor encoders and use them to update some variables in the program.

Note that your particular robot may have the encoders wired in reverse compared to other examples. Load the sketch and run it. You will see pairs of numbers displayed in the serial monitor. These numbers will change as you turn the wheels. Turning the wheel forward should make the corresponding counter increase. Turning it backward should make the counter decrease. You can modify the relevant code section is the numbers change in the wrong direction.

Check that the counters change by the correct amount for your motor and gearbox. There should be 12 counts per complete rotation of the motor shaft. So, if the motor has a 20:1 gearbox, you should see a change of 12 x 20 = 240 counts for each complete rotation of a wheel.

## Code Details

The ATMega328p used in the Arduino nano has INT0 and INT1 connected to two of the digital pins:

>    PD2 for INT0  - Arduino Digital Pin 2
>    PD3 for INT1  - Arduino Digital Pin 3

To tell the processor to generate interrupts when these pins change state, values must be written to particular bits in the External Interrupt Control Register A  (```EICRA```) and the External Interrupt Mask register (```EIMSK```).

### The EICRA

For each of the pin change interrupts, the processor must be told what kind of change can generate an interrupt. The bits that control these settings are found in the External Interrupt Control Register A  (```EICRA```).

The External Interrupt 0 is activated by the external pin INT0 if the corresponding interrupt mask is set and global interrupts are enabled. The level and edges on the external INT0 pin that activate the interrupt are defined as

|ISC01|ISC00|Description|
|:---:|:---:|:---|
|  0  |   0  |Low Level of INT0 generates interrupt|
|  0  |   1  |Logical change of INT0 generates interrupt|
|  1  |   0  |Falling Edge of INT0 generates interrupt|
|  1  |   1  |Rising Edge of INT0 generates interrupt|


The External Interrupt 1 is activated by the external pin INT1 if the corresponding interrupt mask is set and global interrupts are enabled. The level and edges on the external INT1 pin that activate the interrupt are defined as

|ISC11|ISC10|Description|
|:---:|:---:|:---|
|  0  |   0  |Low Level of INT1 generates interrupt|
|  0  |   1  |Logical change of INT1 generates interrupt|
|  1  |   0  |Falling Edge of INT1 generates interrupt|
|  1  |   1  |Rising Edge of INT1 generates interrupt|

To respond to changes from the motor encoders, the proper setting is for any logical change in the pin state.

### The EIMSK

A single register in the processor holds bits that enable and disable the various sources of interrupts. To enable these pin change interrupts, two bits in this register must be set.

```EIMSK:INT0``` (bit 0) enables the INT0 external interrupt<br>
```EIMSK:INT1``` (bit 1) enables the INT1 external interrupt

### DigitalFastWrite

You will see that there is an extra file in this sketch (and the others) - ```digitalFastWrite.h```.

The file is needed to make sure that the encoder interrupts are serviced as fast as possible. The built in ```digitalRead()``` function is not very efficient and the interrupts may arrive at several tens of thousands per second. The code as shown in this example can process each interrupt in under 2.5 microseconds only because it uses ```digitalFastRead()``` instead of the built-in call.

To use this feature you only have to make sure that the ```digitalFastWrite.h``` file is present in the same folder as the sketch and then add the following line at the top of the main sketch ```.ino``` file:

    #include "digitalWriteFast.h"

The Arduino IDE will take care of the rest.