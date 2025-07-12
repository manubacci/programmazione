#include "Door.hpp"

class Enemy: public Entity {
public:
    Enemy ();
    Enemy (Room, int, int);
private:
    void initialize();
    Breed breed;
    // Indica la razza a cui appartiene il nemico e determina i valori inizialmente assegnati alle sue statistiche.
};
