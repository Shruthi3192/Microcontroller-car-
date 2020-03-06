/*
 * Car.c
 *
 * Created: 10-02-2020 14:02:16
 * Author : ashru
 */
  
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#define F_CPU 20000000  // system clock is 20 MHz
int16_t Distance = 0;
static volatile int pulse = 0;
static volatile int i = 0;

 
// Motor Control Functions -- pwm is an 8-bit value
//  (i.e. ranges from 0 to 255)
 
void M1_forward(unsigned char pwm)
{
  OCR0A = 0;
  OCR0B = pwm;
}
 
void M1_reverse(unsigned char pwm)
{
  OCR0B = 0;
  OCR0A = pwm;
}
 
void M2_forward(unsigned char pwm)
{
  OCR2A = 0;
  OCR2B = pwm;
}
 
void M2_reverse(unsigned char pwm)
{
  OCR2B = 0;
  OCR2A = pwm;
}
 
 

void Port_init()
{
	//------------------ MOTOR CONTROL ----------------------------------------
    // configure for inverted PWM output on motor control pins:
    //  set OCxx on compare match, clear on timer overflow
    //  Timer0 and Timer2 count up from 0 to 255
    TCCR0A |=(1<<WGM00)|(1<<WGM01)|(1<<COM0A0)|(1<<COM1A1)|(1<<COM0B1)|(1<<COM0B0);
	TCCR2A |=(1<<WGM20)|(1<<WGM21)|(1<<COM2A0)|(1<<COM2A1)|(1<<COM2B1)|(1<<COM2B0);
 
    // use the system clock/8 (=2.5 MHz) as the timer clock
    TCCR0B |= (1<<CS01);
	TCCR2B |=(1<<CS21);
 
    // initialize all PWMs to 0% duty cycle
    OCR0A = 0;
	OCR0B = 0;
	OCR2A = 0;
	OCR2B = 0;
 
    DDRD |= (1 << PORTD3) | (1 << PORTD5) | (1 << PORTD6);
    DDRB |= (1 << PORTB3);
	
	//------------------ ULTRASONIC SENSOR CONTROL ----------------------------------------
	
	DDRD|=(1<<PORTD0); //	Set  PORTD0 as ultrasonic trigger(output). PORTD2/INT0 as ultrasonic echo
		
	EIMSK |= (1<<INT0);  //enable external interrupt
	EICRA |= (1<<ISC00); // Any logical change on INT0 generates an interrupt request.

	//Config and enable Timer1
	TCCR1B = (0<<CS12)|(1<<CS11)|(1<<CS10);
}



 
 
int main()
{
	Port_init();//Port Initialization
	DDRC = (1<<PORTC4)|(1<<PORTC5);// test led's to check interrupt working of ultrasonic sensor
	sei(); //enabling global interrupt
  
  
	while(1)
	{	
				
		if((PINC & (1<<PORTC2))) // if the IR receiver is not transmitting or seeing green traffic light ::: MOTORS ARE FUNCTIONING
		{
			_delay_ms( 100 );
		 
			 PORTD|=(1<<PIND0);       // trigger=1 high pulse for ultrasonic sensor
			 _delay_us(10);           // wait minimum 10 us
			 PORTD &=~(1<<PIND0);      // trigger=0 low pulse
		 
			pulse = 0;
			while (pulse == 0) // to give time for the interrupt to be hit
			{
			}
		 
			Distance = pulse/58;      // take distance in cm  (speed of sound in air 29 microsecond per cm *2)
			 if (Distance <=20)         // compare the value of count which is distance
			{ 
				//if distance of obstacle is 20cm or less stop the motors
				M1_forward(0);
				M2_forward(0);
				PORTC=(1<<PORTC5);
			
			}
			else
			{
				M1_forward(100);
				M2_forward(100);
				PORTC=(1<<PORTC4);
			 }
		}// end of if condition checking for IR receiver transmission
	
		else
		{// if the IR receiver is  transmitting or seeing red traffic light ::: MOTORS ARE STOPPED
			M1_forward(0);
			M2_forward(0);
		}
	}// end of while
}// end of main


ISR(INT0_vect) 
{ 
	//PIND2 connected to echo, interrupt triggered on any change in logic of this pin
	// if PIND2 on the rising edge
	if((PIND & (1<<PORTD2)))
	{
		TCCR1B |= 1<<CS10;
		TCNT1 = 0;
	}

	else
	{
		// if PIND2 on the falling edge
		TCCR1B = 0;
		pulse = TCNT1; // variable which keeps track of the amount of time it took for the echo to register the signal
	
	}
}

