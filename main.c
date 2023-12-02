/*
 * Water Level Project.c
 *
 * Created: 02.12.2023 14:18:48
 * Author : calme
 */ 
#define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUZZER		PORTB1
#define LED_GREEN	PORTB2
#define LED_YELLOW	PORTB3
#define LED_RED		PORTB4
#define PORT		PORTB
#define DDR			DDRB

int maxSignalValue = 700;
int minSignalValue = 0;

void setup() {
	//Output mode of LEDs and buzzer
	PORT |= (0<<BUZZER) | (0<<LED_GREEN) | (0<<LED_YELLOW) | (0<<LED_RED);
	DDR |= (1<<BUZZER) | (1<<LED_GREEN) | (1<<LED_YELLOW) | (1<<LED_RED);
	//PWM for buzzer
	TCCR1A |= (0<<COM1A1) | (1<<COM1A0) | (1<<WGM11) | (1<<WGM10);
	TCCR1B |= (1<<WGM13) |(1<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
	//ADC input mode
	ADMUX |= (0<<REFS1) | (0<<REFS0) | (0<<ADLAR) | (0<<MUX3) | (1<<MUX2) | (0<<MUX1) | (1<<MUX0);
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
	ADCSRB |= (0<<ADTS2) | (1<<ADTS1) | (1<<ADTS0);
	//One second timer
	//MCU reads ADC and updates state on timer interruption
	TCCR0A |= (0<<COM0A1) | (0<<COM0A0) | (1<<WGM01) | (1<<WGM00);
	TCCR0B |= (1<<WGM02) | (0<<CS02) | (0<<CS01) | (0<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 255;
}

void initialize() {
	//Blinking with LEDs
	for(int i = 0; i < 3; i++) {
		PORT |= (1<<LED_GREEN) | (1<<LED_YELLOW) | (1<<LED_RED);
		_delay_ms(500);
		PORT &= ~(1<<LED_GREEN) & ~(1<<LED_YELLOW) & ~(1<<LED_RED);
		_delay_ms(500);
	}
}

void start() {
	//Start timer
	TCCR0B |= (1<<CS02) | (0<<CS01) | (1<<CS00);
}

void turn_off_all() {
	//Turn off LEDs and buzzer
	PORT &= ~(1<<LED_GREEN) & ~(1<<LED_YELLOW) & ~(1<<LED_RED);
	TCCR1B &= ~(1<<CS10);
}

void change_state(int signal) {
	//If level 0-25%:
	//Enable nothing
	//If level 25-50%:
	//Enable green LED
	//If level 50-75%:
	//Enable yellow LED and buzzer (OCR1A = 0x0FFF)
	//If level 75-100%:
	//Enable red LED and buzzer (OCR1A = 0x0EFF)
	int percentage = ( ((signal-minSignalValue)*1.0) / (maxSignalValue-minSignalValue) ) * 100;
	if(percentage <= 25) {
		//Nothing
	}
	else if(percentage <= 50) {
		PORT |= (1<<LED_GREEN);
	}
	else if(percentage <= 75) {
		PORT |= (1<<LED_YELLOW);
		TCCR1B |= (1<<CS10);
		OCR1A = 0x0FFF;
	}
	else if(percentage <= 100) {
		PORT |= (1<<LED_RED);
		TCCR1B |= (1<<CS10);
		OCR1A = 0x0EFF;
	}
} 

ISR(TIMER0_COMPA_vect) {
	turn_off_all();
	change_state(ADC);
}

int main(void) {
	
    setup();
	initialize();
	start();
	sei(); //Enable interrupts
	
    while (1) {

    }
}

