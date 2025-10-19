
#include "SuiveurLigne.h"

SuiveurLigne::SuiveurLigne(){};



void SuiveurLigne::suivreLigne(bool ignorerIntersection, bool aBesoinAvancerIntersection)
{
        //les valeurs sont différentes pour tenir en compte du matériel
        const uint8_t vitesseSuiveurDefaut    = 36;        
        const uint8_t vitesseCorrection       = 15;           
        const uint8_t vitesseCorrectionLegere = 32;
        const uint8_t vitesseDefautCorrection = 39;

        capteur.assignerValeurs();

            switch (obtenirDirection())
            {
            case Direction::Avancer:
                moteurs.avancer(vitesseSuiveurDefaut);
                break;
            case Direction::Arreter:
                moteurs.arreterMoteurs();
                break;
          case Direction::Gauche:
                moteurs.corrigerTrajectoire(vitesseCorrection,vitesseSuiveurDefaut);
                break;
            case Direction::Droite:
                moteurs.corrigerTrajectoire(vitesseSuiveurDefaut,vitesseCorrection);
                break;
            case Direction::LegerementGauche:
                moteurs.corrigerTrajectoire(vitesseCorrectionLegere,vitesseDefautCorrection);
                break;
            case Direction::LegerementDroite:
                moteurs.corrigerTrajectoire(vitesseDefautCorrection,vitesseCorrectionLegere);
                break;
            case Direction::Droite90:
                if (ignorerIntersection)
                {
                    moteurs.avancer(vitesseSuiveurDefaut);
                }
                else
                {
                    moteurs.tournerDroite90(aBesoinAvancerIntersection);
                }
                break;
            case Direction::Gauche90:
                if (ignorerIntersection)
                {
                    moteurs.avancer(vitesseSuiveurDefaut);
                }
                else
                {
                    moteurs.tournerGauche90(aBesoinAvancerIntersection);

                }
               break;
            case Direction::Aucune:
            default:
                break;
            }
}


Direction SuiveurLigne::obtenirDirection()
{
    Direction direction = Direction::Aucune;

    int poids[5] = {-2, -1, 0, 1, 2};
    int somme = 0;
    
    for (uint8_t i = 0; i < 5; i++) 
    {
        if (capteur.obtenirValeurs()[i] == 1) 
        {  
            somme += poids[i];
        }
    }


    if (somme == 0 && capteur.obtenirValeurs()[S3] == 1)         //Quand toutes les LED sont allumées   
    {
        direction = Direction::Avancer;
    }
    else if (somme == 0 && capteur.obtenirValeurs()[S3] == 0)    //Quand toutes les LED sont éteintes
    {
        direction = Direction::Arreter;
    }
    else if (capteur.obtenirValeurs()[S5] == 1 && capteur.obtenirValeurs()[S4] == 1 
             && capteur.obtenirValeurs()[S3] == 1 && capteur.obtenirValeurs()[S2] 
             == 1 && capteur.obtenirValeurs()[S1] == 0)
    {
        direction = Direction::Droite90;
    }
    else if (capteur.obtenirValeurs()[S5] == 0 && capteur.obtenirValeurs()[S4] == 1 
             && capteur.obtenirValeurs()[S3] == 1 && capteur.obtenirValeurs()[S2] == 1 
             && capteur.obtenirValeurs()[S1] == 1)
    {
        direction = Direction::Gauche90;
    }
    else if (somme == -1)
    {
        direction = Direction::LegerementGauche;
    }
    else if (somme == 1)
    {
        direction = Direction::LegerementDroite;
    }
        else if (somme <= -2)
    {
        direction = Direction::Gauche;
    }    else if (somme >= 2)
    {
        direction = Direction::Droite;
    }

    return direction;
}




void SuiveurLigne::suivreLigneGrille()
{

    capteur.assignerValeurs();
    const uint8_t vitesseCorrection = 30;
    const uint8_t vitesseCorrectionLente = 20;
    switch (obtenirDirection())
    {
        case Direction::Avancer:
            moteurs.avancer(VITESSE_DEFAUT);
            break;
        case Direction::Gauche:
            moteurs.corrigerTrajectoire(vitesseCorrectionLente,VITESSE_DEFAUT);
            break;
        case Direction::Droite:
            moteurs.corrigerTrajectoire(VITESSE_DEFAUT,vitesseCorrectionLente);
            break;
        case Direction::LegerementGauche:
            moteurs.corrigerTrajectoire(vitesseCorrection,VITESSE_DEFAUT);
            break;
        case Direction::LegerementDroite:
            moteurs.corrigerTrajectoire(VITESSE_DEFAUT,vitesseCorrection);
            break;
        default:
            break;

    }
}