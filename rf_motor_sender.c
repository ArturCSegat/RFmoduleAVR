#include "rf_motor_common.h"
#include <avr/io.h>
#include <sys/types.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

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

void write_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        PORTD |= (1<<PD7);
        _delay_us(HIGH_LEN);

        PORTD &= ~(1 << PD7);
        if ((byte & (1 << i)) != 0) {
            _delay_us(ONE_LEN_US - HIGH_LEN);
        } else {
            _delay_us(ZERO_LEN_US - HIGH_LEN);
        }
    }
}

int main(void) {
    DDRD |= (1 << PD7); // 7 out
    DDRD &= ~(1 << PD4); // 4 in

    uart_init(1200);
    while(1) {
        if ((PIND & (1 << PD4)) != 0) {
            for (int i =0; i < TRANS_PREAMBLE_BYTES; i++) {
                write_byte(0);
            }
            
            for (int i = 0; i < 5; i++) {
                write_byte(69);
                _delay_us(BYTE_INTERVAL);
            }

            uart_print("done\n");
            _delay_ms(1000);
        } else {
            PORTD &= ~(1 << PD7);
            _delay_ms(1);
        }
    }
}

