#include "Enemy.hpp"

Enemy:: Enemy(){};
Enemy:: Enemy (Room room, int y, int x): Entity (room, '@', y, x){
// I nemici sono implementati come delle entità, la cui icona è '@' e la cui posizione è specificata al momento dell'inizializzazione.
    switch (rand()%9) {
        case 0: breed = Fly; break;
        case 1: breed = Mosquito; break;
        case 2: breed = Wasp; break;
        case 3: breed = Hornet; break;
        case 4: breed = Worm; break;
        case 5: breed = Mole; break;
        case 6: breed = Snake; break;
        case 7: breed = Snail; break;
        case 8: breed = Mix; break;
    }
    // Determina casualmente a quale delle razze esistenti appartiene il nemico.
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
    /* Determina casualmente la presenza dell'estensione del nemico e la sua direzione in relazione alla posizione dell'entità. Ripete 
    questa operazione fino a quando non trova una direzione che permetta di rappresentare l'estensione in modo che si sovrapponga 
    unicamente ad uno spazio libero, in cui è stampato il carattere ' '. */
    initialize();
}

void Enemy:: initialize (){
    if (breed == Fly){
        stats.health = 10;
        stats.movement = Casual;
        stats.speed = 40;
        stats.attack = Harmless;
        stats.cooldown = 0;
        stats.damage = 0;
        stats.shots = 0;
        stats.range = 0;
        stats.sight = 16;
        stats.direction = Point;
        stats.points = 2;
    }
    else if (breed == Mosquito){
        stats.health = 15;
        stats.movement = Following;
        stats.speed = 60;
        stats.attack = Body;
        stats.cooldown = 120;
        stats.damage = 2;
        stats.shots = 0;
        stats.range = 0;
        stats.sight = 14;
        stats.direction = Point;
        stats.points = 6;
    }
    else if (breed == Wasp){
        stats.health = 20;
        stats.movement = Casual;
        stats.speed = 80;
        stats.attack = Directional;
        stats.cooldown = 200;
        stats.damage = 4;
        stats.shots = 16;
        stats.range = 8;
        stats.sight = 8;
        stats.direction = Point;
        stats.points = 8;
    }
    else if (breed == Hornet){
        stats.health = 30;
        stats.movement = Stop;
        stats.speed = 0;
        stats.attack = (rand()%2 ? Cross : Plus);
        stats.cooldown = 140;
        stats.damage = 6;
        stats.shots = 20;
        stats.range = 12;
        stats.sight = 20;
        stats.direction = Point;
        stats.points = 10;
    }
    else if (breed == Worm){
        stats.health = 5;
        stats.movement = Teleport;
        stats.speed = 500;
        stats.attack = Harmless;
        stats.cooldown = 0;
        stats.damage = 0;
        stats.shots = 0;
        stats.range = 0;
        stats.sight = 20;
        stats.direction = Point;
        stats.points = 4;
    }
    else if (breed == Mole){
        stats.health = 25;
        stats.movement = Teleport;
        stats.speed = 500;
        stats.attack = (rand()%2 ? Cross : Plus);
        stats.cooldown = 220;
        stats.damage = 10;
        stats.shots = 22;
        stats.range = 10;
        stats.sight = 8;
        stats.direction = Point;
        stats.points = 16;
    }
    else if (breed == Snake){
        stats.health = 35;
        stats.movement = Following;
        stats.speed = 100;
        stats.attack = Directional;
        stats.cooldown = 180;
        stats.damage = 8;
        stats.shots = 18;
        stats.range = 14;
        stats.sight = 12;
        stats.direction = Point;
        stats.points = 12;
    }
    else if (breed == Snail){
        stats.health = 40;
        stats.movement = Stop;
        stats.speed = 0;
        stats.attack = Body;
        stats.cooldown = 160;
        stats.damage = 12;
        stats.shots = 0;
        stats.range = 0;
        stats.sight = 12;
        stats.direction = Point;
        stats.points = 14;
    }
    else if (breed == Mix){
        // Determina casualmente le statistiche del nemico qualora la razza a cui appartiene lo implichi.
        stats.health = (1+(rand()%(8-1))+1)*5;
        // Assegna ai punti salute un valore casuale compreso fra 5*1 = 5 e 5*8 = 40, considerando solo i multipli di 5.
        switch (rand()%4) {
            case 0: stats.movement = Casual; break;
            case 1: stats.movement = Following; break;
            case 2: stats.movement = Teleport; break;
            case 3: stats.movement = Stop; break;
        }
        // Assegna allo stile di movimento un valore determinato casualmente fra le opzioni a disposizione.
        stats.speed = (stats.movement == Teleport ? 500 : (1+rand()%(5-2)+2)*20);
        /* Assegna alla velocità del movimento un valore casuale compreso fra 20*2 = 40 e 20*5 = 100, considerando solo i multipli di 20. 
        Gli assegna invece il valore 500 se lo stile di movimento indica che il nemico si muove teletrasportandosi. */
        switch (rand()%4) {
            case 0: stats.attack = Cross; break;
            case 1: stats.attack = Plus; break;
            case 2: stats.attack = Directional; break;
            case 3: stats.attack = Harmless; break;
            case 4: stats.attack = Body; break;
        }
        // Assegna allo stile di attacco un valore determinato casualmente fra le opzioni a disposizione.
        stats.cooldown = (1+rand()%(11-6)+6)*20;
        /* Assegna al tempo di caricamento degli attacchi un valore casuale compreso fra 20*6 = 120 e 20*11 = 220, considerando solo i 
        multipli di 20. */
        stats.damage = (stats.attack == Harmless ? 0 : (1+rand()%(6*2))+1)*2;
        /* Assegna al potenziale di danno degli attacchi un valore casuale compreso fra 2*1 = 2 e 2*6 = 12, considerando solo i multipli di 
        2. Gli assegna invece il valore 0 se lo stile di attacco indica che il nemico è innocuo. */
        stats.shots = (1+rand()%(12-9)+9)*2;
        // Assegna alla velocità dei proiettili un valore casuale compreso fra 2*9 = 18 e 2*12 = 24, considerando solo i multipli di 2.
        stats.range = (1+rand()%(7-4)+4)*2;
        // Assegna alla gittata dei proiettili un valore casuale compreso fra 2*4 = 8 e 2*7 = 14, considerando solo i multipli di 2.
        stats.sight = (1+rand()%(5-2)+2)*4;
        // Assegna all'area visiva un valore casuale compreso fra 4*2 = 8 e 4*5 = 20, considerando solo i multipli di 4.
        stats.points = (1+rand()%(8-1)+1)*2;
        // Assegna ai punti esperienza un valore casuale compreso fra 2*1 = 2 e 2*8 = 16, considerando solo i multipli di 2.
    }
    // Assegna alle statistiche del nemico i valori determinati dalla razza a cui appartiene.
    status = stats;
    // Assegna agli attributi associati allo stato attuale del nemico i valori delle statistiche corrispondenti.
    draw ();
}
