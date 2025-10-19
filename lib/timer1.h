

#pragma once

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Del.h"
#include "Moteur.h"

class Timer1
{
public:
    // Constructeur prenant une valeur OCR1A
    Timer1(uint16_t valeurOCR1);

    // Démarre la minuterie
    void partirMinuterie();

    // Éteint la minuterie
    static void eteindreMinuterie();

private:
    uint16_t valeurOCR1_;  // Valeur à charger dans OCR1A (16 bits pour Timer 1)
};