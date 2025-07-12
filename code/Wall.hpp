#include "Entity.hpp"

class Wall: public Entity {
public:
    Wall ();
    Wall (Room);
    void draw ();
protected:
    Orientation orientation;
    /* Indica l'orientamento del muro principale, ossia se esso è posizionato in orizzontale o in verticale rispetto al sistema della 
    stanza, oppure se non è presente. */
    Side side;
    /* Indica in quale parte della stanza si trova il muro secondario, ossia se esso è collocato nella prima o nella seconda delle sezioni
    in cui il muro primario suddivide la stanza, oppure se non esiste. */
    int yp, xp;
    // Indicano l'ascissa e l'ordinata a cui hanno origine i due varchi perpendicolari che dividono i due muri.
};
