#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct nodeStation {
    int distance;
    int car;
    struct nodeStation *parent;
    struct nodeStation *right;
    struct nodeStation *left;
    struct nodeStation *path;
} station; //Stazioni che rappresentano l'autostrada


typedef struct pathStation {
    int numStation;
    struct nodeStation *startPath;
    struct pathStation *next;
} arcPath;

station *insertStation(station **head, int numDist, int numCar);
station *searchStation(station **head, int dist);
void stationWalk(station *head);



int main(){




    arcPath *possiblePaths = NULL;





    
}

//inserisco una nuova stazione nell'albero, se esiste di già restituisco 0 se no 1.
station *insertStation(station **head, int numDist, int numCar){
    station *leaf = NULL; //foglia, il nodo che non avrà figli
    station *nodo = *head;
    while(nodo != NULL){
        leaf = nodo;
        if ( numDist ==  nodo->distance) return NULL; //non aggiungo il nodo in quanto è già presente
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
    figlio->car = numCar;
    //aggiungere i nuovi elementi trovati
    if(leaf == NULL ){
        *head = figlio;
    }
    else if (leaf->distance < figlio->distance) {
        leaf->right = figlio;
    }
    else {
        leaf->left = figlio;
    }
    return figlio; //ha inserito la stazione correttamente
}

//percorro l'albero delle stazzioni
void stationWalk(station *head){
    if(head != NULL){
        stationWalk(head->left);
        printf("\nstazione: %d [dimensione: %d] - ", head->distance, sizeof(head));
        stationWalk(head->right);
    }
}

station *searchStation(station **head, int dist){
    station *nodo = *head;
    while(nodo != NULL){
        if(nodo->distance == dist ) return nodo;
        else if(dist > nodo->distance ) nodo = nodo->right;
        else nodo = nodo->left;
    }
    return NULL; // non è stato trovato nessun valore con questo nodo
}

//scansione l'albero per creare i percorsi possibili 
station searchPath(station **head, station **end,  int start){

    if( (*head)->distance < end ){
        searchPath( (*head)->right, start, end);
    }

    if( (*head)->distance == end ){
        (*end) = (*head);
    }
    //Chiamo la funzione:


    if ( (*head)->distance > start ){
        searchPath( (*head)->left, start, end);
    }

}

void createPath(station **start, station **end, arcPath **head){

    if( (*start)->car == 0 ){
        //Devo ancora capire cosa devo fare
        return NULL; //FINE programma
    }
    else if( (*start)->car >= ( (*end)->distance - (*start)->distance ) ){
        //Aggiungo questa stazione nella lista delle stazioni possibili di percorrenza

    }

    int numS = 0; //Numero delle stazioni che attreverserà dalla stazione di partenza
    arcPath *nodeCur = (*head);
    arcPath *nodePrior = NULL;

    while( nodeCur != NULL ){
        if( (*start)->car >= ( (nodeCur->startPath)->distance - (*start)->distance ) ){
            if( nodePrior == NULL ){
                numS = nodeCur->numStation + 1;
                nodePrior = nodeCur;
            }
            else if( nodeCur->numStation < numS ){
                

            }
            numS = nodeCur->numStation + 1;
            nodePrior = nodeCur;

        
        }
        else {
            //Devo uscire dal ciclo while in quanto non è raggiungibile nessualtra stazione

        }





    }

    return NULL; 

}



