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
 
 
// Motor Initialization routine -- this function must be called
//  before you use any of the above functions
void Port_init()
{
    // configure for inverted PWM output on motor control pins:
    //  set OCxx on compare match, clear on timer overflow
    //  Timer0 and Timer2 count up from 0 to 255
    TCCR0A = TCCR2A = 0xF3;
 
    // use the system clock/8 (=2.5 MHz) as the timer clock
    TCCR0B = TCCR2B = 0x02;
 
    // initialize all PWMs to 0% duty cycle
    OCR0A = OCR0B = OCR2A = OCR2B = 0;
 
    // set PWM pins as digital outputs (the PWM signals will not
    // appear on the lines if they are digital inputs)
    DDRD |= (1 << PORTD3) | (1 << PORTD5) | (1 << PORTD6);
    DDRB |= (1 << PORTB3);
	
	
	DDRD|=(1<<PORTD0); //	Set  PORTD0 as ultrasonic trigger.
		
	EIMSK |= (1<<INT0); //enable external interrupt
	EICRA |= (1<<ISC00); // Any logical change on INT0 generates an interrupt request.

	//Config and enable Timer1
	TCCR1B = (0<<CS12)|(1<<CS11)|(1<<CS10);
}



 

void delay_ms(unsigned int time_ms)
{
    // _delay_ms() comes from <util/delay.h> and can only
    //  delay for a max of around 13 ms when the system
    //  clock is 20 MHz, so we define our own longer delay
    //  routine based on _delay_ms()
 
    unsigned int i;
 
    for (i = 0; i < time_ms; i++)
        _delay_ms(1);       
}


 
int main()
{
	Port_init();//Port Initialization
	unsigned char sm;
	sei();
	
   
	while(1)
	{	
		DDRD &= ~(1 << PORTD1);     // Clear the PB0 pin
		// PD1 (PCINT17 pin) is now an input

		PORTB |= (1 << PORTD1);    // turn On the Pull-up
		// PD1 is now an input with pull-up enabled


		PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
		PCMSK0 |= (1 << PCINT17);  // set PCINT0 to trigger an interrupt on state change

		
	
		 M1_forward(80);
		 M2_forward(80);
		 PORTD|=(1<<PIND0); // trigger=1 high pulse
		 _delay_us(10); // wait minimum 10 us
		 PORTD &=~(1<<PIND0); // trigger=0 low pulse
		 Distance = pulse/58; // take distance in cm  (29 microsecond per cm *2)
		 _delay_ms(20);
		 if(Distance <=20) // compare the value of count which is distance
		 {
			M1_forward(0);
			M2_forward(0);
		 }
			 
		_delay_ms( 10 );
	}
	
	}


ISR(INT0_vect)
{
	if(i == 1)
	{
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		i = 0;
	}

	if(i==0)
	{
		TCCR1B |= 1<<CS10;
		i = 1;
	}
}

char check,data,invdata,count=0;

ISR (PCINT0_vect)
{
	/* interrupt code here */
	if 
}

	