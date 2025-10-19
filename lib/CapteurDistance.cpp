

#include "CapteurDistance.h"

CapteurDistance::CapteurDistance(uint8_t portLecture)
{
    this->portLecture = portLecture;
}

CapteurDistance::~CapteurDistance()
{
    
}

uint16_t CapteurDistance::lireDistance()
{
    uint16_t distance;
    can convertisseur;
    distance = convertisseur.lecture(portLecture);
    distance = distance >> 2;
    return distance;
}

bool CapteurDistance::verifierPilier(uint16_t distancePilier)
{
    uint16_t distance = lireDistance();
    if (distance > distancePilier)
    {
        _delay_ms(10);
        distance = lireDistance();
        if (distance > distancePilier)
        {
            return true;
        }
    }
        return false;
}