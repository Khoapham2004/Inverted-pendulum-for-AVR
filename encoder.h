#ifndef ENCDODER_H
#define ENCODER_H

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int16_t encoder1_position = 0;
volatile int16_t encoder2_position = 0;

void encoder_init(void);
int32_t encoder1_get(void);
int32_t encoder2_get(void);

/*

ISR(INT0_vect){
    if (PIND & (1<<PD3)){
        encoder1_position++;
    }else{
        encoder1_position--;
    }
}
    */

void encoder_init(void){
    DDRH &=~((1<<PH6)|(1<<PH5));
    PORTH |=((1<<PH6)|(1<<PH5));

    EICRA |= (1 << ISC00) | (1<< ISC01) ;//ngắt cạnh lên
    EIMSK |= (1 << INT0 ) ;

    sei();
}

int32_t encoder1_get(void){
    return encoder1_position ;
}
int32_t encoder2_get(void){
    return encoder2_position ;
}

#endif
