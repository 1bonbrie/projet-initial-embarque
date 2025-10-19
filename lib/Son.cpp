
#define F_CPU 8000000UL 
#include "Son.h"

const double FREQUENCE_MIN = 110.0;
const double FREQUENCE_MAX = 880.0;
const uint8_t MAX_NOTE_64 = 60;
const uint16_t TIME_SCALE = 100;

const uint16_t NOTES[37] = 
{
    110, 117, 123, 131, 139, 147, 156, 165, 175, 185,
    196, 208, 220, 233, 247, 262, 277, 294, 311, 330,
    349, 370, 392, 415, 440, 466, 494, 523, 554, 587,
    622, 659, 698, 740, 784, 831, 880
};

Son::Son() 
{
    DDRB |= (1 << PB2) | (1 << PB3);
    PORTB &= ~(1 << PB2);

    // Configure Timer0 en mode CTC et toggle
    TCCR0A = (1 << WGM01) | (1 << COM0A0);
    TCCR0B = (1 << CS02); // Prescaler par défaut = 256
    TCNT0 = 0;
}

void Son::configurerTimer(uint16_t comparaison, uint16_t prescaler) 
{
    OCR0A = comparaison;

    // Choix du prescaler dans TCCR0B
    switch (prescaler) {
        case 1:    TCCR0B = (1 << CS00); break;
        case 8:    TCCR0B = (1 << CS01); break;
        case 64:   TCCR0B = (1 << CS01) | (1 << CS00); break;
        case 256:  TCCR0B = (1 << CS02); break;
        case 1024: TCCR0B = (1 << CS02) | (1 << CS00); break;
        default:   TCCR0B = (1 << CS02); break; // par défaut : 256
    }
}

void Son::emettreSon(double frequence) 
{
    if (frequence < FREQUENCE_MIN)
        frequence = FREQUENCE_MIN;
    else if (frequence > FREQUENCE_MAX)
        frequence = FREQUENCE_MAX;

    uint16_t prescaler = 256;
    uint16_t comparaison = F_CPU / (2 * prescaler * frequence) - 1;

    configurerTimer(comparaison, prescaler);
}

void Son::emettreNote(uint8_t note) 
{
    if (note < 45 || note > 81)
        return;

    uint16_t freq = NOTES[note - 45];
    uint16_t prescaler = (note > MAX_NOTE_64) ? 64 : 256;
    uint16_t comparaison = F_CPU / (2 * prescaler * freq) - 1;
    TCCR0A |= (1 << COM0A0);
    configurerTimer(comparaison, prescaler);
}

void Son::emettreNotePendant(uint8_t note, uint8_t temps) 
{
    emettreNote(note);
    for (uint8_t i = 0; i < temps; i++)
        _delay_ms(TIME_SCALE);
    arreterSon();
}

void Son::arreterSon() 
{
    TCCR0A &= ~(1 << COM0A0);

    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));

    PORTB &= ~(1 << PB3); 

    OCR0A = 0;
}