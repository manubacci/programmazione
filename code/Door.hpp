#include "Wall.hpp"

class Door: public Entity {
public:
    Door ();
    Door (Room);
    Door (Room, Door, bool);
    Door (Room, Door, Door);
    void draw ();
    void open ();
    Orientation inclination ();
protected:
    Orientation orientation;
    // Indica l'orientamento della porta, ossia se essa è posizionata in orizzontale o in verticale rispetto al sistema della stanza.
};
