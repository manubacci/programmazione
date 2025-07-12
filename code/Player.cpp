#include "Player.hpp"

Player:: Player (){}
Player:: Player (Room room, int y, int x): Entity (room, 'P', y, x){
// Il giocatore è implementato come un'entità, la cui icona è 'P' e la cui posizione è specificata al momento dell'inizializzazione.
    stats.health = 30;
    stats.movement = Stop;
    stats.speed = 0;
    stats.attack = Harmless;
    stats.damage = 5;
    stats.direction = Point;
    stats.points = 0;
    stats.extention = Alone;
    status = stats;
    stats.cooldown = 160;
    stats.range = 10;
    stats.shots = 18;
    destination.current = Current;
    /* Assegna alle statistiche del giocatore degli opportuni valori, in modo da rendere il gioco equilibrato. Le statistiche associate 
    agli attributi di stato sono inizializzate solo dopo aver assegnato a questi attributi i valori iniziali delle corrispondenti 
    statistiche, ossia 0, in modo che all'inizio tutti i Countdown siano azzerati. */
    draw();
}

void Player:: upgrade (Statistics stats){
    this -> stats = stats;
    this -> stats.extention = Alone;
    /* Sovrascrive le statistiche attuali del giocatore con le nuove statistiche specificate, facendo attenzione a non modificare il
    valore assegnato all'attributo che specifica che non è presente un'estensione. */
}

void Player:: transfer (Room room, int y, int x){
    this -> room = room;
    this -> y = y;
    this -> x = x;
    /* Trasferisce il giocatore dalla stanza associata al livello in cui si trova attualmente, corrispondente ad un livello successivo, 
    precedente, secondario o speciale. Sposta poi il giocatore alle coordinate specificate. */
    destination.last = destination.current;
    destination.current = Current;
    /* Imposta la direzione dell'ultima porta attraversata dal giocatore alla direzione della porta che il giocatore stava attraversando 
    immediatamente prima di essere trasferito. Indica poi che il giocatore è stato trasferito all'interno del nuovo livello e che 
    attualmente si sta muovendo nella stanza corripondente senza attraversare nessuna porta. */ 
    draw();
}

void Player:: transition (Destination destination, Door door, int coordinate){
    this -> destination.current = destination;
    this -> destination.coordinate = coordinate;
    this -> destination.door = door;
    /* Quando il giocatore deve essere trasferito, assegna i valori indicati agli attributi che ne descrivono la transizione: la porta 
    utilizzata, la sua direzione e la coordinata relativa del punto preciso nella quale è stata attraversata. */
}

Transition Player:: passage (){
    return destination;
}
