# Progetto di Programmazione

**Anno Accademico 2021-2022**  
**Bacci Manuel** - 0001040090 - [manuel.bacci@studio.unibo.it](mailto:manuel.bacci@studio.unibo.it)

---

## Introduzione

Il gioco si sviluppa in una mappa senza traguardo, composta da infiniti livelli. Ciascun livello è associato ad una stanza differente, generata in modo casuale. All’interno di ogni stanza sono presenti diverse porte, ostacoli e nemici. La mappa viene esplorata stanza dopo stanza dal giocatore comandato da tastiera, che può proseguire nell’esplorazione o tornare indietro e trovare tutto come lo ha lasciato. Sparando dei proiettili, il giocatore deve sconfiggere tutti i nemici generati all’interno del livello in cui si trova. Se un nemico viene colpito da uno dei proiettili sparati dal giocatore, perde una quantità di punti vita pari al suo potenziale di danno. Ogni volta che il giocatore sconfigge un nemico, ossia quando i punti vita del suo bersaglio raggiungono lo zero, guadagna una  determinata quantità di punti esperienza. Una volte che tutti i nemici sono stati sconfitti, le porte presenti all’interno della stanza vengono aperte, permettendo al giocatore di attraversarle per dirigersi verso un livello successivo non ancora esplorato, o verso un livello precedente. E’ inoltre possibile che l’ultimo nemico sconfitto generi un potere, con cui il giocatore può interagire per ripristinare completamente i suoi punti vita. Nel momento in cui il giocatore interagisce con questo potere, è anche probabile che compaia nel livello una nuova porta segreta, diretta verso una stanza nascosta in cui è presente un artefatto. Gli artefatti migliorano una statistica a caso fra gli attributi associati al giocatore: i suoi punti vita, il suo punteggio, il tempo di ricarica dei suoi attacchi, il suo potenziale di danno, la gittata o la velocità dei suoi proiettili. Se all’interno del livello il giocatore viene colpito da un proiettile o da un attacco a corpo sferrato da un nemico, perde una quantità di punti vita pari al suo potenziale di danno. Il giocatore possiede un numero limitato di cuori, ciascuno dei quali corrisponde a dieci punti vita, arrotondati per eccesso. Il gioco termina una volta che i cuori del giocatore scendono a zero.

## Comandi

Premendo o tenendo premuti i tasti **WASD** è possibile movere l’icona del giocatore all’interno della mappa di gioco: con **W** o **w** verso l’alto, con **S** o **s** verso il basso, con **A** o **a** verso sinistra e con **D** o **d** verso destra. Premendo o tenendo premuti i tasti direzionali **↓↑←→** è invece possibile far si che il giocatore spari dei proiettili: con **↑** verso l’alto, con **↓** verso il basso, con **←** verso sinistra e con **→** verso destra.

---

## Compilazione ed Esecuzione

Per eseguire il progetto, aprire un terminale e accedere alla directory **code** contenente i file sorgente, navigando con il comando **cd**. Una volta all’interno della cartella, compilare il programma tramite **make**, che utilizza il Makefile fornito per generare l’eseguibile. Completata la compilazione, l’esecuzione del programma avviene tramite il comando **./Progetto** su sistemi Unix-like o **Progetto.exe** su sistemi Windows. In sintesi:

```shell
cd .../programmazione/code
make
./Progetto
```

---

## Informazioni

Il progetto è stato sviluppato e testato su sistema operativo macOS. All'interno della Directory principale **programmazione**, oltre al codice eseguibile sono presenti una breve relazione sulle principali scelte implementative e una registrazione di alcuni minuti del gioco.
