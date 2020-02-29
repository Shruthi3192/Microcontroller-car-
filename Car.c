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
volatile unsigned short hc_sr04_cnt;

 
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
void motors_init()
{
    // configure for inverted PWM output on motor control pins:
    //  set OCxx on compare match, clear on timer overflow
    //  Timer0 and Timer2 count up from 0 to 255
    TCCR0A = TCCR2A = 0xF3;
 
    // use the system clock/8 (=2.5 MHz) as the timer clock
    TCCR0B = TCCR2B = 0x02;
 
    // initialize all PWMs to 0% duty cycle (braking)
    OCR0A = OCR0B = OCR2A = OCR2B = 0;
 
    // set PWM pins as digital outputs (the PWM signals will not
    // appear on the lines if they are digital inputs)
    DDRD |= (1 << PORTD3) | (1 << PORTD5) | (1 << PORTD6);
    DDRB |= (1 << PORTB3);
}


void Port_Init()
{
	DDRD|=(1<<PORTD0); //	Set all pins of the PORTD as output, PORTD0 as input trigger.
	PORTD|=0b00000000; // Set all pins of PORTD low which turns it off.
	//DDRB|=0b00000000;
	//PORTB|=0b00000000;
	
	EIMSK |= (1<<INT0); //enable external interrupt
	EICRA |= (1<<ISC00); // Any logical change on INT0 generates an interrupt request.

	//Config and enable Timer1
	TCCR1B = (0<<CS12)|(1<<CS11)|(1<<CS10);
}

ISR( INT0_vect )
{
	//echo
	if(PORTD & (1 << PORTD2)) 
	{TCNT1 = 0;}/* Clear Timer counter */
	else  
	{hc_sr04_cnt = TCNT1;
	if (hc_sr04_cnt/2 * 34300)<10)
	{
		M1_forward(0);
		M2_forward(0);
	}
	}
	

}

unsigned char hc_sr04_meas( void )
{
	
	hc_sr04_cnt = 0;
	
	// transmit at least 10 us trigger pulse to the HC-SR04 Trig Pin(d0).
	PORTD |=  (1 << PORTD0);
	_delay_us( 20 );
	PORTD &= ~(1 << PORTD0);


	while( hc_sr04_cnt == 0 );
	
	//if (hc_sr04_cnt<3700)
	 //{
		 return  hc_sr04_cnt/2 * 34300;
		// } 
	//else {return 41;}
	
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
	int a =0;
    motors_init();
	unsigned char sm;
	Port_Init(); // Port Initialization
	sei();
	
   
	while(1)
	{
		 M1_forward(100);
		 M2_forward(100);
		 sm = hc_sr04_meas();
		 a=hc_sr04_cnt/1000;
		if (sm<50 && sm>40)
		{
				
		M1_forward(0);
		M2_forward(0);
		}
	
		 		 
		_delay_ms( 20 );
	}
		

		
	}
	
	