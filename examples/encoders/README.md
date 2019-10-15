The ATMega328p  used in the Arduino nano has INT0 and INT1 connected to two of the digital pins:

>    PD2 for INT0  - Arduino Digital Pin 2<br />
>    PD3 for INT1  - Arduino Digital Pin 3

To tell the processor to generate interrupts when these pins change state, values must be written to particular bits in the External Interrupt Control Register A  (```EICRA```) and the External Interrupt Mask egister (```EIMSK```).

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