#include <iostream>
#include "Game.hpp"

int main () {
    setlocale (LC_ALL, "");
    srand ((unsigned) time (NULL));
    if (!initscr()) {
        std:: cout << "Error: The screen could not be initialized.\n";
        return 0;
    }
    curs_set (FALSE);
    noecho();
    /* Imposta il terminale nella corretta modalità per l'esecuzione del programma, permettendo la stampa di caratteri speciali nella 
    codifica UTF-8, disabilitando la visibilità del cursore e facendo in modo che i caratteri digitati dall'utente non vengono ripetuti più 
    di una volta. */
    srand ((unsigned) time (NULL));
    int height, width;
    getmaxyx (stdscr, height, width);
    // Determina il massimo numero di righe e di colonne di cui è composto il terminale
    if (height < 18 || width < 31) {
        endwin ();
        std:: cout << "Error: Window size is too small. Resize the window and try again.";
        /* Qualora la dimensione del terminale sia inferiore a 18 righe e 31 colonne, ossia se lo spazio a disposizione non consente 
        neppure di impaginare correttamente una stanza la cui area corrisponde al minimo valore possibile, termina l'esecuzione del gioco e 
        stampa un messaggio di errore per avvisare che la dimensione della finestra è insufficiente e per invitare l'utente a ingrandirla. */
        /*
                            1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
                                                                        0          caso pessimo:
            H e a l t h     ♥ ♥ ♥                                       1
            S c o r e s     0                                           2          terminale:
                                                                        3          18 righe
            ♥     3 0       + - X X X X X X - + - X X X X X X - +       4          31 colonne
            ♡     3 0       |                 |                 |       5
            ➶     1 0       X                                   X       6 
            ✷     5         X                                   X       7
            ⏲     1 6 0     X                                   X       8
            ⇶     1 8       |                 |                 |       9
                            + -             - + -             - +       1 0
                            |                 |                 |       1 1
                            X                                   X       1 2
                            X                                   X       1 3
                            X                                   X       1 4
                            |                 |                 |       1 5
                            + - X X X X X X - + - X X X X X X - +       1 6
                                                                        1 7
    */
    } else {
        Game game (height, width);
        int status;
        do status = game.process();
        while (status == 0);
        endwin ();
        // Ripristina il terminale alla versione precedente all'esecuzione del programma.
        if (status == 2) std:: cout << "Error: The window cannot be resized while the program is running.";
        else if (status == 1) std:: cout << "Oh no, it seems that you have been defeated! You can always try again!";
        /* Ad ogni iterazione, ossia ad ogni aggiornamento della grafica, processa il comportamento del gioco e analizza il suo stato. 
        Ripete questa operazione fino a quando lo stato indica che il gioco sta sta procedendo in modo corretto. Nel caso in cui lo stato 
        indica che il gioco si è concluso a causa di un errore dovuto al ridimensionamento della finestra del terminale, interrompe 
        l'esecuzione e stampa un messaggio di errore che invita l'utente a non ripetere questo comportamento. Se lo stato indica invece che 
        il giocatore ha perso, stampa un messaggio per avvisare l'utente della sconfitta. */
    }
    return 0;
}
