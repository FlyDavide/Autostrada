#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodeStation {
    int distance;
    int *cars;
    struct nodeStation *parent;
    struct nodeStation *right;
    struct nodeStation *left;
} station; //Stazioni che rappresentano l'autostrada

typedef struct nodeCars{
    int autonomy;
    struct nodeCars *next;
} autonomyCars; //rappresenta la lista di auto di una determinata stazione


void insertStation(station **head, int numDist);
void stationWalk(station *head);
void delateStation(station **head, int numDist);

int main(int argc, char *argv[]){

    /* metto in pausa per il momento
    int dati[] = {5, 84201, 32123, 12410, 8, 7, 5, 31, 9123, 2, 32, 11, 93, 6148, 8371, 61, 90, 74, 121, 52223, 754, 111, 18391, 895, 8283, 983849, 8342, 9847, 123, 9, 14, 1, 928, 12, 65, 523, 556, 56, 213, 79, 65, 304, 922, 789, 78, 7, 0, 12, 8, 3423, 5433456}; //sono i dati che dovrò inserire nell'albero BST, per verificare che funzioni
    printf("Inizio programma\n");
    
    station *autostrada = NULL;
    int lenDati = sizeof(dati)/sizeof(dati[0]);
    printf("len dati: %d \n", lenDati);
    for(int i = 0; i < lenDati ; i++){
        insertStation(&autostrada, dati[i] );
    }
    stationWalk(autostrada);
    printf("\nFine programma\n");
    */

    //voglio provare a prendere una stringa di interi e spezzarli:
    printf("Inserire Stringa: ");

    char c;
    char num[12];
    int turn = 0; //Turni: 0 -> indica che devo salvarmi la stazione; 1 -> salvarmi il numero di auto da leggere; 2 -> lettura delle auto
    int pos = 0; //posizione di lettura dell'array num;
    int stazione;
    int numCars1; // rimarrà costante in modo che io conosca l'esatta dimensione dell'array
    int numCars2; // il valore che dimunuirà 
    int *cars = NULL;
    while ((c = getchar()) != '\n') {
        if(c == ' '){
            if(turn == 0){
                stazione = atoi(num);
                ++turn;
            } else if(turn == 1){
                numCars1 = atoi(num);
                numCars2 = numCars1;
                cars = (int*) malloc(numCars1 * sizeof(int));
                ++turn;
            } else {
                cars[numCars1 - numCars2] = atoi(num);
                --numCars2;
            }
            pos = 0;
        } else {
            num[pos] = c;
            ++pos;
            num[pos] = '\0'; //Per non rischare quando userò la funzione atoi di leggere dei valori vecchi in quanto l'atori convertirebbe gli interi prima del valore '\0'
        }
    }
    cars[numCars1 - 1] = atoi(num); //è il valore finale che non verrebbe letto in quanto esce dal ciclo visto che finisce con '\n' e non con ' '
    
    fflush(stdout);
    printf("lettura auto:\n");
    int i = 0;
    while(i < numCars1){
        printf("auto %d-> %d\n", i , *(cars + i));
        ++i; 
    }
    printf("stazione: %d\n", stazione);

    printf("fine programma");
    
}

//inserisco una nuova stazione nell'albero
void insertStation(station **head, int numDist){
    station *leaf = NULL; //foglia, il nodo che non avrà figli
    station *nodo = *head;
    while(nodo != NULL){
        leaf = nodo;
        if ( numDist ==  nodo->distance) return; //non aggiungo il nodo in quanto è già presente
        else if( numDist >  nodo->distance ){
            nodo = nodo->right;
        } else {
            nodo = nodo->left;
        }
    }
    station *figlio = (station*) malloc(sizeof(station));
    figlio->distance = numDist;
    figlio->left = NULL;
    figlio->right = NULL;
    figlio->parent = leaf;
    if(leaf == NULL ){
        *head = figlio;
    }
    else if (leaf->distance < figlio->distance) {
        leaf->right = figlio;
    }
    else {
        leaf->left = figlio;
    }
}

//percorro l'albero delle stazzioni
void stationWalk(station *head){
    if(head != NULL){
        stationWalk(head->left);
        printf("num nodo: %d \n", head->distance);
        int sizeNodo = sizeof(head);
        //printf("dimensione nodo: %d \n", sizeNodo);
        stationWalk(head->right);
    }
}

//Cancellare la stazione (Da completare !!)
void delateStation(station **head, int numDist){
    if(head != NULL ){
        if ((*head)->distance == numDist){
            //?
        }
    }
}
