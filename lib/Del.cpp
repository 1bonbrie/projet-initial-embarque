
#include "Del.h"

Del::Del(volatile uint8_t *portx, 
         volatile uint8_t *ddrx, 
         uint8_t broche_1, 
         uint8_t broche_2): 
        portx(portx), ddrx(ddrx), broche_1(broche_1), broche_2(broche_2)
        {            
            *ddrx |= (1 << broche_1) | (1 << broche_2);
        }




void Del::afficherVert()
{
    *portx |= (1 << broche_1);
    *portx &= ~(1 << broche_2);
}

void Del::afficherRouge()
{
    *portx |= (1 << broche_2);
    *portx &= ~(1 << broche_1);
}

void Del::afficherAmbre(uint16_t duree) {
    uint16_t compteurDelai = 0; 

    while (compteurDelai < duree) 
    {
        *portx |= (1 << broche_1);
        *portx &= ~(1 << broche_2);
        _delay_ms(TEMPS_AMBRE);

        *portx |= (1 << broche_2);
        *portx &= ~(1 << broche_1);
        _delay_ms(TEMPS_AMBRE);

        compteurDelai += 2 * TEMPS_AMBRE; 
    }

    eteindreDel();
}

void Del::eteindreDel()
{
    
    *portx &= ~(1 << broche_1);
    *portx &= ~(1 << broche_2);
}


void Del::clignoterDel4Hz(bool& estPresentPilier)
{
    const uint8_t DELAI_CLIGNOTEMENT = 125; 
    const uint8_t NOMBRE_CLIGNOTEMENTS = 8;
    for (uint8_t i = 0 ; i < NOMBRE_CLIGNOTEMENTS ; i++)
    {
        estPresentPilier ? afficherRouge() : afficherVert();
        _delay_ms(DELAI_CLIGNOTEMENT); // 125 ms pour un clignotement à 4 Hz
        eteindreDel();
        _delay_ms(DELAI_CLIGNOTEMENT);
    }
}

void Del::alternerRougeVert2Hz()
{
    const uint16_t DELAI_COULEUR = 250; 
    const uint8_t NOMBRE_ALTERNANCES = 4;

    for (uint8_t i = 0; i < NOMBRE_ALTERNANCES; ++i)
    {
        afficherRouge();
        _delay_ms(DELAI_COULEUR);

        afficherVert();
        _delay_ms(DELAI_COULEUR);
    }

    eteindreDel(); 
}

