#include "Game.hpp"

Game:: Game (int height, int width){
    this -> height = height;
    this -> width = width;
    levels = new Levels;
    levels -> level = Level (height, width);
    levels -> next = NULL;
    levels -> secondary = NULL;
    levels -> previous = NULL;
    /* Genera un nuovo livello che corrisponde al punto di partenza del gioco e lo aggiunge in testa all'apposita lista, come radice 
    dell'albero binario in cui sono elencati tutti i livelli esplorati dal giocatore. Questo livello non è attualmente associato a nessun
    altro livello successivo o secondario. */
    player = levels -> level.initialize ();
    // Genera un nuovo giocatore e lo inizializza all'interno del primo livello del gioco.
    hud ();
}

int Game:: process (){
    int y, x;
    getmaxyx (stdscr, y, x);
    if (y < height || x < width) return 2;
    /* Se il numero attuale di righe o di colonne del terminale è inferiore al corrispondente valore determinato all'inizio del gioco, 
    ossia se l'utente ha diminuito le dimensioni della finestra nel corso dell'esecuzione, termina l'esecuzione indicando che si è 
    verificato un errore. La scelta di interrompere il gioco è dovuta al fatto che ridimensionare la finestra del terminale quando tutti i 
    componenti dell'interfaccia sono già stati stampati e gestiti può causare gravi errori nella grafica. */
    if (player.state().health <= 0) return 1;
    /* Se il valore attualmente assegnato all'attributo di stato che indica la salute del giocatore è uguale o inferiore a 0, termina 
    l'esecuzione indicando che l'utente è stato sconfitto. */
    if (player.passage().current == Next){
        if (levels -> next == NULL) levels = create (levels, Next);
        levels = levels -> next;
        player = levels -> level.initialize (player);
        /* Se i valori assegnati agli attributi che descrivono la transizione del giocatore indicano che esso sta attraversando una porta 
        diretta verso un livello successivo, passa alla gestione del livello successivo nella lista. Considera come livello successivo 
        il livello associato al nodo figlio nel sotto-albero sinistro del nodo corrispondente al livello attuale, generandolo nel caso in 
        cui non sia già stato esplorato in precedenza. Trasferisce poi il giocatore e lo inizializza nuovamente all'interno del livello. */
    } else if (player.passage().current == Previous){
        levels = levels -> previous;
        player = levels -> level.initialize (player);
        /* Se il giocatore sta attraversando una porta diretta verso un livello precedente, passa alla gestione del livello precedente, 
        ossia del nodo padre. Trasferisce poi il giocatore e lo inizializza nuovamente all'interno del livello. */
    } else if (player.passage().current == Secondary || player.passage().current == Special){
        if (levels -> secondary == NULL) levels = create (levels, Secondary);
        levels = levels -> secondary;
        player = levels -> level.initialize (player);
        /* Se il giocatore sta attraversando una porta diretta verso un livello secondario o speciale, passa alla gestione del livello 
        secondario, ossia del nodo figlio nel sotto-albero destro, generandolo nel caso in cui non sia già stato esplorato in precedenza. 
        Trasferisce poi il giocatore e lo inizializza nuovamente all'interno del livello. */
    }
    player = levels -> level.process (player);
    if (state.health != player.state().health || 
        stats.points != player.statistics().points ||
        stats.health != player.statistics().health ||
        stats.range != player.statistics().range ||
        stats.damage != player.statistics().damage ||
        stats.cooldown != player.statistics().cooldown ||
        stats.shots != player.statistics().shots) hud();
        /* Se uno dei valori attualmente assegnati agli attributi di stato o alle statistiche del giocatore ha subito una variazione 
        rispetto ai corrispondenti valori memorizzati in precedenza, aggiorna l'interfaccia in modo da concretizzare questo cambiamento. */
    return 0;
    // Indica che l'esecuzione sta procedendo in modo corretto.
}

void Game:: hud (){
    stats = player.statistics();
    state = player.state();
    // Memorizza i valori attualmente assegnati agli attributi di stato e alle statistiche del giocatore.
    clear();
    // Cancella completamente il contenuto stampato nel terminale.
    mvprintw (1, 2, "Health  "); for (int i = 0; i < (int) ceil (state.health/10.0); i++) printw ("♥");
    /* Stampa nella seconda riga del terminale, a partire dalla terza colonna: (2,1), l'indicatore che specifica la salute residua del 
    giocatore. In particolare, dopo al titolo dell'indicatore stampa 1 cuore se allo stato associato ai punti vita del giocatore è 
    assegnato un valore compreso fra 0 e 10, 2 cuori se il valore è compreso fra 11 e 20 o 3 cuori se è compreso fra 21 e 30. */
    mvprintw (2, 2, "Scores  %d", stats.points);
    mvprintw (4, 2, "♥  %d", stats.health);
    mvprintw (5, 2, "♡  %d", state.health);
    mvprintw (6, 2, "➶  %d", stats.range);
    mvprintw (7, 2, "✷  %d", stats.damage);
    mvprintw (8, 2, "⏲  %d", stats.cooldown);
    mvprintw (9, 2, "⇶  %d", stats.shots);
    /* Stampa nelle righe successive gli indicatori che specificano le restanti caratteristiche del giocatore: i punti esperienza, la 
    salute massima, i punti vita attuali, la gittata dei proiettili, il potenziale di danno, il tempo di caricamento degli attacchi e la 
    velocità dei proiettili. In particolare, dopo al titolo o all'icona che rappresenta l'indicatore stampa il valore associato al 
    corrispondente attributo di stato o alla corrispondente statistica del giocatore. */
    refresh();
    // Stampa sul terminale fisico l'impostazione grafica della finestra corrispondente.
    levels -> level.draw (player);
    // Stampa nuovamente l'impostazione grafica della finestra associata alla stanza su cui si struttura il livello attuale.
    /*
    0 1 2 ...
                                  0
        H e a l t h     ♥ ♥ ♥     1               
        S c o r e s     0         2
                                  3
        ♥     3 0                 4
        ♡     3 0                 5
        ➶     1 0                 6
        ✷     5                   7
        ⏲     1 6 0               8
        ⇶     1 8                 9
                                  ...
    */
}

Levels* Game:: create (Levels* levels, Destination destination){
    Levels* head = levels;
    if (destination == Next){
        levels -> next = new Levels;
        levels -> next -> level = Level (player, height, width);
        levels -> next -> next = NULL;
        levels -> next -> secondary = NULL;
        levels -> next -> previous = levels;
        /* Se il giocatore è diretto verso un livello successivo non ancora esplorato, genera un nuovo livello e lo associa al nodo figlio 
        nel sotto-albero sinistro del nodo corrispondente al livello attuale. In particolare, fa in modo che l'elemento corrispondente al 
        nuovo livello sia puntato dal campo del livello attuale che punta al livello ad esso successivo e che il livello attuale sia invece 
        puntato dal campo del livello generato che punta al livello ad esso precedente. Il livello viene generato specificando il giocatore 
        che dovrà essere trasferito al suo interno. Al livello non è poi associato nessun altro livello successivo o secondario. */
    } else if (destination == Secondary || destination == Special){
        levels -> secondary = new Levels;
        levels -> secondary -> level = Level (player, height, width);
        levels -> secondary -> next = NULL;
        levels -> secondary -> secondary = NULL;
        levels -> secondary -> previous = levels;
        /* Se il giocatore è diretto verso un livello secondario o speciale non ancora esplorato, genera un nuovo livello e lo associa al
        nodo figlio nel sotto-albero destro del nodo corrispondente al livello attuale. In particolare, fa in modo che l'elemento 
        corrispondente al nuovo livello sia puntato dal campo del livello attuale che punta al suo livello secondario e che il livello 
        attuale sia invece puntato dal campo del livello generato che punta al livello ad esso precedente. Al livello non è poi associato 
        nessun altro livello. */
    }
    return head;
}