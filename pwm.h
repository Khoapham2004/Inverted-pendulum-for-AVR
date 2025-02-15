#ifndef PWM_H
#define PWM_H
#include <avr/io.h>

// Định nghĩa chế độ PWM
#define FAST_PWM 1
#define PHASE_CORRECT_PWM 2

void PWM_Init(uint8_t timer, uint8_t channel, uint8_t mode, uint16_t prescaler);
void PWM_SetDutyCycle(uint8_t timer, uint8_t channel, uint8_t duty);
void PWM_Stop(uint8_t timer);

// Hàm khởi tạo PWM cho từng Timer và Channel
void PWM_Init(uint8_t timer, uint8_t channel, uint8_t mode, uint16_t prescaler) {
    switch (timer) {
        case 1:  // Timer1 (16-bit)
            DDRB |= (1 << PB5);  // OC1A (PB5) đầu ra
            DDRB |= (1 << PB6);  // OC1B (PB6) đầu ra
            TCCR1A = (1 << COM1A1) | (1 << COM1B1); // Non-inverting mode
            if (mode == FAST_PWM) {
                TCCR1A |= (1 << WGM11);
                TCCR1B |= (1 << WGM12) | (1 << WGM13);
            } else if (mode == PHASE_CORRECT_PWM) {
                TCCR1A |= (1 << WGM11);
            }
            TCCR1B |= prescaler;
            ICR1 = 255; // Độ phân giải PWM 8-bit
            break;

        case 3:  // Timer3 (16-bit)
            DDRE |= (1 << PE3);  // OC3A (PE3) đầu ra
            DDRE |= (1 << PE4);  // OC3B (PE4) đầu ra
            TCCR3A = (1 << COM3A1) | (1 << COM3B1);
            if (mode == FAST_PWM) {
                TCCR3A |= (1 << WGM31);
                TCCR3B |= (1 << WGM32) | (1 << WGM33);
            } else if (mode == PHASE_CORRECT_PWM) {
                TCCR3A |= (1 << WGM31);
            }
            TCCR3B |= prescaler;
            ICR3 = 255;
            break;
    }
}

// Hàm đặt Duty Cycle cho từng kênh PWM
void PWM_SetDutyCycle(uint8_t timer, uint8_t channel, uint8_t duty) {
    switch (timer) {
        case 1:
            if (channel == 1) OCR1A = duty; // Điều khiển OC1A
            if (channel == 2) OCR1B = duty; // Điều khiển OC1B
            break;
        case 3:
            if (channel == 1) OCR3A = duty; // Điều khiển OC3A
            if (channel == 2) OCR3B = duty; // Điều khiển OC3B
            break;
    }
}

// Hàm dừng PWM
void PWM_Stop(uint8_t timer) {
    switch (timer) {
        case 1:
            TCCR1A = 0;
            TCCR1B = 0;
            break;
        case 3:
            TCCR3A = 0;
            TCCR3B = 0;
            break;
    }
}

#endif
