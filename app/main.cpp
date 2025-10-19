

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Robot.h"
#include "timer1.h"


volatile uint8_t nChoix = 0;
volatile uint8_t choixDirection[2] = {}; 
volatile bool peutDebuterProgramme = false;
volatile bool directionChoisie = true;  
volatile bool doitArreterMoteurs = false;

const uint16_t DELAI_SELECTION_DIRECTION = 2000;
const uint16_t TIMER_750MS = 5858;
const uint16_t DELAI_RECULER_FIN = 1500;

ISR (INT0_vect) {
    Del del(&PORTB, &DDRB, PB0, PB1);
    _delay_ms(DELAI_ANTI_REBOND);

    if (PIND & (1 << PD2)) {
        if (directionChoisie && nChoix < 2)
        {
            del.afficherVert();
            choixDirection[nChoix] = 1; 
            while (PIND & (1 << PD2)){};
            del.eteindreDel();
            nChoix++;

        }
    }
    if (nChoix == 2) 
    {
    _delay_ms(DELAI_SELECTION_DIRECTION);
    peutDebuterProgramme = true;
    directionChoisie = false;
    }
    EIFR |= (1 << INTF0); 
    
}

ISR (INT1_vect) {
    Del del(&PORTB, &DDRB, PB0, PB1);

    _delay_ms(DELAI_ANTI_REBOND);

        if (directionChoisie && nChoix < 2)
        {
            del.afficherRouge();
            choixDirection[nChoix] = 2; 
            while (!(PIND & (1 << PD3))){};
            del.eteindreDel();
            nChoix++;
        }
        if (nChoix == 2) 
        {
        _delay_ms(DELAI_SELECTION_DIRECTION);
        peutDebuterProgramme = true;
        directionChoisie = false;
        }

    
    EIFR |= (1 << INTF1); 
    
}

ISR(TIMER1_COMPA_vect) 
{
    doitArreterMoteurs = true;
}



int main() 
{
    Robot robot(&PORTB, &DDRB, PB0, PB1);
    robot.initialiserInterruptionsBoutons();
    bool aCommenceJ = false;

    const int delaiTransitionEtat = 500;
    const int delaiReculFinal = 350;

    while (!peutDebuterProgramme);
    EtapesParcoursComplet etape = EtapesParcoursComplet::INIT;
    while (true)
    {
        switch(etape)
        {

            case EtapesParcoursComplet::INIT:
            {
                robot.suivreLigne(true, false);
                if (robot.obtenirValeursCapLigne()[S5] == 0 && robot.obtenirValeursCapLigne()[S4] == 0 && robot.obtenirValeursCapLigne()[S3] == 0 &&
                        robot.obtenirValeursCapLigne()[S2] == 0 && robot.obtenirValeursCapLigne()[S1] == 0)
                {
                    etape = EtapesParcoursComplet::ParcoursSelectionDirection;
                    aCommenceJ = false;
                }
                else if (robot.obtenirValeursCapLigne()[S5] == 1 && robot.obtenirValeursCapLigne()[S4] == 1 && robot.obtenirValeursCapLigne()[S3] == 1 &&
                        robot.obtenirValeursCapLigne()[S2] == 1 && robot.obtenirValeursCapLigne()[S1] == 1)
                    {
                        etape = EtapesParcoursComplet::ParcoursGrille;
                        aCommenceJ = true;
                    }
                break;
            }
            case EtapesParcoursComplet::A:
            {
                robot.suivreLigne(true, false);
                if (robot.obtenirDirection() == Direction::Arreter)
                {
                    etape = EtapesParcoursComplet::ParcoursSelectionDirection;
                }
                break;
            }
            case EtapesParcoursComplet::ParcoursSelectionDirection:
            {
                robot.effectuerSelectionDirection(choixDirection);
                etape = EtapesParcoursComplet::C;
                break;
            }

            case EtapesParcoursComplet::C:
            {
                robot.suivreLigne(true, false);
                if (robot.obtenirDirection() == Direction::Droite90)
                {
                    robot.tournerDroite90(true);
                    etape = EtapesParcoursComplet::ParcoursBoucle;
                }
                break;
            }

            case EtapesParcoursComplet::ParcoursBoucle:
            {
                robot.effectuerParcoursBoucle();
                _delay_ms(delaiTransitionEtat);
                etape = EtapesParcoursComplet::F;
                break;
            }

            case EtapesParcoursComplet::F:
            {
                robot.suivreLigne(true,false);
                if (robot.obtenirDirection() == Direction::Droite90)
                {
                    robot.tournerDroite90(true);
                    if (aCommenceJ)
                    {
                        etape = EtapesParcoursComplet::Fin;
                    }
                    else
                    {
                        etape = EtapesParcoursComplet::J;
                    }
                }
                break;
            }
            case EtapesParcoursComplet::J:
            {
                robot.suivreLigne(true, false);
                if (robot.obtenirValeursCapLigne()[S5] == 1 && robot.obtenirValeursCapLigne()[S4] == 1 && robot.obtenirValeursCapLigne()[S3] == 1 &&
                        robot.obtenirValeursCapLigne()[S2] == 1 && robot.obtenirValeursCapLigne()[S1] == 1)
                {
                    etape = EtapesParcoursComplet::ParcoursGrille;
                }
                break;
            }
            case EtapesParcoursComplet::ParcoursGrille:
            {
                robot.effectuerParcoursGrille();
                robot.avancer(VITESSE_DEFAUT);
                _delay_ms(delaiTransitionEtat);
                etape = EtapesParcoursComplet::FinGrille;
                break;                
            }
            case EtapesParcoursComplet::FinGrille:
            {
                robot.suivreLigne(true, false);
                if (aCommenceJ)
                {
                    if (robot.obtenirDirection() == Direction::Droite90)
                    {
                        robot.tournerDroite90(true);
                        etape = EtapesParcoursComplet::A;
                    }                  
                }
                else
                {
                if (robot.obtenirDirection() == Direction::Droite90)
                    {
                        robot.tournerDroite90(true);
                        etape = EtapesParcoursComplet::Fin;
                    }   
                }
                break;
            }
            case EtapesParcoursComplet::Fin:
            {
                Timer1 timer(TIMER_750MS);
                timer.partirMinuterie();
                while (!doitArreterMoteurs)
                {
                    robot.suivreLigne(true,false);
                }
                robot.reculer(VITESSE_DEFAUT);
                _delay_ms(delaiReculFinal);
                robot.arreterMoteurs();
                robot.alternerRougeVert2Hz();
                break;

                return 0;
            }
        }  
    }
}
