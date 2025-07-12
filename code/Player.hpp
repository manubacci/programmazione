#include "Enemy.hpp"

struct Transition {
    Destination last;
    /* Indica la direzione dell'ultima porta attraversata dal giocatore, ossia se la porta che il giocatore ha utilizzato per passare nel 
    livello attuale conduceva ad un livello successivo, precedente, secondario o speciale, oppure se il giocatore si trova attualmente 
    nella stanza di partenza. */
    Destination current;
    /* Indica la direzione della porta che il giocatore sta attraversando, ossia se la porta che sta utilizzando il giocatore per passare 
    ad un nuovo livello conduce ad un livello successivo, precedente, secondario o speciale, oppure se il giocatore si sta muovendo nella 
    stanza attuale senza attraversare nessuna porta. */
    Door door;
    // Indica la porta attraversata dal giocatore per passare ad un nuovo livello.
    int coordinate;
    /* Indica l'ascissa o l'ordinata del punto preciso in cui il giocatore sta attraversando la porta, in relazione all'ascissa o 
    all'ordinata in cui è posizionata l'origine della porta stessa. */
};

class Player: public Entity {
public:
    Player ();
    Player (Room, int, int);
    void upgrade (Statistics);
    void transfer (Room, int, int);
    void transition (Destination, Door, int);
    Transition passage ();
protected:
    Transition destination;
    // Indica le caratteristiche e la destinazione della transizione del giocatore verso un nuovo livello.
};
