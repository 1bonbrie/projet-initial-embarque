

#pragma once

enum class EtapesParcoursBoucle
{
    INIT,
    E_0,
    F_0,
    G,
    GversH,
    GversI,
    H,
    I,
    E_1,
    F_1
};

enum class EtapesParcoursGrille {
    choix1,
    mil1,
    droite1,
    gauche1,
    choix2,
    mil2,
    droite2,
    gauche2,
    fin

};


enum class EtapesParcoursComplet
{
    INIT,
    A,
    ParcoursSelectionDirection,
    C,
    ParcoursBoucle,
    F,
    J,
    ParcoursGrille,
    FinGrille,
    Fin
};
