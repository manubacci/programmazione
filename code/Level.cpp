#include "Level.hpp"

Level:: Level (){}
Level:: Level (int height, int width){
    room = Room (height, width);
    /* Quando non viene specificato nessun giocatore esistente, genera casualmente una nuova stanza sulla quale strutturare un livello che 
    corrisponda al punto di partenza della partita. */
    out = Door (room);
    out.open ();
    first = true;
    third = false; special = false;
    /* Genera all'interno della stanza associata al nuovo livello una porta di uscita, inizialmente chiusa, e la apre. Indica poi che nel 
    livello è presente un'unica porta. */
    bullets = NULL; enemies = NULL;
    generated = false;
    // Indica che nel livello non sono presenti muri, nemici, proiettili o artefatti.
}

Level:: Level (Player player, int height, int width){
    room = Room (height, width);
    /* Quando viene specificato un giocatore da trasferire, genera casualmente una nuova stanza sulla quale strutturare un livello che 
    corrisponda alla destinazione verso cui è diretta la porta che il giocatore sta attraversando. */
    bullets = NULL; enemies = NULL;
    first = false;  third = false; special = false;
    generated = false;
    /* Indica che nel livello non sono inizialmente presenti porte, muri, nemici, proiettili o artefatti. Verranno aggiunti in seguito a 
    seconda delle caratteristiche del livello. */
    if (player.passage().current == Special) {
        in = Door (room, player.passage().door, true);
        in.open ();
        /* Se la porta che il giocatore sta attraversando è diretta verso un livello speciale non ancora esplorato, genera nella stanza una
        nuova porta di ingresso, già aperta. Questa porta deve essere posizionata in modo speculare rispetto alla porta attraversata dal 
        giocatore per entrare nel livello, in modo dare un'impressione di continuità della mappa. */
        first = true;
        special = true;
        // Indica che nel livello è presente una sola porta, che corrisponde ad una porta speciale.
        int ya, xa;
        room.empty (ya, xa);
        artifact = Artifact (room, player, ya, xa);
        generated = true;
        /* Genera in una delle posizioni vuote all'interno della stanza un nuovo artefatto che non racchiude alcun potere. Indica poi che 
        nel livello è presente un artefatto con cui il giocatore non ha ancora interagito. */
    } else {
        wall = Wall (room);
        in = Door (room, player.passage().door, true);
        out = Door (room, in, false);
        /* Se la porta che il giocatore sta attraversando è diretta verso un livello successivo o secondario non ancora esplorato, genera 
        all'interno della stanza un muro e due porte: una d'ingresso e una d'uscita, inizialmente chiuse. */
        if (rand()%4 == 0) {
            secondary = Door (room, in, out);
            third = true;
            /* Con una probabilità del 25%, ossia di 1 su 4, aggiunge anche una porta secondaria. Nel caso, indica poi che nel livello sono 
            presenti 3 porte, di cui una è appunto secondaria. */
        }
        for (int i = 0; i < rand()%4+1; i++) create (enemies);
            // Crea casualmente un numero di nemici compreso fra 1 e 4.
    }
}

void Level:: draw (Player player){
    room.draw();
    if (!first) wall.draw();
    if (!(first && special)) out.draw();
    if (!first || (first && special)) in.draw();
    if (third) secondary.draw();
    if (generated) artifact.draw();
    /* Stampa interamente il contenuto del livello, ossia la stanza ad esso associata e tutte le entità posizionate al suo interno. In 
    particolare, stampa un muro se nella stanza è presente più di una porta, ossia se il livello non è speciale e non corrisponde al punto 
    di partenza. Stampa una porta di uscita se nella stanza è presente più di una porta o se nessuna della porte presenti è speciale, ossia 
    se il livello non è speciale. Stampa una porta di ingresso se nella stanza è presente più di una porta o se l'unica porta presente è 
    speciale, ossia se il livello non corrisponde al punto di partenza. Stampa una porta secondaria se nella stanza sono presenti 
    esattamente tre porte. Infine, stampa un'artefatto se viene indicato che nel livello è stato generato un artefatto con cui il giocatore 
    non ha ancora interagito. */
    player.draw();
    Enemies* ie = enemies;
    while (ie != NULL){
        ie -> enemy.draw();
        ie = ie -> next;
    }
    Bullets* ib = bullets;
    while (ib != NULL){
        ib -> bullet.draw();
        ib = ib -> next;
    }
}

Player Level:: initialize (){
    int yp, xp;
    room.empty (yp, xp);
    Player player (room, yp, xp);
    // Qualora non sia presente nessun giocatore, lo genera collocandolo in una delle posizioni vuote all'interno della stanza.
    return player;
}

Player Level:: initialize (Player player){
    int yp, xp;
    player.position (yp, xp);
    Door entrance;
    if (player.passage().current == Special) entrance = in;
    else if (player.passage().current == Next) entrance = in;
    else if (player.passage().current == Secondary) entrance = in;
    else if (player.passage().current == Previous && (player.passage().last == Special || player.passage().last == Secondary)) entrance = secondary;
    else if (player.passage().current == Previous) entrance = out;
    /* Qualora venga specificato un giocatore esistente, determina la porta attraverso cui esso entra nella stanza associata al nuovo 
    livello, in base ai valori assegnati agli attributi che ne specificano la transizione. In particolare, considera la porta di ingresso 
    se la porta che il giocatore sta attraversando è diretta verso un livello successivo, secondario o speciale. Considera la porta di 
    uscita se la destinazione corrisponde ad un livello precedente. Considera infine la porta secondaria se la porta che il giocatore sta 
    attraversando è diretta verso un livello precedente e l'ultima porta che ha utilizzato era diretta verso un livello speciale o 
    secondario. */
    int yd, xd;
    entrance.position (yd, xd);
    if (entrance.inclination() == Horizontal && yd == 0) {yp = yd+1; xp = xd+ player.passage().coordinate;}
    else if (entrance.inclination() == Horizontal && yd != 0) {yp = yd-1; xp = xd+ player.passage().coordinate;}
    /* Se la porta attraverso cui il giocatore entra nella stanza è orientata in orizzontale e la sua ordinata corrisponde all'ordinata
    del lato superiore, sposta il giocatore nella posizione indicata da questa ordinata, incrementata di 1, e dall'ascissa a cui la porta
    ha origine, incrementata del valore assegnato all'attributo della transizione che specifica il punto nel quale il giocatore sta 
    attraversando la porta. Se l'ordinata della porta è diversa de quella del lato superiore, considera il suo valore decrementato di 1. */
    else if (entrance.inclination() == Vertical && xd == 0) {yp = yd+ player.passage().coordinate; xp = xd+1;}
    else if (entrance.inclination() == Vertical && xd != 0) {yp = yd+ player.passage().coordinate; xp = xd-1;}
    /* Se la porta attraverso la quale il giocatore entra nella stanza è orientata in verticale, e la sua ascissa corrisponde all'ascissa 
    del lato sinistro, sposta il giocatore nella posizione indicate da questa ascissa, incrementata di 1, e dall'ordinata a cui la porta ha 
    origine, incrementata del valore assegnato all'apposito attributo della transizione. Se l'ascissa della porta è diversa de quella del 
    lato sinistro, considera il suo valore decrementato di 1. */
    if (room.get (yp, xp) != ' ') room.empty (yp, xp);
        /* Se la posizione in cui il giocatore deve essere spostato in seguito al suo trasferimento non è vuota, ossia se alle coordinate 
        corrispondenti è stampato un carattere diverso da ' ', determina casualmente la posizione del giocatore fra tutti gli spazi liberi 
        all'interno della stanza. */
    player.transfer (room, yp, xp);
    draw (player);
    // Trasferisce e stampa il giocatore nella stanza associata al nuovo livello.
    return player;
}

Player Level:: process (Player player){
    switch (room.input()){
        case 'W': case 'w': player.direction (North); player = handle (player); break;
        case 'S': case 's': player.direction (South); player = handle (player); break;
        case 'D': case 'd': player.direction (East); player = handle (player); break;
        case 'A': case 'a': player.direction (West); player = handle (player); break;
        /* Modifica la direzione del giocatore in base al valore associato all'Input immesso da tastiera. In particolare, se l'utente preme 
        il tasto "W" direziona il giocatore verso l'alto, con "S" verso il basso, con "D" verso destra e con "A" verso sinistra. Gestisce 
        poi il comportamento del giocatore in conseguenza a questa modifica. */
        case KEY_UP: if (player.state().cooldown == 0) {create (bullets, player, North); player.reset (Cooldown);} break;
        case KEY_DOWN: if (player.state().cooldown == 0) {create (bullets, player, South); player.reset (Cooldown);} break;
        case KEY_RIGHT: if (player.state().cooldown == 0) {create (bullets, player, East); player.reset (Cooldown);} break;
        case KEY_LEFT: if (player.state().cooldown == 0) {create (bullets, player, West); player.reset (Cooldown);} break;
        /* Quando l'utente preme uno dei 4 tasti freccia, se il valore assegnato all'attributo di stato del giocatore che ne indica il 
        tempo di ricarica ha raggiunto lo 0, lo ripristina e genera un nuovo proiettile all'interno della stanza, sparato dal giocatore 
        nella direzione indicata dalla freccia. */
    }
    player = handle (bullets, player);
    player = handle (enemies, player);
    /* Dopo aver gestito il comportamento del giocatore, prosegue in ordine con la gestione dei proiettili e dei nemici presenti 
    all'interno della stanza. Generalmente questa operazione viene eseguita ad ogni iterazione, a seconda della frequenza di aggiornamento 
    della grafica. */
    player.decrease (Cooldown);
    // Decrementa il valore dello stato associato al tempo di ricarica del giocatore.
    if (player.passage().current != Current) {room.clear(); room.refresh();}
        /* Se i valori assegnati agli attributi che descrivono la transizione del giocatore indicano che sta attraversando una porta 
        diretta verso un livello differente, e di conseguenza l'attributo che indica la direzione della porta che il giocatore sta 
        attraversando assume un valore non più neutro, cancella completamente il contenuto stampato all'interno della stanza associata al 
        livello attuale. */
    return player;
}

Player Level:: handle (Player player){
    int yp, xp;
    player.next (yp, xp);
    char step = room.get (yp, xp);
    /* Determina il carattere stampato alla posizione verso cui il giocatore si muoverà al suo passo successivo nella direzione 
    prestabilita. */
    if (step == 'A'){
        artifact.clear();
        generated = false;
        /* Se il passo successivo del giocatore si sovrappone alla posizione in cui è collocato un artefatto, ossia se alle coordinate che  
        il giocatore occuperà dopo lo spostamento è stampato il carattere 'A', elimina l'artefatto generato e indica che nella stanza non 
        è più presente alcun artefatto. Considera poi lo spazio in cui esso era collocato come se fosse vuoto, ossia come se al posto 
        dell'icona dell'artefatto 'A' ci fosse stampato il carattere ' '.*/
        if (!special && !third && rand()%2) {
            /* Se il livello non è speciale e al suo interno non sono già presenti tre porte, con una probabilità del 50%, ossia di 1 su 2, 
            quando il giocatore interagisce con l'artefatto genera una nuova porta, diretta verso un livello speciale. */
            secondary = Door (room, in, out);
            secondary.open();
            // Genera una porta secondaria già aperta, posizionandola in uno dei lati in cui non è presente nessun altra porta.
            third = true;
            special = true;
            // Indica che nel livello sono presenti tre porte, di cui una è speciale.
            player = artifact.upgrade (player);
         } else player = artifact.upgrade (player);
            /* Se nella stanza sono già presenti tre porte, o se la probabilità non è favorevole, potenzia le statistiche del 
            giocatore tenendo conto del fatto che nell'artefatto è racchiuso un qualche potere. */
    } else if (step == ' '){
        int height, width;
        room.area (height, width);
        if (yp == 0 || yp == height-1){
            /* Se il passo successivo del giocatore si sovrappone ad uno spazio vuoto, alle cui coordinate è stampato il carattere ' ', 
            verifica se l'ordinata della posizione che il giocatore occuperà dopo lo spostamento corrisponde all'ordinata di uno dei due 
            lati orizzontali della stanza, ossia se il giocatore sta attraversando una porta orizzontale. */
            if ((!first || !special) && out.inclination() == Horizontal){
                int yd, xd;
                out.position (yd, xd);
                if (yp == yd && xp >= xd && xp < xd+6)
                    player.transition (Next, out, xp-xd);
                    /* Se nella stanza è presente più di una porta o se nessuna delle porte presenti è speciale, allora esiste una porta di 
                    uscita. Se questa porta è orientata in orizzontale verifica se l'ordinata del giocatore corrisponde all'ordinata della 
                    porta e se alla sua ascissa è assegnato a un valore compreso fra l'ascissa a cui ha origine la porta e la stessa 
                    ascissa incrementata di 6, ossia verifica se le coordinate del giocatore corrispondono alle coordinate di uno dei 6  
                    spazi che compongono la porta. In caso affermativo, assegna agli attributi associati alla transizione del giocatore i 
                    valori che descrivono il suo trasferimento verso un livello successivo, che avviene attraversando la porta di uscita 
                    nel punto preciso individuato dall'ascissa del passo successivo del giocatore decrementata dell'ascissa a cui ha 
                    origine la porta. */
            } if ((!first || (first && special)) && in.inclination() == Horizontal){
                int yd, xd;
                in.position (yd, xd);
                if (yp == yd && xp >= xd && xp < xd+6)
                    player.transition (Previous, in, xp-xd);
                    /* Se nella stanza è presente più di una porta o se è presente un'unica porta speciale, allora esiste una porta di 
                    ingresso. Se questa porta è orientata in orizzontale e le coordinate del giocatore corrispondono alle coordinate di uno 
                    dei sei spazi che compongono la porta, assegna alla transizione del giocatore i valori che ne descrivono il 
                    trasferimento verso un livello precedente attraverso la porta di ingresso. */
            } if (third && secondary.inclination() == Horizontal){
                int yd, xd;
                secondary.position (yd, xd);
                if (yp == yd && xp >= xd && xp < xd+6)
                    player.transition ((special ?  Special : Secondary), secondary, xp-xd);
                    /* Se nella stanza sono presenti esattamente tre porte, allora esiste una porta secondaria. Se questa porta è orientata 
                    in orizzontale e le coordinate del giocatore corrispondono alle coordinate di uno dei sei spazi che compongono la porta, 
                    assegna alla transizione del giocatore i valori che ne descrivono il trasferimento verso un livello speciale o 
                    secondario attraverso la porta secondaria. In particolare, se sono presenti tre porte di cui una è speciale, allora la 
                    destinazione corrisponde ad un livello speciale, altrimenti ad un livello secondario. */
            }
        } if (xp == 0 || xp == width-1){
            /* Verifica se l'ascissa della posizione che il giocatore occuperà dopo lo spostamento corrisponde all'ascissa di uno dei due 
            lati verticali della stanza, ossia se il giocatore sta attraversando una porta verticale. */
            if ((!first || !special) && out.inclination() == Vertical){
                int yd, xd;
                out.position (yd, xd);
                if (xp == xd && yp >= yd && yp < yd+3)
                    player.transition (Next, out, yp-yd);
                    /* Se la porta di uscita esiste ed è orientata in verticale, verifica se l'ascissa del giocatore corrisponde 
                    all'ascissa della porta e se alla sua ordinata è assegnato a un valore compreso fra l'ordinata a cui ha origine la 
                    porta e la stessa ordinata incrementata di 3, ossia verifica se le coordinate del giocatore corrispondono alle 
                    coordinate di uno dei 3 spazi che compongono la porta. In caso affermativo, assegna alla transizione del giocatore i 
                    valori che ne descrivono il trasferimento verso un livello successivo, che avviene attraversando la porta di uscita nel 
                    punto preciso individuato dall'ordinata del passo successivo del giocatore decrementata dell'ordinata a cui ha origine 
                    la porta. */
            } if ((!first || (first && special)) && in.inclination() == Vertical){
                int yd, xd;
                in.position (yd, xd);
                if (xp == xd && yp >= yd && yp < yd+3)
                    player.transition (Previous, in, yp-yd);
            } if (third && secondary.inclination() == Vertical){
                int yd, xd;
                secondary.position (yd, xd);
                if (xp == xd && yp >= yd && yp < yd+3)
                    player.transition ((special ? Special : Secondary), secondary, yp-yd);
            }
        }
    }
    player.move ();
    /* Sposta il giocatore nella posizione indicata dalle coordinate che ne specificano il passo successivo nella direzione prestabilita,
    qualora il movimento sia possibile. */
    return player;
}

void Level:: create (Bullets* head, Player player, Direction direction){
    int yp, xp;
    player.next (yp, xp, direction);
    if (room.get (yp, xp) == ' ' || room.get (yp, xp) == '.'){
        if (head == NULL){
            head = new Bullets;
            head -> bullet = Bullet (room, player, direction, yp, xp);
            head -> next = NULL;
        } else {
            Bullets* iteration = head;
            while (iteration -> next != NULL)
                iteration = iteration -> next;
            iteration -> next = new Bullets;
            iteration = iteration -> next;
            iteration -> bullet = Bullet (room, player, direction, yp, xp);
            iteration -> next = NULL;
        }
        /* Se alle coordinate successive alla posizione del giocatore nella direzione indicata è presente uno spazio vuoto o al più già 
        occupato da un altro proiettile, ossia se vi è stampato il carattere ' ' o '.', aggiunge in coda all'apposita lista un nuovo 
        proiettile, sparato dal giocatore nella direzione indicata e generato alle coordinate specificate. */
    }
    bullets = head;
}

void Level:: create (Bullets* head, Enemy enemy, Direction direction){
    int yb, xb;
    enemy.next (yb, xb, direction);
    if ((direction == North || direction == NorthEast || direction == NorthWest) && enemy.statistics().extention == Top) yb--;
        /* Se l'estensione del nemico è posizionata sopra di esso e il proiettile deve essere sparato con una traiettoria verticale o
        diagonale verso l'alto, decrementa di 1 il valore dell'ordinata in cui verrà generato. */
    else if ((direction == South || direction == SouthEast || direction == SouthWest) && enemy.statistics().extention == Bottom) yb++;
    else if ((direction == East || direction == NorthEast || direction == SouthEast) && enemy.statistics().extention == Right) xb++;
    else if ((direction == West || direction == NorthWest || direction == SouthWest) && enemy.statistics().extention == Left) xb--;
    if (room.get (yb, xb) == ' ' || room.get (yb, xb) == '.'){
        if (head == NULL){
            head = new Bullets;
            head -> bullet = Bullet (room, enemy, direction, yb, xb);
            head -> next = NULL;
        } else {
            Bullets* iteration = head;
            while (iteration -> next != NULL)
                iteration = iteration -> next;
            iteration -> next = new Bullets;
            iteration = iteration -> next;
            iteration -> bullet = Bullet (room, enemy, direction, yb, xb);
            iteration -> next = NULL;
        }
        /* Se alle coordinate successive alla posizione del nemico nella direzione indicata è presente uno spazio vuoto o al più già
        occupato da un altro proiettile, ossia se vi è stampato il carattere ' ' o '.', aggiunge in coda all'apposita lista un nuovo
        proiettile, sparato dal nemico nella direzione indicata e generato alle coordinate specificate. Se la posizione in cui deve
        essere generato il proiettile interferisce con l'estensione del nemico, incrementa o decrementa opportunamente di 1 queste 
        coordinate in modo da dare l'impressione che il proiettile venga sparato dall'estensione stessa. */
    }
    bullets = head;
}

void Level:: erase (Bullets* head, Bullets* target){
    while (head != NULL && head == target){
        Bullets* temporary = head;
        head = head -> next;
        temporary -> bullet.clear();
        delete (temporary);
    }
    Bullets* iteration = head;
    while (iteration != NULL && iteration -> next != NULL){
        if (iteration -> next == target) {
            Bullets* temporary = iteration -> next;
            iteration -> next = iteration -> next -> next;
            temporary -> bullet.clear();
            delete (temporary);
        } else iteration = iteration -> next;
    }
    /* Se scorrendo l'apposita lista dalla testa alla coda trova un proiettile che corrisponde alle descrizione indicata, lo elimina dalla 
    lista e lo cancella dalla stanza. */
    bullets = head;
}

Player Level:: handle (Bullets* iteration, Player player){
    while (iteration != NULL){
        iteration -> bullet.decrease (Speed);
        /* Per ciascuno dei proiettili elencati nell'apposita lista, decrementa ad ogni iterazione il valore dello stato associato alla sua 
        velocità. */
        if (iteration -> bullet.state().speed <= 0){
            iteration -> bullet.reset (Speed);
            /* Quando, scorrendo la lista dalla testa alla coda, il valore assegnato allo stato che indica la velocità del proiettile 
            considerato ha raggiunto lo 0, ripristina l'attributo e procede a gestire il movimento del proiettile corrispondente. */
            int yb, xb;
            iteration -> bullet.next (yb, xb);
            char step = room.get (yb, xb);
            /* Determina il carattere stampato alla posizione verso cui il proiettile si muoverà al suo passo successivo nella direzione 
            prestabilita. */
            iteration -> bullet.decrease (Range);
            // Decrementa ad ogni iterazione lo stato associato alla gittata del proiettile.
            if (iteration -> bullet.shooter() == Oppenent && step == 'P'){
                player.hit (iteration -> bullet.statistics().damage);
                Bullets* temporary = iteration -> next;
                erase (bullets, iteration);
                iteration = temporary;
                /* Se il proiettile è stato sparato da un nemico e il suo passo successivo si sovrappone alla posizione in cui è collocato  
                il giocatore, decrementa lo stato associato ai punti vita del giocatore di una quantità pari al potenziale di danno del 
                proiettile, ossia del nemico che lo ha sparato. Elimina poi il proiettile dalla lista e passa alla gestione dell'elemento 
                successivo. */
            } else if (iteration -> bullet.shooter() == User && step == '@'){
                player = erase (enemies, iteration -> bullet, player);
                Bullets* temporary = iteration -> next;
                erase (bullets, iteration);
                iteration = temporary;
                /* Se il proiettile è stato sparato dal giocatore e il suo passo successivo si sovrappone alla posizione in cui è presente 
                un nemico, procede con la gestione della salute del nemico bersagliato, e con la sua eventuale eliminazione. Elimina poi il 
                proiettile dalla lista e passa all'elemento successivo. */
            } else if ((step != ' ' && step != '.') || iteration -> bullet.state().range <= 0){
                iteration -> bullet.reset (Range);
                Bullets* temporary = iteration -> next;
                erase (bullets, iteration);
                iteration = temporary;
                /* Se il passo successivo del proiettile si sovrappone ad uno spazio vuoto o al più occupato da un altro proiettile, ossia 
                alle cui coordinate è stampato il carattere ' ' o '.', e se lo stato che indica la gittata del proiettile ha raggiunto lo 
                0, ripristina l'attributo per poi eliminare il proiettile corrispondente dalla lista e passare all'elemento successivo. */
            } else {
                iteration -> bullet.move ();
                iteration = iteration -> next;
                /* Se non accade nessuna delle opzioni precedenti, sposta il proiettile nella posizione indicata dalle coordinate che ne 
                specificano il passo successivo nella direzione prestabilita, qualora il movimento sia possibile. Passa poi alla gestione 
                del movimento dell'elemento successivo della lista. */
            }
        } else iteration = iteration -> next;
            /* Se il valore assegnato allo stato che indica la velocità del proiettile considerato non ha ancora raggiunto lo 0, lo ignora 
            passando all'elemento successivo della lista. */
    }
    return player;
}

void Level:: create (Enemies* head){
    int ye, xe;
    room.empty (ye, xe);
    if (head == NULL){
        head = new Enemies;
        head -> enemy = Enemy (room, ye, xe);
        head -> next = NULL;
    } else {
        Enemies* iteration = head;
        while (iteration != NULL && iteration -> next != NULL)
            iteration = iteration -> next;
        iteration -> next = new Enemies;
        iteration = iteration -> next;
        iteration -> enemy = Enemy (room, ye, xe);
        iteration -> next = NULL;
    }
    enemies = head;
    /* Aggiunge in coda all'apposita lista un nuovo nemico, generato in una posizione determinata casualmente fra tutti gli spazi liberi 
    all'interno della stanza. */
}

Player Level:: erase (Enemies* head, Bullet bullet, Player player){
    int yb, xb;
    bullet.next (yb, xb);

    while (head != NULL){
        int ye, xe, yi, xi;
        head -> enemy.position (ye, xe);
        head -> enemy.extention (yi, xi);
        if ((ye == yb && xe == xb) || (yi == yb && xi == xb)){
            head -> enemy.hit (player.statistics().damage);
            if (head -> enemy.state().health <= 0){
                player.score (head -> enemy.statistics().points);
                Enemies* temporary = head;
                head = head -> next;
                temporary -> enemy.clear();
                delete (temporary);
            } else break;
        } else break;
    }
    if (head != NULL) {
        Enemies* iteration = head;
        while (iteration != NULL && iteration -> next != NULL){
            int ye, xe, yi, xi;
            iteration -> next -> enemy.position (ye, xe);
            iteration -> next -> enemy.extention (yi, xi);
            if ((ye == yb && xe == xb) || (yi == yb && xi == xb)){
                iteration -> next -> enemy.hit (player.statistics().damage);
                /* Se, scorrendo l'apposita lista dalla testa alla coda, le coordinate a cui è posizionato il nemico considerato o la sua 
                estensione coincidono con il passo successivo del proiettile indicato, decrementa lo stato associato ai punti vita del 
                nemico di una quantità pari al potenziale di danno del proiettile, ossia del giocatore che lo ha sparato. */
                if (iteration -> next -> enemy.state().health <= 0){
                    player.score (iteration -> next -> enemy.statistics().points);
                    Enemies* temporary = iteration -> next;
                    iteration -> next = iteration -> next -> next;
                    temporary -> enemy.clear();
                    delete (temporary);
                    /* Se lo stato che indica i punti vita del nemico considerato ha raggiunto lo 0, incrementa la statistica del giocatore 
                    associata ai suoi punti esperienza di una quantità corrispondente al valore assegnato alla stessa statistica del nemico.
                    Elimina poi il nemico dalla lista e lo cancella dalla stanza. Passa infine ad esaminare l'elemento successivo. */
                } else iteration = iteration -> next;
            } else iteration = iteration -> next;
                /* Se le coordinate non corrispondono o se il valore assegnato allo stato che indica la vita del nemico considerato non ha 
                ancora raggiunto lo 0, lo ignora passando all'elemento successivo della lista. */
        }
        /* Nonostante non sia realmente necessario, questa operazione viene eseguita su tutti gli elementi della lista, anche se è già 
        stato trovato il nemico da eliminare. Tenendo anche in considerazione del fatto che dopo aver eliminato un nemico da una 
        determinata posizione della lista occorre controllare nuovamente la stessa posizione, che verrà occupata dal suo successore. */
    } else {
        if (!first || !special) out.open();
        if (!first || (first && special)) in.open();
        if (third) secondary.open();
        // Se nella lista non è più presente nessun nemico, apre tutte le porte posizionate all'interno della stanza.
        if (rand()%3 == 0) {
            artifact = Artifact (room, player, yb, xb, true);
            generated = true;
        }
        /* Con una probabilità di circa 33%, ossia di 1 su 3, genera alla posizione dell'ultimo nemico eliminato un artefatto che racchiude 
        un potere. Indica poi che nella stanza è presente un artefatto con cui l'utente non ha ancora interagito. */
    }
    enemies = head;
    return player;
}

Player Level:: handle (Enemies* iteration, Player player){
    while (iteration != NULL){
        int ye, xe, yp, xp;
        iteration -> enemy.position (ye, xe);
        player.position (yp, xp);
        bool trajectory = false;
        for (int i = 0; i <= iteration -> enemy.statistics().sight && !trajectory; i++){
        for (int j = 0; j <= iteration -> enemy.statistics().sight*2 && !trajectory; j++){
            if ((yp == ye +i && xp == xe +j) ||
                (yp == ye +i && xp == xe -j) ||
                (yp == ye -i && xp == xe +j) ||
                (yp == ye -i && xp == xe -j)) trajectory = true;
        }}
        /* Scorrendo l'apposita lista dalla testa alla coda, verifica per ciascuno dei nemici se nei suoi dintorni è presente il giocatore, 
        ad una distanza inferiore al valore assegnato alla statistica associata alla sua area visiva. In particolare, verifica se le 
        coordinate del giocatore corrispondono a una delle posizioni individuate dall'ascissa del nemico incrementata o decrementata di un 
        valore compreso fra 0 e il doppio della sua vista e dalla sua ordinata incrementata o decrementata di un valore compreso fra 0 e la 
        sua vista. In caso affermativo, indica che il giocatore è nella traiettoria del nemico. */
        /*
               x-        x+
               4 3 2 1 0 1 2 3 4                 schema:
        + - - - - - - - - - - - - - - - +
        |                               |        vista verticale: 2
        |                               |            orizzontale: 2*2 = 4
        |       X X X X X X X X X       |  2  y-
        |       X X X X X X X X X       |  1
        |       X X X X @ X X X X       |  0
        |       X X X X X X X X X       |  1  y+
        |       X X X X X X X X X       |  2
        |                               |
        |                               |
        + - - - - - - - - - - - - - - - +
        */
        if (trajectory){
            bool obstacle = false;
            while (!obstacle && !(ye == yp && xe == xp)){
                if (yp > ye && xp < xe) {ye++; xe--;}
                else if (yp < ye && xp > xe) {ye--; xe++;}
                else if (yp < ye && xp < xe) {ye--; xe--;}
                else if (yp > ye && xp > xe) {ye++; xe++;}
                else if (yp < ye && xp == xe) ye--;
                else if (yp > ye && xp == xe) ye++;
                else if (yp == ye && xp > xe) xe++;
                else if (yp == ye && xp < xe) xe--;
                if (room.get (ye, xe) == '|' || room.get (ye, xe) == '-' || room.get (ye, xe) == '+') obstacle = true;
            }
            /* Se il giocatore è all'interno dell'area visiva del nemico, verifica se la traiettoria che li collega è intercettata da un 
            qualche ostacolo. A partire dalla posizione occupata dal nemico traccia un percorso in modo che ogni passo corrisponda alle 
            coordinate del passo precedente incrementate o decrementate di 1, a seconda della sua posizione in relazione a quella del 
            giocatore. Per ogni passo, verifica se alle corrispondenti coordinate è presente un muro verticale o orizzontale, ossia se vi è 
            stampato uno dei caratteri '|', '-', '+'. In caso affermativo, indica che è presente un'ostacolo che impedisce al nemico di 
            vedere il giocatore, nonostante esso sia all'interno del suo raggio visivo. Ripete questa operazione fino a quando il percorso 
            non raggiunge la posizione del giocatore. */
            if (!obstacle){
                iteration -> enemy.decrease (Speed);
                /* Se non è presente nessun ostacolo che impedisce al nemico di vedere il giocatore, decrementa ad ogni iterazione il  
                valore dello stato associato alla velocità del nemico. */
                if (iteration -> enemy.state().speed <= 0){
                    iteration -> enemy.reset (Speed);
                    /* Quando il valore assegnato allo stato che indica la velocità del nemico ha raggiunto lo 0, ripristina l'attributo e 
                    procede a gestire il movimento del nemico corrispondente. */
                    iteration -> enemy.position (ye, xe);
                    if (iteration -> enemy.statistics().movement == Teleport){
                        iteration -> enemy.direction (Point);
                        int yi, xi;
                        do {
                            room.empty (ye, xe);
                            yi = ye; xi = xe;
                            switch (iteration -> enemy.statistics().extention){
                                case Top: yi --; break;
                                case Bottom: yi ++; break;
                                case Right: xi ++; break;
                                case Left: xi --; break;
                                case Alone: break;
                            }
                        } while (room.get (yi, xi) != ' ');
                        iteration -> enemy.move (ye, xe);
                        /* Se il valore della statistica associata allo stile di movimento del nemico indica che si muove 
                        teletrasportandosi, determina casualmente una delle posizioni vuote all'interno della stanza in cui collocarlo. 
                        Verifica poi se spostando il nemico nella posizione generata la sua estensione, qualora esista, si sovrappone ad 
                        uno spazio vuoto, in cui è stampato il carattere ' '. In caso negativo, ripete questa operazione fino a quando non 
                        trova una posizione adeguata. Muove infine il nemico verso le coordinate determinate. */
                    } else if (iteration -> enemy.statistics().movement == Casual){
                        switch (rand()%9){
                            case 0: iteration -> enemy.direction (North); break;
                            case 1: iteration -> enemy.direction (South); break;
                            case 2: iteration -> enemy.direction (East); break;
                            case 3: iteration -> enemy.direction (West); break;
                            case 4: iteration -> enemy.direction (NorthEast); break;
                            case 5: iteration -> enemy.direction (NorthWest); break;
                            case 6: iteration -> enemy.direction (SouthEast); break;
                            case 7: iteration -> enemy.direction (SouthWest); break;
                            case 8: iteration -> enemy.direction (Point); break;
                            default: break;
                        }
                        iteration -> enemy.move ();
                        /* Se il valore della statistica associata allo stile di movimento del nemico indica che si muove casualmente, 
                        modifica la direzione del nemico ad una direzione determinata casualmente. Muove poi il nemico verso la posizione 
                        successiva nella direzione considerata. */
                    } else if (iteration -> enemy.statistics().movement == Following){ 
                        if (yp < ye && xp > xe) iteration -> enemy.direction (NorthEast);
                        else if (yp < ye && xp < xe) iteration -> enemy.direction (NorthWest);
                        else if (yp > ye && xp > xe) iteration -> enemy.direction (SouthEast);
                        else if (yp > ye && xp < xe) iteration -> enemy.direction (SouthWest);
                        else if (yp > ye && xp == xe) iteration -> enemy.direction (South);
                        else if (yp < ye && xp == xe) iteration -> enemy.direction (North);
                        else if (yp == ye && xp > xe) iteration -> enemy.direction (East);
                        else if (yp == ye && xp < xe) iteration -> enemy.direction (West);
                        iteration -> enemy.move ();
                        /* Se il valore della statistica associata allo stile di movimento del nemico indica che si muove seguendo il 
                        giocatore, modifica la direzione del nemico a seconda della sua posizione in relazione a quella del giocatore. 
                        Muove poi il nemico verso la posizione successiva nella direzione considerata. */
                    }
                }
                iteration -> enemy.decrease (Cooldown);
                // Decrementa ad ogni iterazione il valore dello stato associato al tempo di caricamento del nemico considerato.
                if (iteration -> enemy.state().cooldown == 0){
                    iteration -> enemy.reset (Cooldown);
                    /* Quando il valore assegnato allo stato che indica il tempo di caricamento del nemico ha raggiunto lo 0, ripristina 
                    l'attributo e procede a gestire l'attacco del nemico corrispondente. */
                    iteration -> enemy.position (ye, xe);
                    if (iteration -> enemy.statistics().attack == Plus){
                        create (bullets, iteration -> enemy, North);
                        create (bullets, iteration -> enemy, South);
                        create (bullets, iteration -> enemy, East);
                        create (bullets, iteration -> enemy, West);
                        /* Se il valore della statistica associata allo stile di attacco del nemico indica che attacca sparando quattro 
                        proiettili le cui traiettorie formano graficamente un più, genera 4 nuovi proiettili, sparati dal nemico nelle 
                        quattro direzioni orizzontali e verticali: Nord, Sud, Est e Ovest. */
                    }
                    else if (iteration -> enemy.statistics().attack == Cross){
                        create (bullets, iteration -> enemy, NorthEast);
                        create (bullets, iteration -> enemy, NorthWest);
                        create (bullets, iteration -> enemy, SouthEast);
                        create (bullets, iteration -> enemy, SouthWest);
                        /* Se il valore della statistica associata allo stile di attacco del nemico indica che attacca sparando quattro 
                        proiettili le cui traiettorie formano graficamente una croce, genera 4 nuovi proiettili, sparati dal nemico nelle 
                        quattro direzioni diagonali: Nord-Est, Nord-Ovest, Sud-Est e Sud-Ovest. */
                    }
                    else if (iteration -> enemy.statistics().attack == Directional){
                        if (yp < ye && xp > xe) create (bullets, iteration -> enemy, NorthEast);
                        else if (yp < ye && xp < xe) create (bullets, iteration -> enemy, NorthWest);
                        else if (yp > ye && xp > xe) create (bullets, iteration -> enemy, SouthEast);
                        else if (yp > ye && xp < xe) create (bullets, iteration -> enemy, SouthWest);
                        else if (yp > ye && xp == xe) create (bullets, iteration -> enemy, South);
                        else if (yp < ye && xp == xe) create (bullets, iteration -> enemy, North);
                        else if (yp == ye && xp > xe) create (bullets, iteration -> enemy, East);
                        else if (yp == ye && xp < xe) create (bullets, iteration -> enemy, West);
                        /* Se il valore della statistica associata allo stile di attacco del nemico indica che attacca mirando al 
                        giocatore, genera un nuovo proiettile la cui direzione dipende della posizione del nemico in relazione a quella del 
                        giocatore. */
                    }
                    int yi, xi;
                    iteration -> enemy.extention (yi, xi);
                    for (int i = 0; i <= 2 + abs (xe - xi); i++){
                    for (int j = 0; j <= 2 + abs (ye - yi); j++){
                        if (room.get (ye - (yi < ye ? 2 : 1) +j, xe - (xi < xe ? 2 : 1) +i) == 'P')
                            player.hit (iteration -> enemy.statistics().damage);
                    }}
                    /* Verifica se il nemico considerato è a contatto con il giocatore ed è quindi in grado di sferrargli un attacco corpo 
                    a corpo, ossia se le coordinate del giocatore corrispondono a una delle posizioni individuate dalle coordinate del 
                    nemico decrementate di 2, se l'ordinata del nemico è minore dell'ordinata della sua estensione, o di 1, in caso 
                    contrario, incrementate ulteriormente di un valore compreso fra 0 e 2 sommato al valore assoluto della sottrazione fra 
                    l'ascissa o l'ordinata dell'estensione e l'ordinata o l'ascissa del nemico, ossia fra 0 e 3 se il nemico e la sua 
                    estensione hanno ascisse o ordinate diverse o fra 0 e 2 in caso contrario. */
                    /*
                             xe-1+
                             0 1 2 3
                    + - - - - - - - - - - - +        schema:
                    |                       |        xe < xi, ye = yi
                    |                       |
                    |        X X X X        |  0 ye-1+
                    |        X @ @ X        |  1
                    |        X X X X        |  2
                    |                       |
                    |                       |
                    + - - - - - - - - - - - +

                             xe-1+
                             0 1 2
                    + - - - - - - - - - - - +        schema:
                    |                       |        xe = xi, ye > yi
                    |                       |
                    |        X X X          |  0 ye-2+
                    |        X @ X          |  1
                    |        X @ X          |  2
                    |        X X X          |  3
                    |                       |
                    + - - - - - - - - - - - +
                    */
                }
            }
        }
        iteration = iteration -> next;
        /* Se nessuno dei valori assegnati agli stati che indicano la velocità e il tempo di caricamento del nemico considerato ha 
        raggiunto lo 0, se il giocatore non si trova all'interno dell'area visiva del nemico o se è presente un ostacolo che impedisce al 
        nemico di vedere il giocatore, ignora il nemico considerato e passa all'elemento successivo della lista. */
    }
    return player;
}