#include "Artifact.hpp"

Artifact:: Artifact (){};
Artifact:: Artifact (Room room, Player player, int y, int x, bool power): Entity (room, 'A', y, x){
// Gli artefatti sono implementati come delle entità, la cui icona è 'A' e la cui posizione è specificata al momento dell'inizializzazione.
    stats = player.statistics ();
    /* Assegna alle statistiche dell'artefatto i valori delle statistiche attuali del giocatore. Di queste, le statistiche utili per 
    l'implementazione sono solo quelle a cui è attribuito un valore numerico diverso da 0, e che non indicano quindi uno stato o uno stile, 
    ossia i punti vita massimi, il potenziale di danno, la velocità dei proiettili, il tempo di caricamento degli attacchi, e la gittata 
    dei proiettili. */
    this -> power = power;
    switch (rand()%6){
        case 0: if (stats.health <= 100) stats.health +=10; break;
        case 1: if (stats.cooldown >= 60) stats.cooldown -=10; break;
        case 2: if (stats.damage <= 15) stats.damage ++; break;
        case 3: if (stats.range <= 30) stats.range += 2; break;
        case 4: if (stats.shots >= 8) stats.shots --; break;
        case 5: stats.points +=5; break;
        default: break;
    }
    /* Incrementa o decrementa di un valore specifico una a caso fra le statistiche utili associate all'artefatto, qualora essa non abbia 
    raggiunto il limite massimo o minimo. */
    bool free;
    do {
        free = true;
        switch (rand()%5) {
            case 0: stats.extention = Top; break;
            case 1: stats.extention = Bottom; break;
            case 2: stats.extention = Right; break;
            case 3: stats.extention = Left; break;
            case 5: stats.extention = Alone; break;
        }
        if ((stats.extention == Top && room.get (y-1, x) != ' ') || 
            (stats.extention == Bottom && room.get (y+1, x) != ' ') || 
            (stats.extention == Right && room.get (1, x+1) != ' ') || 
            (stats.extention == Left && room.get (y, x-1) != ' ')) free = false;
    } while (!free);
    /* Determina casualmente la presenza dell'estensione dell'ertefatto e la sua direzione in relazione alla posizione dell'entità. 
    Ripete poi questa operazione fino a quando non trova una direzione che permetta di rappresentare l'estensione in modo che si 
    sovrapponga unicamente ad uno spazio libero, in cui è stampato il carattere ' '. */
    draw ();
}

Player Artifact:: upgrade (Player player){
    if (power) player.reset (Health);
    else {
        stats.direction = player.statistics().direction;
        player.upgrade (stats);
    }
    /* Se il giocatore entra in contatto con un artefatto che racchiude un potere, ripristina i sui punti vita, altrimenti potenzia le sue 
    statistiche sostituendole con le statistiche migliorate dell'artefatto, prestando attenzione a non modificarne la direzione. */
    return player;
}
