#include "Entity.hpp"

Entity:: Entity (){}
Entity:: Entity (Room room, char icon, int y, int x){
    this -> x = x;
    this -> y = y;
    this -> icon = icon;
    this -> room = room;
    /* Inizializza le caratteristiche principali dell'entità, ossia la finestra a cui appartiene, la sua icona e la sua posizione in  
    relazione al sistema della stanza nella quale è collocata, a seconda dei valori indicati dalla classe derivata che utilizza questa 
    classe base. */
    initialize ();
    if (x != 0 && y != 0) draw();
    /* Stampa l'entità all'interno della stanza solo nel caso in cui sia stata generata dalla classe derivata in una posizione diversa 
    rispetto a quella indicata dalle coordinare Standard: (0,0). */
}

void Entity:: initialize (){
    stats.health = 0;
    stats.movement = Stop;
    stats.speed = 0;
    stats.attack = Harmless;
    stats.cooldown = 0;
    stats.damage = 0;
    stats.shots = 0;
    stats.range = 0;
    stats.sight = 0;
    stats.points = 0;
    stats.direction = Point;
    stats.extention = Alone;
    status = stats;
    /* Assegna agli attributi che specificano le statistiche e lo stato attuale dell'entità dei valori neutri, che non implicano alcun
    movimento o danno. */
    
}

void Entity:: position (int& y, int& x){
    y = this -> y;
    x = this -> x;
    /* Assegna alle variabili passate per riferimento i valori corrispondenti alle coordinate dell'entità rispetto alla sua posizione nel 
    sistema della stanza. */
}

void Entity:: extention (int& y, int& x, Direction direction){
    y = this -> y; 
    x = this -> x;
    switch (stats.extention){
        case Top: y --; break;
        case Bottom: y ++; break;
        case Right: x ++; break;
        case Left: x --; break;
        case Alone: break;
    }
    /* Determina le coordinate dell'estensione dell'entità, incrementando o decrementando le coordinate dell'entità corrispondente a 
    seconda della sua posizione in relazione a quella dell'estensione. */
    if (direction != Point){
        switch (direction){
        case North: y --; break;
        case South: y ++; break;
        case East: x ++; break;
        case West: x --; break;
        case NorthEast: y --; x ++; break;
        case NorthWest: y --; x --; break;
        case SouthEast: y ++; x ++; break;
        case SouthWest: y ++; x --; break;
        default: break;
        }
        /* Determina le coordinate della posizione nella quale avanzerà l'estensione dell'entità al suo passo successivo, incrementando o 
        decrementando le sue coordinate attuali di 1 a seconda della direzione specificata. */
    }
    /* Assegna alle variabili passate per riferimento i valori corrispondenti alle coordinate dell'estensione dell'entità o alle coordinate 
    del suo passo successivo nella direzione di movimento indicata, qualora sia specificata. */
}

void Entity:: next (int& y, int& x, Direction direction){
    y = this -> y; 
    x = this -> x;
    switch (direction != Point ? direction : stats.direction){
        case North: y --; break;
        case South: y ++; break;
        case East: x ++; break;
        case West: x --; break;
        case NorthEast: y --; x ++; break;
        case NorthWest: y --; x --; break;
        case SouthEast: y ++; x ++; break;
        case SouthWest: y ++; x --; break;
        case Point: break;
    }
    /* Determina le coordinate della posizione nella quale avanzerà l'entità al suo passo successivo, incrementando o decrementando le sue 
    coordinate attuali di 1 a seconda della direzione specificata dall'apposita statistica o dal valore indicato. Assegna poi alle 
    variabili passate per riferimento i valori corrispondenti alle coordinate del passo successivo dell'entità in questa direzione. */
}

void Entity:: clear (){
    if (room.get (y, x) == icon) mvwaddch (room.window(), y, x, ' ');
    /* Se nella posizione indicata è stampato il carattere corrispondente all'icona dell'entità, lo cancella sovrascrivendolo con un 
    carattere bianco: ' '. */
    if (stats.extention != Alone){
        int ye, xe;
        extention (ye, xe);
        if (room.get (ye, xe) == icon) mvwaddch (room.window(), ye, xe, ' ');
        // Se è presente un'estensione, ripete lo stesso procedimento anche alle sue coordinate.
    }
    room.refresh();
}

void Entity:: draw (){
    mvwaddch (room.window(), y, x, icon);
    // Stampa alle coordinate indicate il carattere corrispondente all'icona dell'entità.
    if (stats.extention != Alone){
        int ye, xe;
        extention (ye, xe);
        mvwaddch (room.window(), ye, xe, icon);
        // Se è presente un'estensione, ripete lo stesso procedimento anche alle sue coordinate.
    }
    room.refresh();
}

void Entity:: move (){
    int y, x;
    next (y, x);
    /* Se non viene indicata una precisa coppia di coordinate verso cui muovere l'entità al suo prossimo passo, la sposta nella posizione 
    immediatamente successiva alla sua posizione attuale, a seconda della direzione specificata dall'apposita statistica. */
    char step = room.get (y, x);
    // Determina il carattere stampato alla posizione verso cui l'entità si muoverà al suo passo successivo nella direzione prestabilita.
    char duplicate = step;
    if (stats.extention != Alone){
        int yi, xi;
        extention (yi, xi, stats.direction);
        duplicate = room.get (yi, xi);
        /* Se l'apposita statistica indica che all'entità è associata un'estensione, determina anche il carattere stampato alla posizione 
        verso cui l'estensione si muoverà al suo passo successivo nella stessa direzione dell'entità. */
        if (step == icon || duplicate == icon){
            extention (yi, xi);
            if (y == yi && x == xi) step = ' ';
            extention (yi, xi, stats.direction);
            if (yi == this -> y && xi == this -> x) duplicate = ' ';
            /* Se il passo successivo dell'entità si sovrappone alla posizione attualmente occupata dalla sua estensione, considera lo 
            spazio in cui essa è collocata come se fosse vuoto, ossia come se al posto dell'icona dell'entità ci fosse stampato il 
            carattere ' '. Lo stesso ragionamento vale anche se la posizione verso cui l'estensione si muoverà al suo passo successivo è 
            occupata dall'entità. */
        }
    }
    if ((step == ' ' && duplicate == ' ') || (step == '.' && icon == '.')){
        move (y, x);
        /* Sposta l'entità e la sua estensione verso la posizione determinata solo nel caso in cui i loro passi successivi si sovrappongano 
        unicamente a degli spazi vuoti, alle cui coordinate è stampato il carattere ' '. Se l'entità rappresenta un proiettile, ossia se 
        la sua icona è '.', essa si può muovere anche nel caso particolare in cui la posizione verso cui si sposta muovendo il suo passo 
        successivo è già occupata da un altro proiettile. */
    }
}

void Entity:: move (int y, int x){
    /* Se viene indicata una precisa coppia di coordinate verso cui muovere l'entità al suo prossimo passo, la sposta nella posizione 
    specificata da queste coordinate. */
    clear();
    this -> y = y;
    this -> x = x;
    draw();
    /* Per muovere l'entità, cancella la sua icona dalla posizione in cui è collocata attualmente e la stampa nuovamente alle coordinate 
    della posizione che occupa al suo passo successivo. Lo stesso procedimento vale anche per la sua estensione, qualora esista. */
}

void Entity:: decrease (Variant attribute){
    if (attribute == Cooldown && status.cooldown != 0) status.cooldown--;
    else if (attribute == Shots && status.shots != 0) status.shots--;
    else if (attribute == Speed && status.speed > 0) status.speed -= (stats.direction == East || stats.direction == West ? 2 : 1);
    else if (attribute == Range && status.range > 0) status.range -= (stats.direction == East || stats.direction == West ? 1 : 2);
    /* Se il valore assegnato l'attributo di stato indicato non è maggiore di 0, lo decrementa di 1. Nel caso in cui l'attributo indicato 
    corrisponde alla velocità del movimento e la direzione dell'entità è orizzontale, ossia Est o Ovest, decrementa il valore ad esso 
    assegnato di 2 anziché di 1. Lo stesso ragionamento vale anche nel caso in cui l'attributo indicato corrisponda alla gittata dei 
    proiettili sparati e la direzione dell'entità non sia orizzontale. Generalmente questa operazione viene eseguita ad ogni iterazione, a 
    seconda della frequenza di aggiornamento della grafica. */
}

void Entity:: reset (Variant attribute){
    if (attribute == Cooldown && status.cooldown == 0) status.cooldown = stats.cooldown;
    else if (attribute == Shots && status.shots == 0) status.shots = stats.shots;
    else if (attribute == Speed && status.speed <= 0) status.speed = stats.speed;
    else if (attribute == Range && status.range <= 0) status.range = stats.range;
    else if (attribute == Health) status.health = stats.health;
    /* Se il valore assegnato all'attributo di stato indicato ha raggiunto il limite sotto al quale non può più essere decrementato, ossia 
    0, lo ripristina al valore della corrispondente statistica. Se richiesto, ripristina lo stato associato ai punti vita dell'entità a 
    prescindere dal suo valore. */
}

void Entity:: direction (Direction aim){
   stats.direction = aim;
    // Modifica la direzione dell'entità con la nuova direzione indicata.
}

void Entity:: hit (int damage){
   status.health -= damage;
   /* Sottrae allo stato associato ai punti vita dell'entità il potenziale di danno subito. Generalmente questa operazione viene eseguita 
   ogni volta che l'entità è bersagliata da un attacco. */
}

void Entity:: score (int points){
    stats.points += points;
    /* Somma la quantità indicata alla statistica associata ai punti esperienza dell'entità. Generalmente questa operazione viene eseguita 
    ogni volta che il giocatore sconfigge un nemico. */
}

Statistics Entity:: statistics (){
    return stats;
}
Statistics Entity:: state (){
    return status;
}