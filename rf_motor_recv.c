#include "rf_motor_common.h"
#include <avr/io.h>
#include <stdio.h>
#include <sys/types.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/interrupt.h>

void uart_init(unsigned int baudrate) {
    unsigned int ubrr = F_CPU/16/baudrate-1;
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void uart_transmit(unsigned char data) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

void uart_print(const char *str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

ISR(TIMER1_COMPA_vect) {

}

uint8_t recv_byte() {
    uint8_t byte = 0x00;
    while (!(PIND & (1 << PD4))) {}
    for (int i = 0; i < 8; i++) {
        uint16_t high_len = 0;
        while ((PIND & (1 << PD4))) {
            _delay_us(10);
            high_len += 10;
        }

        if (!_acceptable(high_len, HIGH_LEN)) {
            return NOISE_BYTE_ERROR;
        }

        uint16_t low_len = 0;
        while(!(PIND & (1 << PD4))) {
            _delay_us(10);
            low_len += 10;
        }

        if (_acceptable((high_len+low_len), ONE_LEN_US)) {
            byte |= (1 << i);
        } else if (_acceptable((high_len+low_len), ZERO_LEN_US)) {
            byte &= ~(1 << i);
        } else { // noise 
            return NOISE_BYTE_ERROR;
        }
    }   
    return byte;
}


int main(void) {
    uart_init(1200);
    DDRD |= (1 << PD7); // 7 out
    DDRD &= ~(1 << PD4); // 4 in

    for (;;) {
        uint8_t byte = recv_byte();
        if (byte != NOISE_BYTE_ERROR) {
            char buf[18] = {0};
            sprintf(buf, "received: %hhu", byte); 
            uart_print(buf);
        }
    }
}
