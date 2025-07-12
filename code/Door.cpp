#include "Door.hpp"

Door:: Door (){}
Door:: Door (Room room): Entity (room, 'X'){
// Le porte sono implementate come delle entità, la cui icona è 'X' e la cui posizione è irrilevante al momento dell'inizializzazione.
    orientation = (rand()%2 ? Horizontal : Vertical);
    // Determina casualmente se la porta deve essere orientata in orizzontale o in verticale.
    int height, width;
    room.area (height, width);
    bool free = false;
    if (orientation == Horizontal){
        do {
            free = true;
            y = (rand()%2)*(height-1);
            x = rand()%(1+((width-1)-(6+1))-2)+2;
            for (int i=0; i<8 && free; i++) if (room.get (y,x-1+i) != '-') free = false;
        } while (!free);
        /* Se la porta deve essere orientata in orizzontale, determina casualmente su quale dei due lati orizzontali posizionarla, ossia se 
        la sua ordinata deve corrispondere all'ordinata del lato superiore o a quella del lato inferiore. Genera poi un'ascissa casuale 
        alla quale posizionare l'origine della porta, nell'intervallo compreso fra l'ascissa del lato verticale sinistro incrementata di 2 
        e l'ascissa del lato verticale destro decrementata di 7, in modo la lasciare almeno uno spazio libero dal lato sinistro e 7 dal 
        destro: 6 per rappresentare la porta e 1 per distanziarla dal muro nel caso pessimo. Ripete questo procedimento fino a quando non 
        trova dei valori tali per cui stampando, a partire dalla posizione indicata dall'ordinata e dall'ascissa generata, una sequenza di 
        6 caratteri orizzontali preceduti e seguiti da uno spazio libero, vengono sovrascritti solo i caratteri che identificano un lato 
        orizzontale: '-'. In questo modo la porta non interferirà con i muri già presenti nella stanza, alla cui base è stampato il 
        carattere '+' .*/
    } else if (orientation == Vertical){
        do {
            free = true;
            y = rand()%(1+(height-1)-(3+1)-2)+2;
            x = (rand()%2)*(width-1);
            for (int i=0; i<5 && free; i++) if (room.get (y-1+i,x) != '|') free = false;
        } while (!free);
        /* Se la porta deve essere posizionata in verticale, determina casualmente se la sua ascissa deve corrispondere all'ascissa del 
        lato verticale sinistro o a quella del lato verticale destro. L'ordinata è generata casualmente nell'intervallo compreso fra 
        l'ordinata del lato orizzontale superiore incrementata di 2 e l'ordinata del lato orizzontale inferiore decrementata di 5: 3 spazi 
        per rappresentare la porta e 1 per distanziarla. Ripete poi questo procedimento fino a quando non trova una posizione tale per cui 
        i 3 caratteri verticali, lo spazio che li precede e lo spazio che li segue si sovrappongano unicamente ai caratteri che 
        identificano un lato verticale: '|'. */
    }
    draw();
}

Door:: Door (Room room, Door door, bool specular): Entity (room, 'X'){
    orientation = (specular ? door.inclination() : (rand()%2 ? Horizontal : Vertical));
    /* Se viene specificato che la nuova porta deve essere generata in modo speculare rispetto ad un'altra porta appartenente ad una stanza 
    diversa rispetto a quella nella quale deve essere posizionata, gli associa lo stesso orientamento di questa porta, altrimenti lo 
    determina in modo casuale. */
    int y, x;
    door.position (y, x);
    int height, width;
    room.area (height, width);
    bool free = false;
    if (orientation == Horizontal){
        if (specular || orientation == door.inclination()) this -> y = (y == 0 ? height-1 : 0);
            /* Se la nuova porta deve essere orientata in orizzontale in modo speculare rispetto ad un'altra porta, la posiziona nel lato 
            orizzontale opposto rispetto a quello in cui è collocata la porta indicata. In particolare, se l'ordinata di questa porta è 
            uguale all'ordinata del lato orizzontale superiore, assegna all'ordinata della nuova porta l'ordinata del lato orizzontale 
            inferiore. Se invece è diversa, gli assegna l'ordinata del lato orizzontale superiore. Questo ragionamento vale sia quando 
            viene espressamente specificato che la nuova porta deve essere speculare rispetto ad un'altra porta appartenente ad una stanza 
            diversa, sia quando la porta indicata è una porta nella stessa stanza con lo stesso orientamento della porta da generare. */
        else this -> y = (rand()%2)*(height-1);
        do {
            free = true;
            this -> x = rand()%(1+((width-1)-(6+1))-2)+2;
            for (int i=0; i<8 && free; i++) if (room.get (this -> y, this -> x-1+i) != '-') free = false;
        } while (!free);
    } else if (orientation == Vertical){
        if (specular || orientation == door.inclination()) this -> x = (x == 0 ? width-1 : 0);
            /* Se la nuova porta deve essere orientata in verticale in modo speculare rispetto ad un'altra porta e l'ascissa di questa 
            porta è uguale all'ascissa del lato verticale sinistro, assegna all'ascissa della nuova porta l'ascissa del lato orizzontale 
            destro. Se invece è diversa, gli assegna l'ascissa del lato orizzontale sinistro. */
        else this -> x = (rand()%2)*(width-1);
        do {
            free = true;
            this -> y = rand()%(1+((height-1)-(3+1))-2)+2;
            for (int i=0; i<5 && free; i++) if (room.get (this -> y-1+i, this -> x) != '|') free = false;
        } while (!free);
    }
    draw();
}

Door:: Door (Room room, Door in, Door out): Entity (room, 'X'){
    int height, width;
    room.area (height, width);
    orientation = (in.inclination() == Vertical ? Horizontal : Vertical);
    /* Se vengono specificate le porte di ingresso e di uscita già presenti nella stanza, assegna all'orientamento della porta da generare 
    il valore opposto rispetto all'orientamento della porta di ingresso. In questo modo la nuova porta sarà eventualmente parallela alla 
    sola porta di uscita. */
    int y, x;
    bool free;
    out.position (y, x);
    if (orientation == Horizontal){
        if (orientation == out.inclination()) this -> y = (y == 0 ? height-1 : 0);
        else this -> y = (rand()%2)*(height-1);
        do {
            free = true;
            this -> x = rand()%(1+((width-1)-(6+1))-2)+2;
            for (int i=0; i<8 && free; i++) if (room.get (this -> y, this -> x-1+i) != '-') free = false;
        } while (!free);
    } else if (orientation == Vertical){
        if (orientation == out.inclination()) this -> x = (x == 0 ? width-1 : 0);
        else this -> x = (rand()%2)*(width-1);
        do {
            free = true;
            this -> y = rand()%(1+((height-1)-(3+1))-2)+2;
            for (int i=0; i<5 && free; i++) if (room.get (this -> y-1+i, this -> x) != '|') free = false;
        } while (!free);
    }
    /* Se la nuova porta deve essere generata con lo stesso orientamento della porta di uscita, e quindi le due porte di ingresso e di 
    uscita non sono parallele, posiziona la nuova porta sul lato parallelo opposto al lato su cui è collocata la porta di uscita. Nel caso 
    in cui le porte di ingresso e uscita siano invece parallele, genera la nuova porta senza restrizioni. */
    draw ();
}

void Door:: draw (){
    if (orientation == Horizontal) mvwhline (room.window(), y, x, icon, 6);
    // Se la porta è orientata in orizzontale, stampa una sequenza orizzontale di 6 caratteri corrispondenti alla sua icona: 'X' o ' '.
    else if (orientation == Vertical) mvwvline (room.window(), y, x, icon, 3);
    // Se la porta è orientata in verticale, stampa una sequenza verticale di 3 caratteri corrispondenti alla sua icona: 'X' o ' '.
    room.refresh();
}

void Door:: open(){
    icon = ' ';
    draw();
    /* Per fare in modo che la porta appaia graficamente aperta, sostituisce la sua icona 'X' con il carattere ' ' e la sovrascrive a 
    quella stampata in precedenza. */
}

Orientation Door:: inclination () {
    return orientation;
}
