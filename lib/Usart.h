

#pragma once
#include <avr/io.h>

class Usart 
{
    
public:

    Usart();

    void transmissionUsart(uint8_t donnee);

    uint8_t reception();

    void transmissionDeChaine(char chaine[], uint8_t longueur);

};