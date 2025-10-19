

#ifndef F_CPU 
#define F_CPU 8000000UL
#endif

#include "LineMaker.h"
#include <avr/io.h>
#include <util/delay.h> 

LineMaker::LineMaker(){
    DDRA &= ~(1 << PA0) & ~(1 << PA1) & ~(1 << PA2)
    & ~(1 << PA3) & ~(1 << PA4);
    assignerValeurs();
}
uint8_t* LineMaker::obtenirValeurs()
{
    return valeurs;
}
void LineMaker::assignerValeurs() {
    for(uint8_t i = 0 ; i < 5 ; i++)
    {
        valeurs[i] = (PINA & (1 << PORT[i])) ? 1 : 0;
    }
}
