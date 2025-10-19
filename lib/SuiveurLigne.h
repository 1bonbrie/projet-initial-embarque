


#define F_CPU 8000000UL

#pragma once
#include "LineMaker.h"
#include "Moteur.h"
#include "Usart.h"

enum class Direction
{
    Aucune,
    Avancer,
    Gauche,
    LegerementGauche,
    Gauche90,
    Droite,
    LegerementDroite,
    Droite90,    
    Arreter
};

class SuiveurLigne 
{       
  public:

    SuiveurLigne();
    void suivreLigne(bool ignorerIntersection = false, 
                     bool aBesoinAvancerIntersection = true);

    void suivreLigneGrille();


    Direction obtenirDirection();

   LineMaker capteur;
   Moteur moteurs;
};