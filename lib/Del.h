

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#pragma once
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>



class Del
{
public:
    Del(volatile uint8_t *portx, volatile uint8_t *ddrx, uint8_t broche_1, uint8_t broche_2);
    ~Del() {}  

    void afficherVert();
    void afficherRouge();
    void afficherAmbre(uint16_t duree);
    void eteindreDel();
    void clignoterDel4Hz(bool& estPresentPilier);
    void alternerRougeVert2Hz();


private:
    static const uint8_t TEMPS_AMBRE = 10; 
    volatile uint8_t *portx;
    volatile uint8_t *ddrx;
    const uint8_t broche_1;
    const uint8_t broche_2;
};
