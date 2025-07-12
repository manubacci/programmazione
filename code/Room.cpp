#include "Room.hpp"

Room:: Room (){}
Room:: Room (int height, int width){
    this -> height = (rand() % (1+(height < 40 ? height : 40) -5 -13)) + 13;
    this -> width = (rand() % (1+(width < 80 ? width : 80) -12 -19)) + 19;
    /* Assegna al numero di righe e di colonne della finestra da inizializzare dei valori generati casualmente in un intervallo limitato. 
    Per le righe, il limite inferiore dell'intervallo corrisponde alla minima distanza per cui diventa possibile rappresentare i lati 
    orizzontali della stanza e collocare nello spazio fra essi compreso due porte verticali di altezza 3, separate da un muro orizzontale e 
    distanziate di almeno uno spazio dai lati e dal muro. Corrisponde quindi a 13. Per le colonne, si considera invece lo spazio minimo che 
    permette di rappresentare i due lati verticali e due porte orizzontali di larghezza 6, separate da un muro verticale e opportunamente 
    distanziate. Corrisponde dunque a 19. I limiti superiori sono specificati dal massimo numero di righe e di colonne supportate dal 
    terminale. Nel caso in cui i valori siano maggiori rispettivamente di 40 e 80, il limite viene ridotto a questi valori massimi. 
    Per impaginare correttamente la finestra nel terminale fisico e non sovrapporla agli altri elementi della grafica, ai limiti superiori 
    devono essere sottratte 5 righe e 12 colonne. */
    room = newwin (this -> height,this -> width, 4, 10);
    /* Genera un puntatore ad una nuova finestra con il numero di righe e di colonne indicato, posizionata in modo che la sua origine 
    coincida con le coordinate specificate: (10,4), rispetto al sistema del terminale. In questo modo, stampando la finestra sul terminale 
    fisico essa non si sovrappone agli elementi della grafica già disegnati. */
    /*
                        1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
                                                                    0          caso pessimo:
        H e a l t h     ♥ ♥ ♥                                       1
        S c o r e s     0                                           2          terminale:
                                                                    3          18 righe
        ♥     3 0       + - X X X X X X - + - X X X X X X - +       4          31 colonne
        ♡     3 0       |                 |                 |       5
        ➶     1 0       X                                   X       6          finestra:
        ✷     5         X                                   X       7          18-5 = 13 righe
        ⏲     1 6 0     X                                   X       8          31-12 = 19 colonne
        ⇶     1 8       |                 |                 |       9          origine (10,4)
                        + -             - + -             - +       1 0
                        |                 |                 |       1 1
                        X                                   X       1 2
                        X                                   X       1 3
                        X                                   X       1 4
                        |                 |                 |       1 5
                        + - X X X X X X - + - X X X X X X - +       1 6
                                                                    1 7
    */
    keypad (room, true);
    // Abilita la ricezione dell’Input da tastiera immesso nel terminale corrispondente alla finestra.
    wtimeout (room, 5);
    /* Imposta a 5 Tick l'intervallo di attesa fra ogni lettura dei dati in Input. Questo parametro determina anche la frequenza di 
    aggiornamento di tutti gli elementi della grafica. */
    draw ();
}

int Room:: input (){
    return wgetch (room);
    /* Rileva l'Input immesso da tastiera nel terminale corrispondente alla finestra. Questo valore è gestito come un intero per facilitare 
    la ricezione e la gestione dei tasti di funzione. */
}

void Room:: draw() {
    wborder (room,'|','|','-','-','+','+','+','+');
    /* Stampa all'interno della finestra un contorno che ne delimita i bordi, utilizzando i caratteri indicati: '|' per i lati verticali, 
    '-' per gli orizzontali e '+' per gli angoli. Le ascisse a cui hanno origine i due bordi verticali corrispondono rispettivamente a 0 e 
    al numero di colonne decrementato di 1. Le ordinate dei due bordi orizzontali corrispondono invece a 0 e al numero di righe 
    decrementato di 1. */
    /*
           0 1 2 3       schema:
        0  + - - +       
        1  |     |       4 righe, 4 colonne
        2  |     |       lati orizzontali: (0,0), (0,3)
        3  + - - +              verticali: (0,0), (3,0)
    */
    refresh();
}

void Room:: clear(){
    wclear (room);
    // Cancella completamente il contenuto stampato all'interno della finestra.
    refresh();
}

void Room:: refresh(){
    wrefresh (room);
    // Stampa l'impostazione grafica attuale della finestra sullo schermo fisico del terminale.
}

char Room:: get (int y, int x){
    return (char) mvwinch (room, y, x);
    // Verifica il carattere stampato nella finestra alla posizione corrispondente alle coordinate indicate.
}

void Room:: empty (int& y, int& x){
    while (mvwinch (room, y = (rand()%(height-2)+1), x = (rand()%(width-1-2)+1)) != ' ');
    /* Verifica il carattere stampato ad una posizione le cui coordinate sono determinate casualmente fra tutti i valori interni ai bordi 
    della finestra, ossia compresi fra 1 e il numero di righe o di colonne decrementato di 2. Esegue questa operazione fino a quando non 
    rileva una posizione vuota, nella quale è stampato il carattere ' '. Assegna poi le coordinate di questa posizione alle variabili 
    passate per riferimento. */
    /*
           0 1 2 3       schema:
        0  + - - +       
        1  | X X |       4 righe, 4 colonne
        2  | X X |       posizioni vuote: (1,1), (2,1)
        3  + - - +                        (1,2), (2,2)
    */
}

void Room:: area (int& height, int& width){
    height = this -> height;
    width = this -> width;
    /* Assegna alle variabili passate per riferimento i valori corrispondenti al numero di righe e di colonne della finestra, ossia 
    all'altezza e alla larghezza della stanza. */
}

WINDOW* Room:: window (){
    return room;
}