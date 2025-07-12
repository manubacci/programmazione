#include "Bullet.hpp"

Bullet:: Bullet (){};
Bullet:: Bullet (Room room, Player player, Direction direction, int y, int x): Entity (room, '.', y, x){
// I proiettili sono implementati come delle entità, la cui icona è '.' e la cui posizione è specificata al momento dell'inizializzazione.
    stats.speed = player.statistics().shots;
    stats.damage = player.statistics().damage;
    stats.range = player.statistics().range;
    stats.direction = direction;
    status = stats;
    subject = User;
    /* Se il proiettile è stato generato come conseguenza di un attacco del giocatore, assegna alle statistiche che ne indicano la 
    velocità, il danno e la gittata le corrispondenti statistiche associate al giocatore che lo ha sparato. Assegna poi agli attributi 
    di stato attuali del proiettile i valori delle statistiche corrispondenti e alla sua direzione il valore specificato. Indica infine 
    che il soggetto che ha sparato il proiettile è il giocatore. */
    draw ();
}

Bullet:: Bullet (Room room, Enemy enemy, Direction direction, int y, int x): Entity (room, '.', y, x){
    stats.speed = enemy.statistics().shots;
    stats.damage = enemy.statistics().damage;
    stats.range = enemy.statistics().range;
    stats.direction = direction;
    status = stats;
    subject = Oppenent;
    /* Se il proiettile è stato sparato da un nemico, assegna alle sue statistiche e ai suoi attributi di stato le corrispondenti 
    statistiche associate al nemico che lo ha sparato. Imposta poi la direzione del proiettile al valore specificato e indica che il 
    soggetto che lo ha sparato è un nemico. */
    draw ();
}

Subject Bullet:: shooter (){
    return subject;
}
