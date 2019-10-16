# Systick - working in the background

Robot control software works best when the control tasks are executed at fixed, regular intervals. For contest robots like UKMARSBOT, these tasks must happen very frequently - hundreds of times per second. If the taasks are not executed regularly then control will be much more difficult and can be very unreliable.

The kinds of task that must be performed regularly include reading the wheel encoders, checking the battery voltage, monitoring the sensors, calculating motor speeds and updating the motor drive. Other robots may need additinal tasks.

Your robot code can take care of these tings as part of the main processing loop but it can get really difficult to make certain all the tasks are completed at the right time, in the right order and at the correct rate.

Full-blown operating systems for robots as well as your desktop computer, laptop, tablet or 'phone are able to multitask. That is, they appear to be doing many things at once. In fact, they have background and foreground tasks. For example, the mouse pointer on your computer is managed by a background task that is part of the operating system. People who write a program to run on the computer do not have to take care of moving the mouse pointer araund and they certainly would find it difficult to coordinate that movement with other programs running on the same computer. Instead, they can just ask the operating system where the mouse is, safe in the knowledge that the job is already taken care of.

In the robot, it would be really helpful if tasks like reading the battery voltage and getting sensor readings were already taken care of for you so that you only have to look up the results. Tasks that are performed in this way, without involving the main code, are said to be running 'in the background'. For UKMARSBOT, there is a fairly easy way to make that happen.

## Interrupts

The controller in your robot can make use of a feature called an interrupt. That is, you can write code that only gets run if a specific event happens. Your main program goes about its business and, when the event happens, your program is put on hold and the the corresponding interrupt code is executed. Once the interrupt code has finished, your program picks up exactly where it left off as if nothing had happened. It is just like hitting pause on a video if a phone call comes.

Also present in the controller are a number of timers. These can be set up to generate a timer event at regular intervals. This is just what you want for the regular task execution. one of these timers is confiigured to generate a timer interrupt event at some fixed frequency and you write a special section of code called and Interrupt Service Routine (ISR) that responds to the interrupt. The processor looks after the whole business of suspending your main program, running the ISR and then returning to the main code as if nothing has happened.

On some processors, there is a built-in facility for generating regular interrupts for servicing system tasks. On those processors, the interrupt is called SysTick. The normal processor on UKMARSBOT does not have a built-in SysTick event but the name is still used hear to try and be clear what this event is used for.

## Example.

This is not the place to go into how to configure interrupts in the processor. That is better dealt with elsewhere. For now, just know that the example code sets up the controller so that it is interrupted 500 times per second. The ISR in the example just does two things - it reads the battery voltage and the function switches. The results of these operations are stored in global variables ready for use by the main code.

The main code in the example just sits prints out the current battery voltage and the value of the function switches and then prints them to the serial monitor. Also, the function switches are tested and, if the push button is pressed, the built-in LED lights up.

You will notice that the main code does not have to call the ```analogueRead()``` function or know anything about how the function switches work. All it needs to do is look up the results obtained in the background by the ISR.

Many different kinds of task can be performed in the ISR but care is needed. Most importantly, the ISR code must not take more time to execute than the time between interrupts - 2 ms in this case. Never make cals to the serial port functions in the ISR, they are low priority taks than can take a long time to execute.




