

#ifndef SON_H
#define SON_H

#include <avr/io.h>
#include <util/delay.h>

class Son {
public:
    Son();

    void emettreSon(double frequence);                   
    void emettreNote(uint8_t note);                       
    void emettreNotePendant(uint8_t note, uint8_t temps); 
    void arreterSon();                                    

    private:
    void configurerTimer(uint16_t comparaison, uint16_t prescaler); 
};

#endif