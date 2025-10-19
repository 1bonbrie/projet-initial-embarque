

#include "Moteur.h"
#include "LineMaker.h"
#include "SuiveurLigne.h"

Moteur::Moteur()
{

    //Méthode stoppant toutes les interruptions pendant la configuration du moteur
    cli();

    //Définition des broches PD4 PD5 en sortie pour les signaux PWM
    DDRD    |= (1<< PD4 ) | (1 << PD5);  
    //Définition des broches PD6 et PD7 comme sortie pour la direction des moteurs
    DDRD    |= (1<< PD6 ) | (1 << PD7);  

    // Configure Timer/Counter 2 en mode phase Correct 8-bit 
    // active la sortie OC2A et OC2B en mode de comparaison pour générer des signaux PWM
    TCCR2A  |= (1<<WGM10) | (1<< COM1A1) | (1<<COM1B1);

    // Configure le prescaler du Timer 2 à 1024 
    // (CS12 = 1 et CS10 = 1) pour définir la fréquence du timer
    TCCR2B  |= (1<<CS12) | (1<<CS10);


    //Méthode réactivant les interruptions
    sei();
}

uint8_t conversionPourcentageEnPWM(uint8_t pourcentageVitesse)
{
    return (pourcentageVitesse * VITESSE_MAXIMALE) / 100;
}

void Moteur::avancer(uint8_t pourcentageVitesse)
{

    uint8_t cyclePwm = conversionPourcentageEnPWM(pourcentageVitesse);
    if (cyclePwm > VITESSE_MAXIMALE || cyclePwm < VITESSE_MINIMALE)
    {
        return;
    }

    PORTD &= ~((1 << PD4) | (1 << PD5));

    OCR2A = cyclePwm;
    OCR2B = cyclePwm;
}

void Moteur::reculer(uint8_t pourcentageVitesse)
{
    uint8_t cyclePwm = conversionPourcentageEnPWM(pourcentageVitesse);
    if (cyclePwm > VITESSE_MAXIMALE || cyclePwm < VITESSE_MINIMALE)
    {
        return;
    }

    PORTD |= (1 << PD4) |(1 << PD5);

    OCR2A = cyclePwm;
    OCR2B = cyclePwm;        
}

void Moteur::definirDirectionDroite()
{
    PORTD &= ~(1 << PD4);
    PORTD |= (1 << PD5);
}

void Moteur::definirDirectionGauche()
{
    PORTD &= ~(1 << PD5);
    PORTD |= (1 << PD4);
}

void Moteur::arreterMoteurs()
{
    OCR2A = VITESSE_MINIMALE;
    OCR2B = VITESSE_MINIMALE;

}

void Moteur::corrigerTrajectoire(uint8_t vitesseRoueG, uint8_t vitesseRoueD)
{
    uint8_t cyclePwmDroit = conversionPourcentageEnPWM(vitesseRoueG);
    uint8_t cyclePwmGauche = conversionPourcentageEnPWM(vitesseRoueD);

    PORTD &= ~((1 << PD4) | (1 << PD5));
    OCR2B = cyclePwmDroit;       //roue droite
    OCR2A = cyclePwmGauche;      //roue gauche
}

void Moteur::tournerDroite90()
{
    definirDirectionDroite();
    OCR2B = VITESSE_ROTATION_DROITE;
    OCR2A = VITESSE_ROTATION_DROITE;
    _delay_ms(DELAI_ROTATION_90_DROITE);
}

void Moteur::tournerGauche90()
{
    definirDirectionGauche();
    OCR2B = VITESSE_ROTATION_GAUCHE;
    OCR2A = VITESSE_ROTATION_GAUCHE;
    _delay_ms(DELAI_ROTATION_90_GAUCHE);
}

void Moteur::tournerDroite90(bool aBesoinAvancer)
{
    if (aBesoinAvancer)
    {
        avancer(VITESSE_DEFAUT);
        _delay_ms(DELAI_AVANCER);
    }

    
    definirDirectionDroite();
    OCR2B = VITESSE_ROTATION_DROITE;
    OCR2A = VITESSE_ROTATION_DROITE;

    LineMaker cLigne;
    while (true)
    {
        cLigne.assignerValeurs();
        if (cLigne.obtenirValeurs()[S1] == 0 && cLigne.obtenirValeurs()[S2] == 0 && cLigne.obtenirValeurs()[S3] == 1
            && cLigne.obtenirValeurs()[S4] == 1 && cLigne.obtenirValeurs()[S5] == 0)
        {
            arreterMoteurs();
            _delay_ms(DELAI_ARRET_MOTEUR);
            break;
        }   
    }   

}

void Moteur::tournerGauche90(bool aBesoinAvancer)
{
    
    if (aBesoinAvancer)
    {
        avancer(VITESSE_DEFAUT);
        _delay_ms(DELAI_AVANCER);
    }

    definirDirectionGauche();
    OCR2B = VITESSE_ROTATION_GAUCHE;
    OCR2A = VITESSE_ROTATION_GAUCHE;
    
    LineMaker cLigne;
    while (true)
    {
        cLigne.assignerValeurs();
        if (cLigne.obtenirValeurs()[S1] == 1 && cLigne.obtenirValeurs()[S2] == 1 && cLigne.obtenirValeurs()[S3] == 0
            && cLigne.obtenirValeurs()[S4] == 0 && cLigne.obtenirValeurs()[S5] == 0)
        {
            arreterMoteurs();
            _delay_ms(DELAI_ARRET_MOTEUR);
            break;
        }   
    }    
}


void Moteur::prendreCheminAlternatifG()
{
    tournerDroite90();

    LineMaker cLigne;

    definirDirectionDroite();
    OCR2B = VITESSE_ROTATION_GAUCHE;
    OCR2A = VITESSE_ROTATION_GAUCHE;

    while (true)
    {
        cLigne.assignerValeurs();
        if (cLigne.obtenirValeurs()[S1] == 0 && cLigne.obtenirValeurs()[S2] == 0 && cLigne.obtenirValeurs()[S3] == 0
            && cLigne.obtenirValeurs()[S4] == 1 && cLigne.obtenirValeurs()[S5] == 1)
        {
            arreterMoteurs();
            _delay_ms(DELAI_ARRET_MOTEUR);
            break;
        }   
    }      
}



void Moteur::tournerDroitePrecis(bool aBesoinAvancer)
{
    const uint8_t vitesseRotationLente = 75;

    if (aBesoinAvancer)
    {
        avancer(VITESSE_DEFAUT);    
        _delay_ms(DELAI_AVANCER);
    }

    bool buffer = false;
    LineMaker cLigne;
    
    definirDirectionDroite();
    OCR2B = vitesseRotationLente;
    OCR2A = vitesseRotationLente;

    while (true)
    {
        cLigne.assignerValeurs();
        if (cLigne.obtenirValeurs()[S1] == 0 && cLigne.obtenirValeurs()[S2] == 0 && cLigne.obtenirValeurs()[S3] == 1
            && cLigne.obtenirValeurs()[S4] == 0 && cLigne.obtenirValeurs()[S5] == 0 && buffer)
        {
            arreterMoteurs();
            _delay_ms(DELAI_ARRET_MOTEUR);
            break;
        }
        if(cLigne.obtenirValeurs()[S1] == 0 && cLigne.obtenirValeurs()[S2] == 0 && cLigne.obtenirValeurs()[S3] == 0
            && cLigne.obtenirValeurs()[S4] == 0 && cLigne.obtenirValeurs()[S5] == 0)
            buffer = true;
    }

}

void Moteur::tournerGauchePrecis(bool aBesoinAvancer)
{
    const uint8_t vitesseRougeGauche = 37;    // pour ajustememnt materiel
    const uint8_t vitesseRotationLente = 80;

    bool buffer = false;

    if (aBesoinAvancer)
    {
        corrigerTrajectoire(vitesseRougeGauche,VITESSE_DEFAUT);
        _delay_ms(DELAI_AVANCER);
    }

    
    definirDirectionGauche();
    OCR2B = vitesseRotationLente;
    OCR2A = vitesseRotationLente;

    LineMaker cLigne;
    while (true)
    {
        cLigne.assignerValeurs();
        if (cLigne.obtenirValeurs()[S1] == 0 && cLigne.obtenirValeurs()[S2] == 0 && cLigne.obtenirValeurs()[S3] == 1
            && cLigne.obtenirValeurs()[S4] == 1 && cLigne.obtenirValeurs()[S5] == 0 && buffer)
        {
            arreterMoteurs();
            _delay_ms(DELAI_ARRET_MOTEUR);
            break;
        }
        if(cLigne.obtenirValeurs()[S1] == 0 && cLigne.obtenirValeurs()[S2] == 0 && cLigne.obtenirValeurs()[S3] == 0
            && cLigne.obtenirValeurs()[S4] == 0 && cLigne.obtenirValeurs()[S5] == 0)
            buffer = true;
    }
}


bool Moteur::tournerDroite90Distance() 
{
    bool estPresentPilier = false;
    bool buffer = false;
    const uint8_t vitesseRotationLente = 85;

    definirDirectionDroite();
    OCR2B = vitesseRotationLente;
    OCR2A = vitesseRotationLente;

    CapteurDistance cDistance(PA6);
    LineMaker cLigne;

    while (true)
    {
        cLigne.assignerValeurs();
        if(!cDistance.verifierPilier(30) && cLigne.obtenirValeurs()[S1] == 0 && cLigne.obtenirValeurs()[S2] == 0 && cLigne.obtenirValeurs()[S3] == 0
        && cLigne.obtenirValeurs()[S4] == 0 && cLigne.obtenirValeurs()[S5] == 0)
            buffer = true;
        if(cDistance.verifierPilier(30) && buffer)
            estPresentPilier = true;
        if (cLigne.obtenirValeurs()[S4] == 1 && cLigne.obtenirValeurs()[S5] == 1 && buffer)
        {
            break;
        }
             
    } 
    return estPresentPilier;
}

