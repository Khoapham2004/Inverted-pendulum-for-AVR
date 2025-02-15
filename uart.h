#ifndef UART_H
#define UART_H

#include <avr/io.h>

void UART_init(uint32_t baud_rate);
void UART_send_char(char data);
void UART_send_string(const char *str);
void UART_send_line(const char *str);
char UART_receive_char(void);

void UART_init(uint32_t baud_rate) {
    uint16_t ubrr_value = (F_CPU / (16UL * baud_rate)) - 1;
    
    UBRR0H = (uint8_t)(ubrr_value >> 8);
    UBRR0L = (uint8_t)(ubrr_value);
    
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Bật UART RX & TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit, 1 stop bit, no parity
}
 void UART_send_char(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Chờ buffer trống
    UDR0 = data;
}

 void UART_send_string(const char *str) {
    while (*str) {
        UART_send_char(*str++);
    }
}

void UART_send_line(const char *str) {
    UART_send_string(str);
    UART_send_char('\n');
}

char UART_receive_char(void) {
    while (!(UCSR0A & (1 << RXC0))); // Chờ dữ liệu
    return UDR0;
}

#endif // UART_H
