

#include "Bouton.h"
#include <avr/interrupt.h> 

Bouton::Bouton() {}

Bouton::~Bouton() {}

void Bouton::front()
{
    cli();
    EICRA &= ~((1 << ISC01) | (1 << ISC00)); 
    EICRA |= (1 << ISC00);                   
    EIMSK |= (1 << INT0);
    sei();
}

void Bouton::frontMontant()
{
    cli();
    EICRA &= ~((1 << ISC01) | (1 << ISC00));
    EICRA |= (1 << ISC01) | (1 << ISC00);    
    EIMSK |= (1 << INT0);
    sei();
}

void Bouton::frontDescendant()
{
    cli();
    EICRA &= ~((1 << ISC01) | (1 << ISC00));
    EICRA |= (1 << ISC01);                  
    EIMSK |= (1 << INT0);
    sei();
}

Bouton::Bouton(bool etatBouton)
{   
    boutonBBactif = etatBouton;
    DDRD &= ~(1 << PD2); 
    DDRD &= ~(1 << PD3); 

    frontMontant();

    cli();

    EICRA |= (1 << ISC10);
    EICRA &= ~(1 << ISC11);

    EIMSK |= (1 << INT1);

    sei();
}

void Bouton::initialiserInterruptionsBoutons()
{
    cli();

    DDRD &= ~(1 << PD2); 
    DDRD &= ~(1 << PD3); 

    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    
    EICRA |= (1 << ISC10);
    EICRA &= ~(1 << ISC11);

    EIMSK |= (1 << INT1);
    EIMSK |= (1 << INT0);

    sei();
}