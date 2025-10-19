
#include "timer1.h"

Timer1::Timer1(uint16_t valeurOCR1) : valeurOCR1_(valeurOCR1) {}

void Timer1::partirMinuterie()
{
    cli();  // Désactive les interruptions globales

    TCNT1 = 0;  // Réinitialise le compteur du Timer 1

    OCR1A = valeurOCR1_;  // Charge la valeur du registre de comparaison

    TCCR1A = 0;  // Mode normal (pas de PWM ou autres modes avancés)

    // Configure le prescaler à 1024 et active le mode CTC (Clear Timer on Compare Match)
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);

    TIMSK1 = (1 << OCIE1A);  // Active l'interruption pour OCR1A

    DDRD |= (1 << PD6);  // Active la broche PD6 comme sortie (OC1A)

    sei();  // Active les interruptions globales
}

void Timer1::eteindreMinuterie()
{
    cli();  // Désactive les interruptions globales

    TCNT1 = 0;  // Réinitialise le compteur du Timer 1

    OCR1A = 0;  // Désactive le comparateur

    TCCR1A = 0;  // Réinitialise les registres de configuration

    TCCR1B = 0;  // Désactive le Timer 1

    TIMSK1 = 0;  // Désactive les interruptions du Timer 1

    DDRD &= ~(1 << PD6);  // Désactive la broche PD6 (OC1A)

    sei();  // Active les interruptions globales
}