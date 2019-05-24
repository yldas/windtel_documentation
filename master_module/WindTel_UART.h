/*
 * WindTel_UART.h
 *
 *  Created on: Apr 16, 2019
 *      Author: Luis O. Vega Maisonet and Kahlil J. Fonseca
 */
#include "msp.h"
#ifndef WINDTEL_UART_H_
#define WINDTEL_UART_H_

void uartDelay(uint16_t msec){
    uint32_t i = 0;
    uint32_t time = (msec/1000) * (48000000/15);
    for(i = 0; i < time; i++){
        ;
    }
}

void uart_pin_setup(void){
    CS->KEY = CS_KEY_VAL;
    CS->CTL1 |= (uint32_t) 0x00000200;
    CS->CLKEN |= (uint32_t) 0x00008000;

    EUSCI_A2->CTLW0 |= BIT6;                        //UCSSEL0 = 01b = clk src set to ACLK
    EUSCI_A2->CTLW0 |= BIT0;                        //UCSWRST = 1b = software reset enabled
    EUSCI_A2->BRW = (uint16_t) 13;                 //(int) clk/baud rate

    P3->DIR |= BIT2;
    P3->DIR |= BIT3;
    P3SEL0 |= BIT2;
    P3SEL1 &= ~BIT2;
    P3SEL0 |= BIT3;
    P3SEL1 &= ~BIT3;

    EUSCI_A0->CTLW0 |= BIT6;                        //UCSSEL0 = 01b = clk src set to ACLK
    EUSCI_A0->CTLW0 |= BIT0;                        //UCSWRST = 1b = software reset enabled
    EUSCI_A0->BRW = (uint16_t) 13;                 //(int) clk/baud rate

    P1->DIR |= BIT2;
    P1->DIR |= BIT3;
    P1->SEL0 |= BIT2+BIT3;
    P1->SEL1 &= ~(BIT2+BIT3);

    EUSCI_A1->CTLW0 |= BIT6;                        //UCSSEL0 = 01b = clk src set to ACLK
    EUSCI_A1->CTLW0 |= BIT0;                        //UCSWRST = 1b = software reset enabled
    EUSCI_A1->BRW = (uint16_t) 13;                 //(int) clk/baud rate

    P2->DIR |= BIT2;
    P2->DIR |= BIT3;
    P2->SEL0 |= BIT2+BIT3;
    P2->SEL1 &= ~(BIT2+BIT3);
}

int length(char* string)
{
    char *temp;
    int length = 0;
    for (temp = string; *temp != '\0'; temp++)
    {
        length++;
    }
    return length;
}

void transmitMessageToPressureModule(char* message){
    int i;
    for(i = 0; i < length(message); i++){
        EUSCI_A2->CTLW0 |= BIT0;
        while(!(EUSCI_A2->IFG & BIT1));
        EUSCI_A2->IFG &= ~BIT1;
        EUSCI_A2->CTLW0 &= ~BIT0;
        EUSCI_A2->TXBUF = message[i];
        while(!(EUSCI_A2->IFG & BIT3));
        EUSCI_A2->IFG &= ~BIT3;
        EUSCI_A2->CTLW0 &= ~BIT0;
    }
}

void UART2_Receiver_Mode(){
    EUSCI_A2->IE = 0x0001;
    NVIC->ISER[0] |= 1 << ((EUSCIA2_IRQn) & 31);
    __no_operation();
}
void UART2_Transmitter_Mode(){
    EUSCI_A2->IE = 0x0002;
    __no_operation();
}

void UART2_Stop_Mode(){
    EUSCI_A2->IFG = 0;
    EUSCI_A2->IE = 0x0000;
    __no_operation();}

void transmitMessageToDynamicModule(char* message){
    P4->OUT ^= BIT2;
    int i;
    for(i = 0; i < length(message); i++){
        EUSCI_A0->CTLW0 |= BIT0;
        while(!(EUSCI_A0->IFG & BIT1));
        EUSCI_A0->IFG &= ~BIT1;
        EUSCI_A0->CTLW0 &= ~BIT0;
        EUSCI_A0->TXBUF = message[i];
        while(!(EUSCI_A0->IFG & BIT3));
        EUSCI_A0->IFG &= ~BIT3;
        EUSCI_A0->CTLW0 &= ~BIT0;
    }
}

void transmitMessageToBalanceModule(char* message){
    int i;
    for(i = 0; i < length(message); i++){
        EUSCI_A1->CTLW0 |= BIT0;
        while(!(EUSCI_A1->IFG & BIT1));
        EUSCI_A1->IFG &= ~BIT1;
        EUSCI_A1->CTLW0 &= ~BIT0;
        EUSCI_A1->TXBUF = message[i];
        while(!(EUSCI_A1->IFG & BIT3));
        EUSCI_A1->IFG &= ~BIT3;
        EUSCI_A1->CTLW0 &= ~BIT0;
    }
}

void UART0_Receiver_Mode(){
    EUSCI_A0->IE = 0x0001;
    NVIC->ISER[0] |= 1 << ((EUSCIA0_IRQn) & 31);
    __no_operation();
}
void UART0_Transmitter_Mode(){
    EUSCI_A0->IE = 0x0002;
    __no_operation();
}
void UART0_Stop_Mode(){
    EUSCI_A0->IFG = 0;
    EUSCI_A0->IE = 0x0000;
    __no_operation();
}

void UART1_Receiver_Mode(){
    EUSCI_A1->IE = 0x0001;
    NVIC->ISER[0] |= 1 << ((EUSCIA1_IRQn) & 31);
    __no_operation();
}
void UART1_Transmitter_Mode(){
    EUSCI_A1->IE = 0x0002;
    __no_operation();
}
void UART1_Stop_Mode(){
    EUSCI_A1->IFG = 0;
    EUSCI_A1->IE = 0x0000;
    __no_operation();
}
#endif /* WINDTEL_UART_H_ */
