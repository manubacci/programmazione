#include "Room.hpp"

struct Statistics {
    int health;
    // Indica i punti vita dell'entità e corrisponde alla quantità di danni che essa può sopportare prima di essere distrutta.
    Movement movement;
    /* Indica lo stile di movimento dell'entità, ossia se essa si muove casualmente, seguendo un bersaglio o teletrasportandosi, oppure se 
    è ferma. */
    int speed;
    // Indica la velocità di movimento dell'entità e corrisponde al tempo che intercorre fra ogni suo passo ed il passo ad esso successivo.
    Attack attack;
    /* Indica lo stile di attacco dell'entità, ossia se essa attacca corpo a corpo, sparando un proiettile verso il bersaglio o sparando 
    quattro proiettili le cui traiettorie formano un più o una croce, oppure se è innocua. */
    int cooldown;
    /* Indica il tempo di caricamento degli attacchi dell'entità e corrisponde al tempo che intercorre fra ogni suo attacco e l'attacco 
    ad esso successivo. */
    int damage;
    /* Indica il potenziale di danno dell'entità e corrisponde alla quantità di punti vita che ogni suo attacco sottrae al bersaglio. */
    int shots;
    /* Indica la velocità dei proiettili sparati dell'entità e corrisponde al tempo che intercorre fra ogni loro movimento e il movimento 
    ad esso successivo. */
    int range;
    /* Indica la gittata dei proiettili sparati dall'entità e corrisponde alla massima distanza che questi possono percorrere prima di 
    essere distrutti. */
    int sight;
    // Indica l'area visiva dell'entità e corrisponde alla distanza entro la quale può interagire con il bersaglio.
    int points;
    /* Indica i punti esperienza dell'entità e corrisponde alla quantità di punti che il giocatore possiede o che può può guadagnare 
    sconfiggendo il bersaglio. */
    Direction direction;
    /* Indica la direzione di movimento dell'entità, ossia se al suo passo successivo essa dovrà muoversi verso Nord, Sud, Est, Ovest o una 
    delle possibili combinazioni di questi, oppure se dovrà stare ferma nel punto in cui è posizionata. */
    Extention extention;
    /* Indica la direzione dell'estensione dell'entità, ossia se essa si trova immediatamente sopra, sotto, a destra o a sinistra rispetto 
    alla posizione della corrispondente entità, oppure se non è presente. */
};

class Entity {
public:
    Entity ();
    Entity (Room, char = ' ', int = 0, int = 0);
    void position (int&, int&);
    void extention (int&, int&, Direction = Point);
    void next (int&, int&, Direction = Point);
    void clear ();
    void draw ();
    void move ();
    void move (int, int);
    void decrease (Variant);
    void reset (Variant);
    void direction (Direction);
    void hit (int);
    void score (int);
    Statistics statistics ();
    Statistics state ();
protected:
    void initialize ();
    int y, x;
    char icon;
    Statistics stats, status;
    /* Le statistiche dell'entità rappresentano le sue condizioni generali mentre i valori associati ai corrispondenti stati indicano
    la variazione di questi valori all'avanzare del tempo di gioco. Gli attributi di stato più importanti ai fini dell'esecuzione sono
    quelli ai quali è assegnato un valore che deve decrementare nel tempo, seguendo la frequenza di aggiornamento dell'interfaccia, o 
    in conseguenza a determinate azioni. Questi stati sono: i punti vita, la velocità di movimento, il tempo di caricamento degli 
    attacchi, a velocità dei proiettili e la gittata dei proiettili. */
    Room room;
};
