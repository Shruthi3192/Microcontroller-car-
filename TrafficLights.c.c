/*
 * Traffic Lights.c
 *
 * Created: 10-02-2020 15:58:17
 * Author : ashru
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>



int main(void)
{
    /* Replace with your application code */
	uint8_t i;
	DDRD = 0b11100000; // pins 5(Red),6(Green),7(Yellow) set to output on PORT D
	//DDRB = 0b00000001; // some pins of PORT B as output
    while (1) 
    {
		// want to have green light glowing to begin with
		//Hence turn on green light at 6
		PORTD =0b01000000;
		_delay_ms(500);
		PORTD = 0b00100000;// turn on red light
		_delay_ms(500);
		PORTD = 0b10000000;//turn on yellow light
		_delay_ms(500);
		//PORTD = 0b00100000;
		//_delay_ms(50);
		
		
		}
		
}


