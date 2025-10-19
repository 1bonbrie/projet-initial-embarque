
#define F_CPU 8000000UL

#pragma once
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CapteurDistance.h"

#define DELAI_AVANCER 1400
#define DELAI_ROTATION_90_DROITE 1000
#define DELAI_ROTATION_90_GAUCHE 1500
#define VITESSE_MINIMALE 0
#define VITESSE_MAXIMALE 255

#define VITESSE_ROTATION_GAUCHE 90
#define VITESSE_ROTATION_DROITE 80

#define VITESSE_DEFAUT 40
#define DELAI_ARRET_MOTEUR 400


class Moteur 
{       
  public:

    //Constructeur objet robot
    Moteur();

    void avancer(uint8_t pourcentageVitesse);

    void reculer(uint8_t pourcentageVitesse);
    void definirDirectionDroite();
    void definirDirectionGauche(); 

    void tournerDroite90(bool aBesoinAvancer); 
    void tournerDroite90();

    void prendreCheminAlternatifG();

    void tournerGauche90(bool aBesoinAvancer); 
    void tournerGauche90();

    void tournerDroitePrecis(bool aBesoinAvancer);
    void tournerGauchePrecis(bool aBesoinAvancer);
    bool tournerDroite90Distance(); 


    //Métode arrêtant les moteurs du robot
    void arreterMoteurs();

    void corrigerTrajectoire(uint8_t vitesseRoueD, uint8_t vitesseRoueG);
};