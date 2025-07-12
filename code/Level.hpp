#include "Bullet.hpp"

struct Enemies {
    Enemy enemy;
    Enemies* next;
};

struct Bullets {
    Bullet bullet;
    Bullets* next;
};

class Level {
public:
    Level ();
    Level (int, int);
    Level (Player, int, int);
    void draw (Player);
    Player initialize ();
    Player initialize (Player);
    Player process (Player);
private:
    Room room;
    // Indica la stanza su cui è strutturato il livello corrente.
    Wall wall;
    // Indica i muri primari e secondari presenti all'interno della stanza.
    bool first, third, special;
    /* Indicano rispettivamente se nel livello devono essere presenti: una sola porta, esattamente tre porte o un numero indifferente di 
    porte di cui una deve essere speciale. */
    Door in, out, secondary;
    // Indicano, qualora siano presenti, la porta di ingresso, la porta di uscita e la porta secondaria posizionate nella stanza.
    Enemies* enemies;
    // Punta all'elemento in testa alla lista nella quale sono elencati tutti i nemici attualmente vivi del livello.
    Bullets* bullets;
    // Punta alla lista contenente tutti i proiettili sparati e non ancora distrutti.
    bool generated;
    // Indica se nel livello è stato generato o meno un artefatto con cui il giocatore non ha ancora interagito.
    Artifact artifact;
    // Indica l'artefatto, qualora sia stato generato.
    Player handle (Player);
    void create (Bullets*, Player, Direction);
    void create (Bullets*, Enemy, Direction);
    Player handle (Bullets*, Player);
    void erase (Bullets*, Bullets*);
    void create (Enemies*);
    Player handle (Enemies*, Player);
    Player erase (Enemies*, Bullet, Player);
};
