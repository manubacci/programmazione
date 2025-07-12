#include "Player.hpp"

class Artifact: public Entity {
public:
    Artifact ();
    Artifact (Room, Player, int, int, bool = false);
    Player upgrade (Player player);
private:
    bool power;
    // Indica se l'artefatto racchiude o meno un potere.
};
