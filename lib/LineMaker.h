

#pragma once
#include <avr/io.h>

#define S1 0
#define S2 1
#define S3 2
#define S4 3
#define S5 4

class LineMaker{
    public:
    LineMaker();

    uint8_t* obtenirValeurs();

    void assignerValeurs();
    
    private:
    uint8_t valeurs[5];        // v[0] = s1, v[5] = s5 
    const uint8_t PORT[5] = 
    {
        PA0,PA1,PA2,PA3,PA4
    };

};