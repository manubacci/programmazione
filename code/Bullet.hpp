#include "Artifact.hpp"

class Bullet: public Entity {
public:
    Bullet ();
    Bullet (Room, Player, Direction, int, int);
    Bullet (Room, Enemy, Direction, int, int);
    Subject shooter ();
private:
    Subject subject;
    // Indica il soggetto che ha sparato il proiettile.
};
