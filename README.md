# Stop_watch_myfirst_ES_Project
this is my first embedded system project without using any drivers so it was very very hard to build, design and implement a digital stopwatch with two operational modes: counting up and counting down. The project utilizes an ATmega32  microcontroller, six multiplexed seven-segment displays (com_anode), and multiple push  buttons for interaction.

System Requirements:
1. Use ATmega32 Microcontroller.
   
2. System Frequency: 16Mhz
 
3. Configure Timer1 in ATmega32 with CTC mode to count the stopwatch time.

4. Connect the six 7-segments in the project using the multiplexed technique.

5. Stopwatch counting should start once the power is connected to the MCU.

6. Configure External Interrupt INT0 with falling edge. Connect a push button with the internal 
pull-up resistor. If a falling edge detected the stopwatch time should be reset. 

7. Configure External Interrupt INT1 with raising edge. Connect a push button with the 
external pull-down resistor. If a raising edge detected the stopwatch time should be paused.

8. Configure External Interrupt INT2 with falling edge. Connect a push button with the internal 
pull-up resistor. If a falling edge detected the stopwatch time should be resumed.

9. Countdown Mode Setup:
To operate the stopwatch in countdown mode, follow these steps:
a. Pause the Timer:
Ensure that the timer is paused. This can be done by pressing the pause button connected 
to PD3 (INT1).
b. Toggle to Countdown Mode:
Use the mode toggle button connected to PB7 to switch the stopwatch from the default 
increment mode to countdown mode.
c. Adjust the Countdown Start Time:
Set the desired start time for the countdown using the adjustment buttons.
6
d. Resume the Countdown:
Once the desired countdown time is set, press the resume button connected to PB2 (INT2) 
to start the countdown.
e. Buzzer Activation:
When the countdown reaches zero, the buzzer connected to the alarm circuit will be 
triggered, alerting the user that the countdown has finished.
