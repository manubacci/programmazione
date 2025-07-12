#include "Enums.hpp"

class Room {
public:
    Room ();
    Room (int, int);
    int input ();
    void draw ();
    void clear ();
    void refresh ();
    char get (int, int);
    void empty (int&, int&);
    void area (int&, int&);
    WINDOW* window ();
protected:
    WINDOW* room;
    // Punta alla finestra inizializzata, al cui interno viene generata la nuova stanza.
    int height, width;
    // Indicano rispettivamente il numero di righe e di colonne della finestra, ossia l'altezza e la larghezza della stanza corrispondente.
};
