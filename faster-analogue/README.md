# Faster Analogue conversions

For most purposes, you can just use the built-in Arduino functions for reading analogue sensors. They are safe and reliable and everyone understands how they work.

However, when you want to sample a lot of sensor very frequently, the built in functions are just too slow. On UKMARSBOT, I might want to read all eight analogue channels (six sensors, the battery and the function selector) underds of times per second. That will bog the processor down and not leave enough time to look after the rbot motion and follow a line of navigate a maze.

The standard conversion takes 128us to complete and if I were to read every sensor channel twice along with the battery an function switch then I would need 1792 microseconds just for that. Add in a bit of overhead while waiting for the sensors to respond and do the function calls and it takes nearly 2ms for do all the conversions. If I tried to do that 500 times per second, the robt would have literaly no time for anything else.

It is not difficult to speed up analogue conversions but there is a small penalty. the faster the conversions happen, the less accurate they become. A few experiments show that, for UKMARSBOT, a reasonable compromise is to set the conversion time to 28 microseconds. This still gives enough accuracy but reduces the sensor reading cycle to something like 500 microseconds total. That would only take up 25% of the available processor time if the control loop frequency is 500Hz.

## Details

### ADCSR - The Analogue Control and Status Register

The bottom three bits of ADCSR, determine the prescaler factor. The time shown is for a single conversion to a floating point value with this line of code:

     battery = analogRead(A7) * 4.8828; // 5000/1024

The variance is calculated over 10000 samples with the analogue input set to as close as possible to 2.5V with a potentiometer. Variance is directly related to the ammount of noise in the result.

The time taken for each call is measured by setting and clearing a port pin by direct port manipulation to reduce overhead as much as possible. The length of the resulting pulse is measured on an oscilloscope.

### Results

|ADPS2|ADPS1|ADPS0|Divisor|analogRead()| Variance |
|:---:|:---:|:---:|------:|-----------:|---------:|
|  0  |  0  |  0  |   2   |     16 us  |         -|
|  0  |  0  |  1  |   2   |     16 us  |         -|
|  0  |  1  |  0  |   4   |     18 us  |195.0493  |
|  0  |  1  |  1  |   8   |     22 us  | 18.9510  |
|  1  |  0  |  0  |  16   |     28 us  |  1.0999  |
|  1  |  0  |  1  |  32   |     43 us  |  0.4864  |
|  1  |  1  |  0  |  64   |     68 us  |  0.2682  |
|  1  |  1  |  1  | 128   |    120 us  |  0.3425  |

Normally all three bits are set to 1.

Clearly, the conversions are less accurate as the ADC clock increases. By the time the divisor is down to 8 (ADC clock = 2MHz), the ADC is working at no more than 8 bits of useful resolution. If that is enough for the application then it will be reasonably effective. Some accuracy can be recovered if the results are filtered with a moving average or exponential filter.

## The Sketch

This exaple just shows how to set up the ADC system for faster conversions. The rest of the code is identical to the sensor-basics example so you can compare results. You will possibly notice that the sensor reading varies a little more but the average is the same. For those who care - because the sensor value is a difference of two numbers that are themselves more variable, the overall variability of the result is more than that of individual cannels. Don't worry - it all works out just fine.