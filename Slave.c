/*
 * File:   Slave.c
 * Author: Jorge Ceron
 *
 * Created on 4 de agosto de 2022, 08:33 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h> 
#include <stdio.h>
#include "spi.h"
#include "oscilador.h"
#include "tmr0.h"
#define _XTAL_FREQ 1000000


/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint8_t CONTM = 0;
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() SPI_slave(void){
    if (PIR1bits.SSPIF){
        CONTM = spiRead();          // Se almacena el valor enviado del Maestro
        spiWrite(CONTM << 1);       // Se envía el valor multiplicado *2 al Maestro
        PORTB = CONTM;              // Valor recibido al PORTD del Slave
        PIR1bits.SSPIF = 0;         // Limpiar bandera de interrupcion de SPI
    }
    return;
}

void main(void) {
    setup();
    while(1){    
    }
    return;
}

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){       
    // Configuración de puertos
    ANSEL = 0b00000000;         // I/O digitales
    ANSELH = 0b00000000;        // I/O digitales
    TRISAbits.TRISA5 = 1;       // SS como Input
    TRISB = 0b00000000;         // PORTB como salida  
    PORTB = 0b00000000;         // Limpieza del PORTA
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    // Configuración de interrupciones
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    PIR1bits.SSPIF = 0;         // Limpiar bandera de SPI
    PIE1bits.SSPIE = 1;         // Habilitar interrupciones de SPI
    INTCONbits.PEIE = 1;        // Habilitar interrupciones de perifericos
    return;
}
