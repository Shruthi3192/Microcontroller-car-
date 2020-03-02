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
	setIrModOutput();
	TIMSK2 = (1<<OCIE2B); // Output Compare Match B Interrupt Enable
    /* Replace with your application code */
	uint8_t i;
	DDRD |= (1<<PIND3)|(1<<PIND6)|(1<<PIND7); // pins 3(Red),6(Green),7(Yellow) set to output on PORT D
	//DDRB = 0b00000001; // some pins of PORT B as output
    while (1) 
    {
		// want to have green light glowing to begin with
		//Hence turn on green light at 6
		PORTD =0b01000000;
		_delay_ms(500);
		PORTD = 0b00001000;// turn on red light
		_delay_ms(500);
		PORTD = 0b10000000;//turn on yellow light
		_delay_ms(500);
		//PORTD = 0b00100000;
		//_delay_ms(50);
		
		
		}
		
}


volatile int pulse = 0;

ISR(TIMER2_COMPB_vect){  // Interrupt service routine to pulse the modulated pin 3
	pulse++;
	if(pulse >= 8) { // change number for number of modulation cycles in a pulse
		pulse =0;
		TCCR2A ^= (1<<COM2B1); // toggle pin 3 enable, turning the pin on and off
	}
}

void setIrModOutput(){  // sets pin 3 going at the IR modulation rate
	//DDRD|=(1<<PIND3);
	TCCR2A = (1<<COM2B1) | (1<<WGM21) | (1<<WGM20); // Just enable output on Pin 3 and disable it on Pin 11
	TCCR2B = (1<<WGM22) | (1<<CS22);
	OCR2A = 51; // defines the frequency 51 = 38.4 KHz, 54 = 36.2 KHz, 58 = 34 KHz, 62 = 32 KHz
	OCR2B = 26;  // defines the duty cycle - Half the OCR2A value for 50%
	TCCR2B = TCCR2B & 0b00111000 | 0x2; // select a prescale value of 8:1 of the system clock
}

void setup(){
	setIrModOutput();
	TIMSK2 = (1<<OCIE2B); // Output Compare Match B Interrupt Enable
}

//



