#include "Level.hpp"

struct Levels {
    Level level;
    // Specifica il nodo, ossia il livello attuale.
    Levels* previous;
    // Punta al nodo padre, ossia al livello precedente.
    Levels* next;
    // Punta al nodo figlio nel sotto-albero sinistro, ossia al livello successivo.
    Levels* secondary;
    // Punta al nodo figlio nel sotto-albero destro, ossia al livello secondario.
};

class Game {
public:
    Game (int, int);
    int process ();
protected:
    void hud ();
    Levels* create (Levels*, Destination);
    Player player;
    // Indica il giocatore gestito dall'utente che si interfaccia con i meccanismi del gioco.
    Levels* levels;
    // Punta alla radice dell'albero binario in cui sono elencati tutti i livelli esplorati dal giocatore.
    Statistics stats, state;
    // Memorizzano gli attributi di stato e le statistiche associate al giocatore.
    int width, height;
    // Indicano il numero di righe e di colonne del terminale su cui viene eseguito il gioco.
};
