
#pragma once

#ifndef F_CPU 
#define F_CPU 8000000UL
#endif


#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "can.h"
#include "Usart.h"

class CapteurDistance
{

    public:

    //Constructeur et destructeur
    CapteurDistance(uint8_t portLecture = PA6);

    ~CapteurDistance();

    //Méthode lireDistance, retourne la distance,
    //plus la valeur est grande, plus l'objet est proche
    uint16_t lireDistance();

    //retourne vrai si le capteur détecte un pilier
    bool verifierPilier(uint16_t distancePilier = 15);

    //Méthode transmettreDistanceUart, transmet la distance
    //par le port UART
    void transmettreDistanceUart();

    private:
    uint8_t portLecture;
};