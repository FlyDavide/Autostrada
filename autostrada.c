#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodeStation {
    int distance;
    int cars;
    struct nodeStation *parent;
    struct nodeStation *right;
    struct nodeStation *left;
} station;


void insertStation(station **head, int numDist);
void stationWalk(station *head);

int main(int argc, char *argv[]){
    int dati[] = {5, 8, 2, 9, 1}; //sono i dati che dovrò inserire nell'albero BST, per verificare che funzioni
    printf("Inizio programma\n");
    
    station *autostrada = NULL;
    for(int i = 0; i < 5; i++){
        insertStation(&autostrada, dati[i] );
    }
    stationWalk(autostrada);
    printf("Fine programma\n");
}

void insertStation(station **head, int numDist){
    station *leaf = NULL; //foglia, il nodo che non avrà figli
    station *nodo = *head;
    while(nodo != NULL){
        leaf = nodo;
        if( numDist >  nodo->distance ){
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

void stationWalk(station *head){
    if(head != NULL){
        stationWalk(head->left);
        printf("- %d ", head->distance);
        stationWalk(head->right);
    }
}
