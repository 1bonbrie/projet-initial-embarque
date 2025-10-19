
#include "Robot.h"

Robot::Robot(volatile uint8_t* portDel, volatile uint8_t* ddrDel, uint8_t brocheDel_1, uint8_t brocheDel_2)
    : del(portDel, ddrDel, brocheDel_1, brocheDel_2)
{
    arreterSon();
}

Robot::~Robot()
{
    arreterMoteurs();
    arreterSon();
    eteindreDel();
}


//Méthodes de Capteur de distance
bool Robot::verifierPilier(uint16_t distancePilier) {
    return capteur.verifierPilier(distancePilier);
}

//Méthodes de Del
void Robot::afficherVert() 
{
    del.afficherVert();
}

void Robot::afficherRouge() 
{
    del.afficherRouge();
}

void Robot::afficherAmbre(uint16_t duree) 
{
    del.afficherAmbre(duree);
}

void Robot::eteindreDel() 
{
    del.eteindreDel();
}

void Robot::clignoterDel4Hz(bool estPresentPilier) 
{
    del.clignoterDel4Hz(estPresentPilier);
}

void Robot::alternerRougeVert2Hz() 
{
    del.alternerRougeVert2Hz();
}

//Méthodes de Moteur
void Robot::avancer(uint8_t vitesse) 
{
    moteur.avancer(vitesse);
}

void Robot::reculer(uint8_t vitesse) 
{
    moteur.reculer(vitesse);
}

void Robot::arreterMoteurs() 
{
    moteur.arreterMoteurs();
}

void Robot::tournerDroite90(bool aBesoinAvancer) 
{
    moteur.tournerDroite90(aBesoinAvancer);
}

void Robot::tournerGauche90(bool aBesoinAvancer) 
{
    moteur.tournerGauche90(aBesoinAvancer);
}

void Robot::prendreCheminAlternatifG()
{
    moteur.prendreCheminAlternatifG();
}

bool Robot::tournerDroite90Distance() 
{
    return moteur.tournerDroite90Distance();
}

void Robot::tournerGauchePrecis(bool aBesoinAvancer) 
{
    moteur.tournerGauchePrecis(aBesoinAvancer);
}

void Robot::tournerDroitePrecis(bool aBesoinAvancer) 
{
    moteur.tournerDroitePrecis(aBesoinAvancer);
}

//Méthodes de Son
void Robot::emettreSon(double frequence) 
{
    son.emettreSon(frequence);
}

void Robot::emettreNote(uint8_t note) 
{
    son.emettreNote(note);
}

void Robot::emettreNotePendant(uint8_t note, uint8_t temps) 
{
    son.emettreNotePendant(note, temps);
}

void Robot::arreterSon() 
{
    son.arreterSon();
}

//Méthodes de Bouton
void Robot::configurerFront() 
{
    bouton.front();
}

void Robot::configurerFrontMontant() 
{
    bouton.frontMontant();
}

void Robot::configurerFrontDescendant() 
{
    bouton.frontDescendant();
}

void Robot::initialiserInterruptionsBoutons()
{
    bouton.initialiserInterruptionsBoutons();
}

//Méthodes de SuiveurLigne
void Robot::suivreLigne(bool ignorerIntersection, bool aBesoinAvancerIntersection)
{
    suiveurLigne.suivreLigne(ignorerIntersection, aBesoinAvancerIntersection);
}

void Robot::suivreLigneGrille()
{
    suiveurLigne.suivreLigneGrille();
}

Direction Robot::obtenirDirection()
{
    return suiveurLigne.obtenirDirection();
}

uint8_t* Robot::obtenirValeursCapLigne()
{
    return suiveurLigne.capteur.obtenirValeurs();
}


//Méthodes pour effectuer les différents parcours
void Robot::effectuerParcoursBoucle()
{

    const uint16_t delaiTransitionEtat = 500;
    const uint8_t distancePilier = 60;

    EtapesParcoursBoucle etape = EtapesParcoursBoucle::INIT;
    while (true)
    {
        switch (etape)
        {
            case EtapesParcoursBoucle::INIT: 
            {
                while (true)
                {
                    suivreLigne(true, false);
                    if (obtenirDirection() == Direction::Droite90)
                    {
                        _delay_ms(delaiTransitionEtat);
                        etape = EtapesParcoursBoucle::E_0;
                        break;
                    }              
                }
                break;
            }
            

            case EtapesParcoursBoucle::E_0: 
            {

                while (true)
                {
                    suivreLigne(true, false);    
                    if (obtenirDirection() == Direction::Droite90)
                    {
                        tournerDroite90(true);
                        etape = EtapesParcoursBoucle::F_0;
                        break;
                    }
                }
                break;
            }
            
            case EtapesParcoursBoucle::F_0: 
            {
                while (true)
                {
                    suivreLigne(true, false);        
                    if (obtenirDirection() == Direction::Droite90)
                    {
                        etape = EtapesParcoursBoucle::G;
                        break;
                    }
                }
                break;
            }

            case EtapesParcoursBoucle::G: 
            {
                tournerDroite90(true);
                _delay_ms(delaiTransitionEtat);
                bool estPresentPilier = capteur.verifierPilier(distancePilier);
                _delay_ms(delaiTransitionEtat);
                if (estPresentPilier)
                {
                    clignoterDel4Hz(estPresentPilier);
                    prendreCheminAlternatifG();
                    etape = EtapesParcoursBoucle::GversI;

                }
                else
                {
                    clignoterDel4Hz(estPresentPilier);
                    _delay_ms(delaiTransitionEtat);
                    etape = EtapesParcoursBoucle::GversH;
                }
                break;
            }

            case EtapesParcoursBoucle::GversH: 
            {
                while (true)
                {
                    suivreLigne(false,true);
                    if (obtenirDirection() == Direction::Droite90)
                    {
                        etape = EtapesParcoursBoucle::H;
                        break;
                    }
                }
                break;
            }
            
            case EtapesParcoursBoucle::GversI: 
            {
                while (true)
                {
                    suivreLigne(true, false);
                    if (obtenirDirection() == Direction::Droite90 
                        ||obtenirDirection() == Direction::Gauche90)
                    {
                        tournerDroite90(true);
                        etape = EtapesParcoursBoucle::I;
                        break;
                    }
                }
                break;
            }

            case EtapesParcoursBoucle::H: 
            {
                while (true)
                {
                    suivreLigne(true, false);
                    if (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0)
                    {
                        _delay_ms(delaiTransitionEtat);
                        etape = EtapesParcoursBoucle::I;
                        break;
                    }
                }
                break;
            }     
                   
            case EtapesParcoursBoucle::I: 
            {
                suivreLigne(true, false);
                if (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1)
                {
                    tournerDroite90(true);
                    etape = EtapesParcoursBoucle::E_1;
                }
                break;
            }     

            case EtapesParcoursBoucle::E_1:
            {
                while (true)
                {
                    suivreLigne(true,false);
                    if (obtenirDirection() == Direction::Droite90)
                    {
                        etape = EtapesParcoursBoucle::F_1;
                        break;
                    }
                }
                break;
            }
            
            case EtapesParcoursBoucle::F_1:
            {
                return;
                break;
            }
            default:
                break;
        }
    }
    return;
}


void Robot::effectuerSelectionDirection(volatile uint8_t choixDirection[2])
{
    const uint16_t dureeSonGrave = 1000;
    const uint16_t delaiTransition = 750;
    const uint8_t frequenceSon = 45;
    uint8_t pointsTraites = 0;

    while (true)
    {
        suiveurLigne.suivreLigne(true, true);

        if (obtenirDirection() == Direction::Arreter)
        {
            arreterMoteurs();
            emettreNote(frequenceSon); 
            _delay_ms(dureeSonGrave);
            arreterSon();
            _delay_ms(dureeSonGrave); // Pause après arrêt

            if (choixDirection[pointsTraites] == 1) {
                avancer(VITESSE_DEFAUT);
                _delay_ms(delaiTransition);
                arreterMoteurs();
                tournerDroite90(false);
            } else {
                avancer(VITESSE_DEFAUT);
                _delay_ms(delaiTransition);
                arreterMoteurs();
                tournerGauche90(false);
            }

            _delay_ms(dureeSonGrave);
            pointsTraites++;
        }
        else if (pointsTraites == 2)
        {
            bool estTermine = true;
            while (estTermine) {
                suiveurLigne.suivreLigne(false, true);
                if (suiveurLigne.obtenirDirection() == Direction::Droite90 
                    || suiveurLigne.obtenirDirection() == Direction::Gauche90)
                {
                    estTermine = false;
                }
            }

            break; 
        }
    }
}


void Robot::effectuerParcoursGrille()
{

    const uint8_t distancePilierAvant = 60;
    const uint8_t distancePilierDiagonale = 70;
    const uint16_t delai1TransitionEtat = 600;
    const uint16_t delai2TransitionEtat = 500;
    const uint16_t dureeAvancerMoteurs = 750;
    const uint16_t delai3TransitionEtat = 200;

    EtapesParcoursGrille etape = EtapesParcoursGrille::choix1;

    while (true)
    {
        switch (etape)
        {
            case EtapesParcoursGrille::choix1:
            {
                avancer(VITESSE_DEFAUT);
                _delay_ms(dureeAvancerMoteurs);
                arreterMoteurs();
                if(!verifierPilier())
                {
                    etape = EtapesParcoursGrille::mil1;
                    break;
                }
                avancer(35);
                _delay_ms(dureeAvancerMoteurs);
                arreterMoteurs();
                while(!verifierPilier(distancePilierAvant))
                {
                    suivreLigneGrille();
                }
                bool droit = tournerDroite90Distance();
                if(!droit)
                {
                    etape = EtapesParcoursGrille::droite1;
                    break;
                }
                tournerGauche90(false);
                _delay_ms(delai3TransitionEtat);
                tournerGauche90(false);
                arreterMoteurs();
                etape = EtapesParcoursGrille::gauche1;
                break;
            }
            case EtapesParcoursGrille::mil1:
            {
                arreterMoteurs();
                clignoterDel4Hz(false);
                while(true)
                {
                    suivreLigneGrille();
                    if(C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                    C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1)
                        break;
                }
                avancer(VITESSE_DEFAUT);
                _delay_ms(delai2TransitionEtat);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if(C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                    C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1)
                        break;
                }
                avancer(VITESSE_DEFAUT);
                _delay_ms(delai2TransitionEtat);
                arreterMoteurs();
                etape = EtapesParcoursGrille::choix2;
                break;
            }
            case EtapesParcoursGrille::droite1:
            {
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 0 && C_LIGNE[S4] == 0 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1) 
                        || (C_LIGNE[S5] == 0 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        { tournerGauchePrecis(true);
                            break;
                        }
                }
                arreterMoteurs();
                clignoterDel4Hz(false);
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 0 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1) || (C_LIGNE[S5] == 0 && C_LIGNE[S4] == 0 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        break;
                }
                avancer(VITESSE_DEFAUT);
                _delay_ms(dureeAvancerMoteurs);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 0 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1) 
                        || (C_LIGNE[S5] == 0 && C_LIGNE[S4] == 0 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        break;
                }
                arreterMoteurs();  
                tournerGauchePrecis(true);
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        break;
                }
                arreterMoteurs();  
                tournerDroitePrecis(true);
                _delay_ms(delai2TransitionEtat);            
                etape = EtapesParcoursGrille::choix2;
                break;
            }
            case EtapesParcoursGrille::gauche1:
            {
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0) 
                        || (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 0))
                        { 
                            tournerDroitePrecis(true);
                            break;
                        }
                }
                arreterMoteurs();
                clignoterDel4Hz(false);
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0) 
                        || (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 0))
                        break;
                }
                avancer(VITESSE_DEFAUT);
                _delay_ms(dureeAvancerMoteurs);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0) 
                        || (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 0))
                        break;
                }
                arreterMoteurs();  
                tournerDroitePrecis(true);
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        break;
                }
                arreterMoteurs();  
                tournerGauchePrecis(true);
                _delay_ms(delai2TransitionEtat);           
                etape = EtapesParcoursGrille::choix2;
                break;
            }
            case EtapesParcoursGrille::choix2:
            {
                if(!verifierPilier())
                {
                    etape = EtapesParcoursGrille::mil2;
                    break;
                }
                while(!verifierPilier(distancePilierDiagonale))
                {
                    suivreLigneGrille();
                }
                reculer(VITESSE_DEFAUT);
                _delay_ms(delai1TransitionEtat);
                arreterMoteurs();
                bool droit = tournerDroite90Distance();
                if(!droit)
                {
                    etape = EtapesParcoursGrille::droite2;
                    break;
                }
                
                tournerGauche90(false);
                _delay_ms(delai1TransitionEtat);
                reculer(VITESSE_DEFAUT);
                _delay_ms(delai3TransitionEtat);
                tournerGauchePrecis(false);
                arreterMoteurs();
                etape = EtapesParcoursGrille::gauche2;
                break;
            }
            case EtapesParcoursGrille::mil2:
            {
                arreterMoteurs();
                clignoterDel4Hz(false);
                while(true)
                {
                    suivreLigneGrille();
                    if(C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                    C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1)
                        break;
                }
                avancer(VITESSE_DEFAUT);
                _delay_ms(dureeAvancerMoteurs);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if(C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                    C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1)
                        break;
                }
                arreterMoteurs();
                tournerDroite90(true);
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                    C_LIGNE[S2] == 1 && C_LIGNE[S1] == 0) 
                    || (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                    C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0))
                        break;
                }
                etape = EtapesParcoursGrille::fin;
                break;
            }
            case EtapesParcoursGrille::gauche2:
            {
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        { 
                            break;
                        }
                }
                arreterMoteurs();
                tournerDroitePrecis(true);
                clignoterDel4Hz(false);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0) 
                        || (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 0))
                        {
                            break;
                        }
                }
                arreterMoteurs();
                avancer(VITESSE_DEFAUT);
                _delay_ms(800);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0) 
                        || (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 0))
                        {
                            break;
                        }   
                }
                arreterMoteurs();
                tournerDroitePrecis(true);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0) || (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 0))
                        {
                            break;
                        }
                }
                avancer(VITESSE_DEFAUT);
                _delay_ms(dureeAvancerMoteurs);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 0 && C_LIGNE[S1] == 0) 
                        || (C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 0))
                        {
                            break;
                        }
                }
                etape = EtapesParcoursGrille::fin;
                break;
            }
            case EtapesParcoursGrille::droite2:
            {
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        { 
                            break;
                        }
                }
                arreterMoteurs();
                tournerGauchePrecis(true);
                clignoterDel4Hz(false);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 0 && C_LIGNE[S4] == 0 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1) 
                        || (C_LIGNE[S5] == 0 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        {
                            break;
                        }
                }
                avancer(VITESSE_DEFAUT);
                _delay_ms(dureeAvancerMoteurs);
                arreterMoteurs();
                while(true)
                {
                    suivreLigneGrille();
                    if((C_LIGNE[S5] == 1 && C_LIGNE[S4] == 1 && C_LIGNE[S3] == 1 &&
                        C_LIGNE[S2] == 1 && C_LIGNE[S1] == 1))
                        { 
                            break;
                        }
                }
                arreterMoteurs();
                tournerDroitePrecis(true);
                etape = EtapesParcoursGrille::fin;
                break;
            }
            case EtapesParcoursGrille::fin:
            {
                return;
            }
            default:
            break;
        }

    }

    return;
}