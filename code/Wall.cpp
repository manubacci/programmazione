#include "Wall.hpp"

Wall:: Wall (){};
Wall:: Wall (Room room): Entity (room){
/* I muri sono implementati come delle entità, appartenenti alla stanza indicata, la cui icona e la cui posizione sono irrilevanti al 
momento dell'inizializzazione. */
    int height, width;
    room.area (height, width);
    switch (rand()%3){
        case 0: orientation = Horizontal; break;
        case 1: orientation = Vertical; break;
        case 2: orientation = None; break;
    }
    // Determina casualmente se il muro principale è orientato in orizzontale o in verticale, oppure se non è presente.
    if (orientation == None) side = Zero;
    else switch (rand()%3){
        case 0: side = Zero; break;
        case 1: side = First; break;
        case 2: side = Second; break;
    }
    /* Se è presente un muro principale, determina casualmente se il muro secondario si trova nella prima o nella seconda delle due sezioni
    in cui questo suddivide la stanza, oppure se non esiste. */
    y = rand()%(1+((height-1)-6)-6)+6;
    x = rand()%(1+((width-1)-9)-9)+9;
    /* Determina un'ordinata e un'ascissa casuali alle quali collocare il muro principale e il muro secondario a seconda della loro 
    posizione, qualora esistano. Il valore dell'ordinata è generato nell'intervallo compreso fra l'ordinata del lato orizzontale superiore 
    incrementata di 6 e l'ordinata del lato orizzontale inferiore decrementata di 6. In questo modo, il muro orizzontale posizionato 
    all'ordinata generata non interferisce con lo spazio minimo necessario per collocare fra esso e il lato parallelo più vicino una porta 
    verticale di altezza 3, lasciando uno spazio di distanza sopra e sotto di essa. Il valore dell'ascissa deve invece appartenere 
    all'intervallo compreso fra l'ascissa del lato verticale sinistro incrementata di 9 e l'ascissa del lato verticale destro decrementata 
    di 9, per impaginare correttamente un'eventuale porta orizzontale di larghezza 6. Per includere nell'intervallo anche gli estremi 
    superiori occorre incrementarli ulteriormente di 1. */
    /*
                               colonne -
        0 1 2 3 4 5 6 7 8 9 .. 10 9 8 7 6 5 4 3 2 1

        + - X X X X X X - + ... + - X X X X X X - +    0        muri verticali: (9, 0), (colonne-10, 0)
        |                 |     |                 |    1           orizzontali: (0, 6), (0, righe-7)
        X                 |     |                 X    2
        X                 |     |                 X    3
        X                 |     |                 X    4
        |                 |     |                 |    5
        + - - - - - - - - + ... + - - - - - - - - +    6
       ...               ...   ...               ...  ...
        + - - - - - - - - + ... + - - - - - - - - +    7  righe -
        |                 |     |                 |    6
        X                 |     |                 X    5
        X                 |     |                 X    4
        X                 |     |                 X    3
        |                 |     |                 |    2
        + - X X X X X X - + ... + - X X X X X X - +    1
    */
    yp = 0; xp = 0;
    draw();
}

void Wall:: draw (){
    int height, width;
    room.area (height, width);
    bool free;
    
    if (orientation == Horizontal){
        mvwhline (room.window(), y, 0, '+', 1+((width-1)-0));
        mvwhline (room.window(), y, 1, '-', 1+(((width-1)-0)-2));
        /* Se il muro principale è orientato in orizzontale, stampa una linea orizzontale di caretteri '+', che ha origine alla posizione 
        indicata dall'ascissa del lato verticale sinistro e dall'ordinata generata, con una lunghezza pari alla distanza fra il muro 
        verticale sinistro e il muro verticale destro, estremi compresi. Sovrascrive poi ad essa una linea orizzontale di caratteri '-', a 
        partire dall'ascissa del lato verticale sinistro incrementata di 1, con una lunghezza pari alla lunghezza della linea sovrascritta 
        decrementata di 2. Stampa in questo modo una sequenza di caratteri '-' che inizia e termina con un '+' e percorre la stanza dal 
        lato sinistro al lato destro. */
        if (side == First){
            mvwvline (room.window(), 0, x, '+', 1+(y-0));
            mvwvline (room.window(), 1, x, '|', 1+((y-0)-2));
            /* Se il muro secondario è posizionato nella prima delle due sezioni in cui la stanza è suddivisa, stampa un muro verticale che 
            ha origine alla posizione indicata dall'ascissa generata e dall'ordinata del lato orizzontale superiore, con una lunghezza pari 
            alla differenza fra l'ordinata comune a tutti i punti del muro principale e l'ordinata del lato orizzontale superiore, estremi 
            compresi. Disegna poi il muro come una sequenza verticale di caratteri '|' che inizia e termina con un '+' e percorre la stanza 
            dal lato superiore al muro principale. */
            if (yp == 0) do {
                free = true;
                yp = rand()%(1+(y-(1+3))-2)+2;
                for (int i=0; i<5 && free; i++) if (room.get (yp-1+i,x) != '|') free = false;
            } while (!free);
            mvwvline (room.window(), yp, x, ' ', 3);
            /* Genera una nuova ordinata con un valore determinato casualmente in un intervallo limitato. A partire dalla posizione 
            indicata da questa ordinata e dall'ascissa del muro secondario stampa un varco, ossia una sequenza verticale di 3 spazi bianchi 
            ' ' che sovrascrive il muro. Il limite inferiore dell'intervallo è la minima fra le ordinate dei punti appartenenti al muro 
            secondario che permette di posizionare questo varco in modo che la sua origine sia distanziata di almeno uno spazio dal muro 
            orizzontale superiore, e corrisponde quindi a 2. Il limite superiore è invece specificato dalla minima ordinata a cui 
            posizionare l'origine del varco in modo da garantire una distanza di almeno 4 spazi fra essa e il muro principale: 3 spazi per 
            rappresentare il varco e 1 per distanziarlo dal muro, e corrisponde dunque all'ordinata del muro principale decrementata di 4. 
            Ripete poi questo procedimento fino a quando non trova una posizione idonea in cui collocare il varco, ossia tale che i 3 
            caratteri verticali da stampare, lo spazio che li precede e lo spazio che li segue, si sovrappongano unicamente ai caratteri 
            che specificano il muro secondario: '|'. */
            /*
                                           colonne -
                0 1 2 3 4 5 6 7 8 9   ...  10 9 8 7 6 5 4 3 2 1

                + - X X X X X X - + - ... - + - X X X X X X - +    0        passaggi verticali: (9, 2), (colonne-10, 2),
                |                 |         |                 |    1                            (9, righe-5), (collone-10, righe-5)
                X                                             X    2               orizzontali: (2, 6), (colonne-8, 6),
                X                                             X    3                            (2, righe-7, colonne-8, righe-7)
                X                                             X    4
                |                 |         |                 |    5
                + -             - + - ... - + -             - +    6
                |                 |         |                 |
               ...               ...       ...               ...  ...
                |                 |         |                 |
                + -             - + - ... - + -             - +    7
                |                 |         |                 |    6
                X                                             X    5
                X                                             X    4
                X                                             X    3
                |                 |         |                 |    2
                + - X X X X X X - + - ... - + - X X X X X X - +    1
                                                                   righe -
            */
        } else if (side == Second){
            mvwvline (room.window(), y, x, '+', 1+((height-1)-y));
            mvwvline (room.window(), y+1, x, '|', 1+(((height-1)-y)-2));
            if (yp == 0) do {
                free = true;
                yp = rand()%(1+((height-1)-(3+1))-(y+2))+(y+2);
                for (int i=0; i<5 && free; i++) if (room.get (yp-1+i,x) != '|') free = false;
            } while (!free);
            mvwvline (room.window(), yp, x, ' ', 3);
        }
        /* Se il muro secondario è posizionato nella seconda delle due sezioni, stampa un muro verticale che percorre la stanza dal muro 
        principale al lato orizzontale inferiore. Su questo muro disegna un varco verticale di altezza 3, che ha origine ad un'ordinata 
        generata casualmente nell'intervallo compreso fra l'ordinata del muro principale incrementata di 2 e l'ordinata del lato 
        orizzontale inferiore decrementata di 4. */
        if (xp == 0) do {
            free = true;
            xp = rand()%(1+((width-1)-(6+1))-2)+2;
            for (int i=0; i<8 && free; i++) if (room.get (y,xp-1+i) != '-') free = false;
        } while (!free);
        mvwhline (room.window(), y, xp, ' ', 6);
        /* Stampa sul muro principale un varco orizzontale di larghezza 6, che ha origine ad un'ascissa generata casualmente 
        nell'intervallo compreso fra l'ascissa del lato verticale sinistro incrementata di 2 e l'ascissa del lato verticale destro 
        decrementata di 7: 6 spazi per rappresentare il varco e 1 per distanziarlo dal lato. */
    }
    else if (orientation == Vertical){
        mvwvline (room.window(), 0, x, '+', 1+((height-1)-0));
        mvwvline (room.window(), 1, x, '|', 1+(((height-1)-0)-2));
        /* Se il muro principale è orientato in verticale stampa un muro verticale che percorre la stanza dal lato orizzontale superiore al 
        lato orizzontale inferiore. */
        if (side == First){
            mvwhline (room.window(), y, 0, '+', 1+(x-0));
            mvwhline (room.window(), y, 1, '-', 1+((x-0)-2));
            if (xp == 0) do {
                free = true;
                xp = rand()%(1+(x-(6+1))-2)+2;
                for (int i=0; i<8 && free; i++) if (room.get (y,xp-1+i) != '-') free = false;
            } while (!free);
            mvwhline (room.window(), y, xp, ' ', 6);
            /* Se il muro secondario è posizionato nella prima delle due sezioni, stampa un muro orizzontale che percorre la stanza dal 
            lato verticale sinistro al muro principale. Su questo muro disegna un varco orizzontale di larghezza 6 che ha origine ad 
            un'ascissa generata casualmente nell'intervallo compreso fra l'ascissa del lato verticale sinistro incrementata di 2 e 
            l'ascissa del muro principale decrementata di 7. */
        } else if (side == Second){
            mvwhline (room.window(), y, x, '+', 1+((width-1)-x));
            mvwhline (room.window(), y, x+1, '-', 1+(((width-1)-x)-2));
            if (xp == 0) do {
                free = true;
                xp = rand()%(1+((width-1)-(6+1))-(x+2))+(x+2);
                for (int i=0; i<8 && free; i++) if (room.get (y,xp-1+i) != '-') free = false;
            } while (!free);
            mvwhline (room.window(), y, xp, ' ', 6);
            /* Se il muro secondario è posizionato nella seconda delle due sezioni, stampa un muro orizzontale che percorre la stanza dal 
            muro principale al lato verticale destro. Su questo muro disegna un varco orizzontale di dimensione 6 che ha origine ad 
            un'ascissa generata casualmente nell'intervallo compreso fra l'ascissa del muro principale incrementata di 2 e l'ascissa del 
            lato verticale destro decrementata di 7. */
        }
        if (yp == 0) do {
            free = true;
            yp = rand()%(1+((height-1)-(3+1))-2)+2;
            for (int i=0; i<5 && free; i++) if (room.get (yp-1+i,x) != '|') free = false;
        } while (!free);
        mvwvline (room.window(), yp, x, ' ', 3);
        /* Stampa sul muro principale un varco verticale di altezza 3, che ha origine ad un'ordinata generata casualmente nell'intervallo 
        compreso fra l'ordinata del muro orizzontale superiore incrementata di 2 e l'ordinata del lato orizzontale inferiore decrementata 
        di 4. */
    }
    room.refresh();
}