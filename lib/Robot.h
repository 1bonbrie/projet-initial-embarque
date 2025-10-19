

#ifndef F_PU
#define F_CPU 8000000UL
#endif

#pragma once
#include "Del.h"
#include "Son.h"
#include "Bouton.h"
#include "Moteur.h"
#include "SuiveurLigne.h"
#include "Parcours.h"


#define C_LIGNE suiveurLigne.capteur.obtenirValeurs()

class Robot
{
public:


    Robot(volatile uint8_t* portDel, volatile uint8_t* ddrDel, uint8_t brocheDel_1, uint8_t brocheDel_2);
    ~Robot();

    bool verifierPilier(uint16_t distancePilier = 15);

    void afficherVert();
    void afficherRouge();
    void afficherAmbre(uint16_t duree);
    void eteindreDel();
    void clignoterDel4Hz(bool estPresentPilier);
    void alternerRougeVert2Hz();

    void avancer(uint8_t vitesse);
    void reculer(uint8_t vitesse);
    void arreterMoteurs();
    void tournerDroite90(bool aBesoinAvancer = false); 
    void tournerGauche90(bool aBesoinAvancer = false); 
    void corrigerTrajectoire(uint8_t vitesseRoueD, uint8_t vitesseRoueG);
    void prendreCheminAlternatifG();
    bool tournerDroite90Distance();
    void tournerGauchePrecis(bool aBesoinAvancer);
    void tournerDroitePrecis(bool aBesoinAvancer); 


    void emettreSon(double frequence);
    void emettreNote(uint8_t note);
    void emettreNotePendant(uint8_t note, uint8_t temps);
    void arreterSon();

  
    void configurerFront();
    void configurerFrontMontant();
    void configurerFrontDescendant();
    void initialiserInterruptionsBoutons();

    //classe suiveurLigne
    void suivreLigne(bool ignorerIntersection, 
                      bool aBesoinAvancerIntersection);
    void suivreLigneGrille();
    Direction obtenirDirection();
    uint8_t* obtenirValeursCapLigne();

    //Méthodes de parcours
    void effectuerParcoursBoucle();
    void effectuerSelectionDirection(volatile uint8_t choixDirection[2]);
    void effectuerParcoursGrille();
private:
    Del del;
    Bouton bouton;
    Son son;
    Moteur moteur;
    SuiveurLigne suiveurLigne;
    CapteurDistance capteur;


};