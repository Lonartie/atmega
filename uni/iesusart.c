/**
 * @file iesusart.c
 * @brief Basic serial communication via USART for ATMEGA328
 * @version 0.1
 * @date 2021-06-08
 */

#include "iesusart.h"
#include <avr/io.h>

/**
 * @brief 
 * Reads a single byte out of the USART receive buffer.
 * A good way to use this would be calling it from an receive interrupt service routine
 * @return unsigned char (received byte)
 */
unsigned char USART_receiveByte(void)
{
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0))) {
        // Busy waiting! zzzZZZzzzZZZzzz
    }    
    // Return received data from buffer
    return UDR0;
}

/**
 * @brief Writes a single byte to the USART transmit buffer
 * @param data Byte that shall be transmitted
 */
void USART_transmitByte(unsigned char data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0))) {
        // Busy waiting! zzzZZZzzzZZZzzz
    }
    // Put data into buffer, send the data
    UDR0 = data;
}

/**
 * @brief Transmittes a string (char by char) until '\0’ is reached
 */
void USART_print(const char *c)
{
    while (*c != '\0') {
        USART_transmitByte(*c);
        c++;
    }
}

/**
 * @brief Sets up the USART port (The USART baudrate register)
 * @param ubrr Content to write into the UBRR register
 */
void USART_init(unsigned long ubrr)
{
    // Set baud rate, high byte first
    UBRR0H = (unsigned char) (ubrr >> 8);
    // Set baud rate, low byte second
    UBRR0L = (unsigned char) ubrr;
    // Enable receiver/transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Frame format: 8 data, 2 stop bits
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    /* Transmit something right after initialization to overcome the lagg at the
     * start of a simulation in SimulIDE.
    */
   USART_print("<(^_^)>\n\0");
}