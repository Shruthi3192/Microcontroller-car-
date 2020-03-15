/*
 * GccApplication1_test.c
 *
 * Created: 06-03-2020 17:00:56
 * Author : ashru
 */ 

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>


void Port_Initialisation()
{
//PORTD4 used for green led, PORTD3 used for red led, IR led connected at PORTD5
DDRD|=(1<<PORTD5)|(1<<PORTD3)|(1<<PORTD4);	
	
}

int main(void)
{
	Port_Initialisation();
    TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0B1);// Counter used to pulse modulate IR
	TCCR0B = (1<<WGM02)|(1<<CS00); // Counter used to create IR signals
	OCR0A = 25; // Peak to reset the counter at 25 microseconds
	OCR0B = 13;// IR signals rising and falling edge of 13 micro seconds
    while (1) 
    {
		//_delay_ms(2000);
		TCCR0A =(1<<WGM01)|(1<<WGM00)|(0<<COM0B1); // IR not transmitting (turning off COM0B1)
		PORTD =(1<<PORTD4);
		_delay_ms(1500);
		TCCR0A =(1<<WGM01)|(1<<WGM00)|(1<<COM0B1); // IR transmitting (turning on COM0B1)
		PORTD =(1<<PORTD3);
		_delay_ms(1500);
		
    }
}

