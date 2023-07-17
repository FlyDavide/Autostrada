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
    struct pathStation *prec;
} arcPath;

station *insertStation(station **head, int numDist, int numCar);
station *searchStation(station **head, int dist);
void stationWalk(station *head);
int readNumOutput();
void searchPath(station **headS, station **endS, int start, int end, arcPath **headP);
void createPath(station **start, station **end, arcPath **head);
void updateListPath(arcPath **head, arcPath **node, station **start);
void addPath(arcPath **head, station **dist, station **end, int numS);
void deleteListPath(arcPath **head);
void walkPath(station **startPath);
void readPath(arcPath **head);


int main(){

    char c;
    station *autostrada = NULL;
    station *stazione;


    while( 1 ){
        printf("Dati: ");
        c = getchar();
        switch (c){
        case 's':
            getchar();
            int dist = readNumOutput();
            int car = readNumOutput();
            stazione = insertStation(&autostrada, dist, car);
            printf("\ndati Salvati\n");
            break;
        case 'p':
            getchar();
            int start = readNumOutput();
            int end = readNumOutput();
            station *endStation = NULL;
            arcPath *percorsi = NULL;
            printf("\nStazioni: ");
            searchPath(&autostrada, &endStation, start, end, &percorsi);
            walkPath( &(percorsi->startPath) );
            readPath(&percorsi);
            //deleteListPath(&percorsi);
            printf("\nCreazione percorso fine!");
            break;
        default:
            break;
        }

    }
    
}


int readNumOutput(){ 
    int pos = 0;
    char num[12];
    char l = getchar();
    while( l != ' ' && l != '\n' ){
        num[pos] = l;
        ++pos;
        l = getchar();
    }
    num[pos] = '\0';
    return atoi(num);
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
    figlio->path = NULL;
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
void searchPath(station **headS, station **endS, int start, int end, arcPath **headP){
    if((*headS) != NULL ){
        if( (*headS)->distance < end ){
        searchPath(&((*headS)->right), &(*endS), start, end, &(*headP));
        }

        if( (*headS)->distance >= start && (*headS)->distance < end ){
            createPath(&(*headS), &(*endS), &(*headP));
            //printf("%d ", (*headS)->distance);
        } else if( (*headS)->distance == end ){
            (*headS)->path = NULL;
            (*endS) = (*headS);
        }

        if ( (*headS)->distance >= start ){
            searchPath( &((*headS)->left), &(*endS), start, end, &(*headP));
        }
    }

}

void createPath(station **start, station **end, arcPath **head){
    if( (*start)->car >= ( (*end)->distance - (*start)->distance ) ){
        //Aggiungo questa stazione nella lista delle stazioni possibili di percorrenza
        addPath(&(*head), &(*start), &(*end), 1);
    }
    else{
        arcPath *nodeCur = *head;
        arcPath *choseNodeP = NULL;
        station *choseNodeS = NULL;
        int numS;
        while( ( nodeCur != NULL ) && ( (*start)->car >= ( (nodeCur->startPath)->distance - (*start)->distance ) ) ){
            if( ( choseNodeP == NULL && choseNodeS == NULL ) || ( nodeCur->numStation < numS ) ||
                ( ( nodeCur->numStation == numS ) && (
                    ( ( choseNodeP != NULL ) && ( ((nodeCur->startPath)->distance - (*start)->distance) < ( (choseNodeP->startPath)->distance - (*start)->distance) ) ) ||
                    ( (choseNodeS != NULL) && ( ((nodeCur->startPath)->distance - (*start)->distance) < ( choseNodeS->distance - (*start)->distance)) ) 
                ) )  
            ){
                numS = nodeCur->numStation;
                choseNodeP = nodeCur;
                choseNodeS = nodeCur->startPath;
                while( numS > 1 && ( (*start)->car >= ( (choseNodeS->path)->distance - (*start)->distance) ) ){
                    choseNodeS = choseNodeS->path;
                    --numS;
                }
                if( choseNodeP->startPath != choseNodeS ) choseNodeP = NULL;
                else choseNodeS = NULL;
            }
            nodeCur = nodeCur->next;
        }
        //aggiorno il valore della lista dei possibili percorsi: faccio un update
        if( choseNodeP != NULL ){//aggiorno l'arco
            updateListPath(&(*head), &choseNodeP, &(*start)); 
        }
        else if ( choseNodeS != NULL ){//aggiungo un nuovo arco
            addPath(&(*head), &(*start), &choseNodeS, numS + 1);
        }
    }
}

//L'aggiornamento consiste nel prendere il nodo che deve essere aggiornato e spostarlo all'inizio della lista in quanto sarà quello più vicino
void updateListPath(arcPath **head, arcPath **node, station **start){
    if( (*head) != (*node) ){
        arcPath *nodoPrec = (*node)->prec;
        arcPath *nodoNext = (*node)->next;
        nodoPrec->next = nodoNext;
        if( nodoNext != NULL ) nodoNext->prec = nodoPrec;
        (*node)->prec = NULL;
        (*head)->prec = (*node);
        (*node)->next = (*head);
        (*head) = (*node);
    }
    (*head)->numStation = (*head)->numStation + 1;
    (*start)->path = (*head)->startPath;
    (*head)->startPath = *start;
}

void addPath(arcPath **head, station **dist, station **end, int numS){
    (*dist)->path = *end;
    arcPath *nodoNew = (arcPath *) malloc( sizeof(arcPath) );
    nodoNew->numStation = numS;
    nodoNew->next = *head;
    if( (*head) != NULL ) (*head)->prec = nodoNew;
    nodoNew->prec = NULL;
    nodoNew->startPath = *dist;
    (*head) = nodoNew;
}

void deleteListPath(arcPath **head){
    arcPath *nodeNext = NULL;
    arcPath *nodeCur = *head;
    while ( nodeCur != NULL ){
        nodeNext = nodeCur->next;
        free( nodeCur );
        nodeCur = nodeNext;
    }
}

void walkPath(station **startPath){
    station *node = *startPath;
    while( node != NULL ){
        printf(" %d ", node->distance );
        node = node->path;
    }
    
}

void readPath(arcPath **head){
    arcPath *nodo = *head;
    printf("\nLista Percorsi: ");
    while( nodo != NULL ){
        printf(" [ dist: %d , numS: %d ]", (nodo->startPath)->distance, nodo->numStation);
        nodo = nodo->next;
    }
    printf("\n");
}