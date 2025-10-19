
#pragma once
#include <avr/io.h> 
#include <avr/interrupt.h>

#define DELAI_ANTI_REBOND 30

class Bouton
{
    
public:
    Bouton();
    Bouton(bool etatBouton);
    ~Bouton();
    void front();
    void frontMontant();
    void frontDescendant();
    void initialiserInterruptionsBoutons();

    private:
    bool boutonBBactif;
};