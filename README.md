# Microcontroller-car-
Automated car sensing IR and ultrasonic waves
-----------------------------------------------------------------------------------------------
There are 2 scripts in this repository namely Car.c and TrafficLights.c
------------------------------------------------------------------------------------------------
Car.c is the script written for the car powered by Atmega328P
The sensors used in this car are Ultrasonic sensor and IR Receiver
Ultrasonic sensor uses an interrupt to function whenever and obstacle is seen
IR receiver is running in the if loop in the main while loop.

The IR receiver looks for a red signal to stop or green to move the car.
-----------------------------------------------------------------------------------------------
TrafficLight.c is the script written to blink the traffic lights which controls the car movement
Mini traffic light LED component is used to function as a Traffic Light system
This system is also powered by an atmega328P.

The car does not read the traffic lights directlty.
Here the red traffic light is synched with and IR Led.
IR led pulses at around 38KHZ frequency from the timer of the controller.
The car stops when the IR Led is transmitting or IR Receiver is receiving signal.
---------------------------------------------------------------------------------------------------------

