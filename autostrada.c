#include <stdio.h>
#include <stdlib.h>
#define maxArray 100

typedef struct nodeStation {
    int distance;
    struct nodeCars *cars;
    struct nodeStation *parent;
    struct nodeStation *right;
    struct nodeStation *left;
    struct nodeStation *path;
} station;

typedef struct pathStation {
    int numStation;
    struct nodeStation *startPath;
    struct pathStation *next;
    struct pathStation *prec;
} arcPath;

typedef struct nodeCars{
    int autonomy;
    struct nodeCars *next;
} autonomyCars;


station *insertStation(station **head, int numDist);
station *searchStation(station **head, int dist);
void delateStation(station **head, station **nodeToDelate);
station *treeSuccessor(station **node);

int readNumOutput();
void goToEndLine(char stop);
char getCommand(char lettera);
void heapSort(int *arrayCars, int numCars);
void maxHeapify(int *arrayCars, int num, int heapMax);

autonomyCars *createListAutonomyCars(int *arrayCarsOrder, int lenArray);
autonomyCars *insertAutnomyCars(autonomyCars **cars, int *arrayCarsOrder, int lenArray);
int delateCar(station **head, autonomyCars **listCars, int autonomyCar);
void deleteListCars(autonomyCars **head);

void searchPathIncreasing(station **headS, station **endS, int start, int end, arcPath **headP);
void searchPathDecreasing(station **headS, station **endS, int start, int end, arcPath **headP);
void createPath(station **start, station **end, arcPath **head);
void updateListPath(arcPath **head, arcPath **node, station **start);
void addPath(arcPath **head, station **dist, station **end, int numS);
void deleteListPath(arcPath **head);
void walkPath(station **startPath);
int searcBestPath(station *firstPath, station *secondPath, int numStation);


int main(){    
    station *autostrada = NULL;
    
    int stateTurn; 
    int numLine;
    int maxArrayAuto = maxArray;
    int readOutput;
    int *arrayCars = NULL;
    int lenArrayCars = 0;
    int end;
    station *stazione = NULL;
    arcPath *percorsi = NULL;

    while( (readOutput = getchar()) != EOF ){

        stateTurn = getCommand(readOutput);
        goToEndLine(' ');

        if( stazione != NULL && stateTurn != 'a' ){
            heapSort(arrayCars, lenArrayCars);
            if( stazione->cars != NULL ) stazione->cars = insertAutnomyCars(&(stazione->cars), arrayCars, lenArrayCars);
            else stazione->cars = createListAutonomyCars(arrayCars, lenArrayCars);
            free(arrayCars);
            stazione = NULL;
            maxArrayAuto = maxArray;
        }

        numLine = readNumOutput();

        switch (stateTurn) {
            case 's':
                stazione = insertStation( &autostrada, numLine );
                if( stazione != NULL ){
                    lenArrayCars = readNumOutput();
                    if(lenArrayCars){
                        arrayCars = (int *) malloc( lenArrayCars * sizeof(int));
                        for(int i = 0; i < lenArrayCars; i++){
                            arrayCars[i] = readNumOutput();
                        }
                        heapSort(arrayCars, lenArrayCars);
                        stazione->cars = createListAutonomyCars(arrayCars, lenArrayCars);
                    }
                    stazione = NULL;
                    printf("aggiunta\n");   
                }
                else {
                    goToEndLine('\n');
                    printf("non aggiunta\n");
                }
                break;
            case 'a':
                if( ( stazione != NULL ) && ( numLine == stazione->distance ) ){
                    ++lenArrayCars;
                    if( lenArrayCars > maxArrayAuto ){
                        maxArrayAuto += maxArray;
                        arrayCars = (int*) realloc( arrayCars, maxArrayAuto * sizeof(int) );
                    }
                    arrayCars[lenArrayCars - 1] = readNumOutput();

                } else if ( stazione != NULL ){
                    heapSort(arrayCars, lenArrayCars);
                    if( stazione->cars != NULL ) stazione->cars = insertAutnomyCars(&(stazione->cars), arrayCars, lenArrayCars);
                    else stazione->cars = createListAutonomyCars(arrayCars, lenArrayCars);
                    free(arrayCars);
                    lenArrayCars = 0;
                    stazione = NULL;
                    maxArrayAuto = maxArray;
                }

                if ( stazione == NULL ){
                    stazione = searchStation( &autostrada, numLine);
                    if( stazione != NULL ){
                        arrayCars = (int*) malloc(maxArrayAuto * sizeof(int));
                        arrayCars[0] = readNumOutput();
                        lenArrayCars = 1;
                    } else {
                        goToEndLine('\n');
                        printf("non aggiunta\n");
                        break;
                    }
                }
                printf("aggiunta\n");
                break;
            case 'r':
                stazione = searchStation( &autostrada, numLine );
                if( stazione != NULL ){
                    numLine = readNumOutput();
                    if( delateCar(&stazione, &(stazione->cars), numLine) ) printf("rottamata\n");
                    else printf("non rottamata\n");
                    stazione = NULL;
                } else{
                    goToEndLine('\n');
                    printf("non rottamata\n");
                }
                break;
            case 'd':
                stazione = searchStation( &autostrada, numLine );
                if( stazione != NULL ){
                    delateStation(&autostrada, &stazione);
                    stazione = NULL;
                    printf("demolita\n");
                } else printf("non demolita\n");
                break;
            case 'p':
                end = readNumOutput();
                stazione = searchStation(&autostrada, numLine);
                if( numLine == end ) printf("%d\n", numLine);
                else if( stazione->cars == NULL ) printf("nessun percorso\n");
                else if( (stazione->cars)->autonomy >= abs(end - numLine) ) printf("%d %d\n", numLine, end);
                else{
                    if( numLine < end ) searchPathIncreasing(&autostrada, &stazione, numLine, end, &percorsi);
                    else searchPathDecreasing(&autostrada, &stazione, numLine, end, &percorsi);
                    if( ( percorsi != NULL ) && ( (percorsi->startPath)->distance == numLine ) ) {
                        walkPath(&(percorsi->startPath));
                        deleteListPath(&percorsi);
                    } else printf("nessun percorso\n");
                }
                stazione = NULL;
                percorsi = NULL;
                break;
            default:
                break;
        }
    }
    return 0;
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

char getCommand(char lettera){
    char c = lettera;
    if( c == 'a' ){
        while( ( c = getchar() ) != '-'){}
        c = getchar();
    }
    return c;
}

void goToEndLine(char stop){
    while( getchar() != stop ){}
}

void heapSort(int *arrayCars, int numCars){
    for(int i = (numCars/2) - 1 ; i >= 0; i--){
        maxHeapify(arrayCars, i, numCars);
    }
    for(int i = numCars - 1; i > 0; i--){
        int temp = arrayCars[i];
        arrayCars[i] = arrayCars[0];
        arrayCars[0] = temp;
        maxHeapify(arrayCars, 0, i);
    }
}

void maxHeapify(int *arrayCars, int num, int heapMax){
    int max = num;
    int left = 2*num + 1;
    int right = 2*num + 2;
    if( left < heapMax && arrayCars[left] > arrayCars[max]){
        max = left;
    }
    if(right < heapMax && arrayCars[right] > arrayCars[max]){
        max = right;
    }
    if(max != num){
        int temp = arrayCars[num];
        arrayCars[num] = arrayCars[max];
        arrayCars[max] = temp;
        maxHeapify(arrayCars, max, heapMax);
    }
}

autonomyCars *createListAutonomyCars(int *arrayCarsOrder, int lenArray){
    autonomyCars *head = NULL;
    for(int i = 0; i < lenArray; i++){
        autonomyCars *node = (autonomyCars*) malloc( sizeof(autonomyCars) );
        node->autonomy = arrayCarsOrder[i];
        node->next = head;
        head = node;
    }
    return head;
}

autonomyCars *insertAutnomyCars(autonomyCars **cars, int *arrayCarsOrder, int lenArray){
    autonomyCars *head = *cars;
    autonomyCars *nodoPre = NULL;
    autonomyCars *nodo = *cars;

    for(int i = lenArray - 1; i >= 0; i--){
        while( (nodo != NULL) && (nodo->autonomy > arrayCarsOrder[i] )){
            nodoPre = nodo;
            nodo = nodo->next;
        }
        autonomyCars *newNode = (autonomyCars*) malloc( sizeof(autonomyCars) );
        newNode->autonomy = arrayCarsOrder[i];
        newNode->next = nodo;
        if( ( i == lenArray - 1 ) && ( nodoPre == NULL ) ) head = newNode;
        else nodoPre->next = newNode;
        nodoPre = newNode;
    }
    return head;
}


int delateCar(station **head, autonomyCars **listCars, int autonomyCar){
    autonomyCars *nodo = *listCars;
    autonomyCars *nodoPrec = NULL;
    while( ( nodo != NULL ) && ( autonomyCar <= nodo->autonomy ) ){
        if( autonomyCar == nodo->autonomy ){
            if( nodoPrec != NULL ) nodoPrec->next = nodo->next;
            else ( (*head)->cars ) = nodo->next;
            free(nodo);
            return 1;
        } else {
            nodoPrec = nodo;
            nodo = nodo->next;
        }
    }
    return 0;
}

void deleteListCars(autonomyCars **head){
    autonomyCars *nodo = *head;
    autonomyCars *nodoNext = NULL;
    while( nodo != NULL){
        nodoNext = nodo->next;
        free(nodo);
        nodo = nodoNext;
    }
}

station *insertStation(station **head, int numDist){
    station *leaf = NULL;
    station *nodo = *head;
    while(nodo != NULL){
        leaf = nodo;
        if ( numDist ==  nodo->distance) return NULL;
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
    figlio->cars = NULL;
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
    return figlio;
}

station *searchStation(station **head, int dist){
    station *nodo = *head;
    while(nodo != NULL){
        if(nodo->distance == dist ) return nodo;
        else if(dist > nodo->distance ) nodo = nodo->right;
        else nodo = nodo->left;
    }
    return NULL;
}

void delateStation(station **head, station **nodeToDelate){
    station *y = NULL;
    station *x = NULL;
    if((*nodeToDelate)->left == NULL || (*nodeToDelate)->right == NULL ){
        y = *nodeToDelate;
    } else {
        y = treeSuccessor(&(*nodeToDelate));
    }

    if(y->left != NULL) x = y->left;
    else x = y->right;

    if( x != NULL){
        x->parent = y->parent;
    }
    if(y->parent == NULL){
        (*head) = x;
    }
    else if ( y == (y->parent)->left){
        (y->parent)->left = x;
    }
    else{
        (y->parent)->right = x;
    }
    if(y != *nodeToDelate){
        (*nodeToDelate)->distance = y->distance;
        (*nodeToDelate)->path = y->path;
        deleteListCars(&((*nodeToDelate)->cars));
        (*nodeToDelate)->cars = y->cars;
    } else deleteListCars(&(y->cars));
    free(y);
}

station *treeSuccessor(station **node){
    station *x = NULL;
    if((*node)->right != NULL){
        x = (*node)->right;
        while( x->left != NULL ){
            x = x->left;
        }
        return x;
    }
    station *y = x->parent;
    while( y != NULL && x == y->right ){
        x = y;
        y = y->parent;
    }
    return y;
    
}

void searchPathIncreasing(station **headS, station **endS, int start, int end, arcPath **headP){
    if((*headS) != NULL ){
        if( (*headS)->distance < end ){
        searchPathIncreasing(&((*headS)->right), &(*endS), start, end, &(*headP));
        }

        if( ((*headS)->distance >= start) && ((*headS)->distance < end) && ((*headS)->cars != NULL) ){
            createPath(&(*headS), &(*endS), &(*headP));
        } else if( (*headS)->distance == end ){
            (*headS)->path = NULL;
            (*endS) = (*headS);
        }

        if ( (*headS)->distance >= start ){
            searchPathIncreasing( &((*headS)->left), &(*endS), start, end, &(*headP));
        }
    }
}

void searchPathDecreasing(station **headS, station **endS, int start, int end, arcPath **headP){
    if((*headS) != NULL ){
        if( (*headS)->distance > end ){
        searchPathDecreasing(&((*headS)->left), &(*endS), start, end, &(*headP));
        }

        if( ((*headS)->distance <= start) && ((*headS)->distance > end) && ((*headS)->cars != NULL) ){
            createPath(&(*headS), &(*endS), &(*headP));
        } else if( (*headS)->distance == end ){
            (*headS)->path = NULL;
            (*endS) = (*headS);
        }

        if ( (*headS)->distance <= start ){
            searchPathDecreasing( &((*headS)->right), &(*endS), start, end, &(*headP));
        }
    }
}


void createPath(station **start, station **end, arcPath **head){
    if( ((*start)->cars)->autonomy >= abs( (*end)->distance - (*start)->distance ) ){
        addPath(&(*head), &(*start), &(*end), 1);
    }
    else{
        arcPath *nodeCur = *head;
        arcPath *chosenPath = NULL;
        station *choseNodeS = NULL;
        station *nodePathCur = NULL;

        int numS = -1;
        int numCur = -1;
        int equal = 0;

        while( ( nodeCur != NULL ) && ( ((*start)->cars)->autonomy >= abs( (nodeCur->startPath)->distance - (*start)->distance ) ) ){
            if( numS == -1 ){
                numS = nodeCur->numStation;
                chosenPath = nodeCur;
            }

            nodePathCur = nodeCur->startPath;
            numCur = nodeCur->numStation;

            if( numS > numCur ){
                chosenPath = nodeCur;
                choseNodeS = NULL;
                numS = numCur;
            }
            while( (numCur - 1) >= 1 && ((*start)->cars)->autonomy >= abs( ((nodePathCur->path)->distance - (*start)->distance ) ) ){
                if( (numCur - 1) < numS ){
                    choseNodeS = nodePathCur->path;
                    chosenPath = NULL;
                    numS = numCur - 1;
                }
                nodePathCur = nodePathCur->path;
                --numCur;
            }

            if( numCur == numS ){
                if( choseNodeS != NULL && nodePathCur != choseNodeS ){
                    equal = searcBestPath( nodePathCur, choseNodeS, numS);
                    if( equal == 1 ) choseNodeS = nodePathCur;
                }
                else if( chosenPath != NULL && chosenPath->startPath != nodePathCur ){
                    equal = searcBestPath( nodePathCur, chosenPath->startPath, numS);
                    if( equal == 1 ) chosenPath = nodeCur;
                }
            }
            nodeCur = nodeCur->next;
        }

        if( chosenPath != NULL ){
            updateListPath(&(*head), &chosenPath, &(*start)); 
        }
        else if ( choseNodeS != NULL ){
            addPath(&(*head), &(*start), &choseNodeS, numS + 1);
        }
    }
}

int searcBestPath(station *firstPath, station *secondPath, int numStation){
    if( numStation >= 1){
        int end = searcBestPath( firstPath->path, secondPath->path, numStation - 1);
        if( end != 0 ) return end;
        else {
            if( firstPath->distance < secondPath->distance ) return 1;
            else if( firstPath->distance > secondPath->distance ) return 2;
            else return 0;
        }
    }
    return 0;
}

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
        if( node->path != NULL ) printf("%d ", node->distance );
        else printf("%d\n", node->distance );
        node = node->path;
    }
}