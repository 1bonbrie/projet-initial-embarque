
#include "Usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug.h"

Usart usart = Usart();

void afficher(char* donnee) 
{


    while(*donnee)
    {
        usart.transmissionUsart(*donnee++);
    }

}