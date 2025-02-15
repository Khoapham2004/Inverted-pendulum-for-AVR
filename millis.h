#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long timer2_millis = 0;

ISR(TIMER2_COMPA_vect) {
    timer2_millis++; // Mỗi lần ngắt, tăng 1ms
}

void millis_setup() {
    cli();  // Tắt ngắt toàn cục
    TCCR2A = (1 << WGM21);  // Chế độ CTC (Clear Timer on Compare Match)
    TCCR2B = (1 << CS22);   // Prescaler = 64
    OCR2A = 249;            // So sánh với 249 để tạo ngắt mỗi 1ms (16MHz / 64 / 250 = 1kHz)
    TIMSK2 = (1 << OCIE2A); // Bật ngắt Timer2 khi so sánh trùng khớp
    sei();  // Bật ngắt toàn cục
}

unsigned long millis() {
    unsigned long ms;
    cli();  // Ngăn gián đoạn khi đọc giá trị
    ms = timer2_millis;
    sei();
    return ms;
}