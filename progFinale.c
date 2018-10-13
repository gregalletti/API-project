#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARRAY_SIZE 75
#define MAX_IN 30000
#define INC_TAPE 128
#define ARRAY_INC 16
#define DIM_CODA 512

/*
 * Structure that contains an array to indicize the letters.
 */
typedef struct p{
    struct t * table[ARRAY_SIZE];
}pointer;

/*
 * Transition structure, contains all the information needed to perform a move.
 */
typedef struct t{
    char out;
    char move;
    int nextState;
    struct t * nextTransition;
}transition;

/*
 * Tape structure, contains the string and the number of pointers to this tape.
 */
typedef struct na{
    char * stringa;
    int pointers;
}nastro;

/*
 * Node structure, contains all the information needed to handle the non determinism.
 */
typedef struct n{
    struct na * tape;
    int index;
    int currentState;
    long level;
    int lenght;
}node;

/*
 * Buffer where the input strings are saved, before the malloc with their specific length.
 */
char * inputBuffer;
long max_counter;
int * q;
pointer ** adjList;
char * input;
int finalConf[3];
int stateNumber = 0;
int adjDim = 0;
int inputDim;
node * coda[DIM_CODA];
int q_head = 0;
int q_tail = 0;

void insertList(int origin, int dest, char leggo, char scrivo, char muovo);

void run();

void printResult();

void reset();

int getIndexFromChar(char in);

void initialize(int newDim);


int main() {

    char line[MAX_IN];

    int origin = 0;
    int dest = 0;
    int i;
    int readTr = 1;
    int readAc = 1;
    int acState;
    char leggo = ' ';
    char scrivo = ' ';
    char muovo = ' ';

    finalConf[0] = 0;
    finalConf[1] = 0;
    finalConf[2] = 0;

    scanf("%s", line);

    adjList = (pointer**) malloc(sizeof(pointer*));

    adjList[0] = NULL;

//--------------------LEGGO LA FUNZIONE DI TRANSIZIONE----------------------------------------------------------------------------

    if(strcmp(line, "tr") == 0){
        while (readTr){
            i = 0;
            while(i <= 4){
                scanf("%s", line);

                if(strcmp(line,"acc") != 0){
                    if(i == 0) {
                        origin = atoi(line);
                        if (origin > stateNumber)
                            stateNumber = origin;
                    }
                    else if(i == 4) {
                        dest = atoi(line);
                        if (dest > stateNumber)
                            stateNumber = dest;
                    }
                    else if(i == 1)
                        leggo = line[0];
                    else if(i == 2)
                        scrivo = line[0];
                    else
                        muovo = line[0];
                    i++;
                }
                else {
                    readTr = 0;
                    break;
                }
            }
            if(readTr) {
                if (origin > adjDim)
                    initialize(origin);
                insertList(origin, dest, leggo, scrivo, muovo);
            }
        }
    }
    //creo il vettore di stati in modo dinamico, in base al numero di stati totali della macchina letta
    q = (int*) calloc(stateNumber + 1, sizeof(int));
//--------------------LEGGO GLI STATI DI ACCETTAZIONE-----------------------------------------------------------------------------

    while (readAc){
        scanf("%s", line);

        if(strcmp(line,"max") != 0){
            acState = atoi(line);
            q[acState] = 1;
        }
        else {
            readAc = 0;
        }
    }

//--------------------LEGGO IL N? MASSIMO DI PASSI--------------------------------------------------------------------------------

    scanf("%li", &max_counter);

//--------------------LEGGO L'INPUT-----------------------------------------------------------------------------------------------

    scanf("%s", line);
    int firstLine = 1;

    inputBuffer = (char*) malloc(MAX_IN*sizeof(char));

    /*
     * creo la coda di 64 nodi, alloco tutto (tranne il nastro), la free viene fatta alla fine
     */
    for (int j = 0; j < DIM_CODA; j++) {
        coda[j] = malloc(sizeof(node));
        coda[j]->currentState = -1;
        coda[j]->tape = NULL;
    }

    while(scanf("%s", inputBuffer) != EOF){

        inputDim = (int) strlen(inputBuffer);

        coda[0]->currentState = 0;
        coda[0]->index = 0;
        coda[0]->level = 0;
        coda[0]->tape = (nastro*) malloc(sizeof(nastro));
        coda[0]->tape->pointers = 1;
        coda[0]->tape->stringa = malloc(sizeof(char) * (inputDim + 1));
        strcpy(coda[0]->tape->stringa, inputBuffer);
        coda[0]->lenght = inputDim;

        run();

        if(!firstLine)
            printf("\n");

        printResult();
        reset();

        firstLine = 0;

        free(input);
    }
    free(inputBuffer);
    return 0;

}

void initialize(int newDim) {

    int x = newDim + ARRAY_INC;
    adjList = (pointer**) realloc(adjList , sizeof(pointer*) * (x + 1));
    for (int i = adjDim + 1; i < x + 1; i++)
        adjList[i] = NULL;
    adjDim = x;

}

int parseMove(int index, char move) {

    switch (move){
        case 'R':
            return index + 1;
        case 'L':
            return index - 1;
        default:
            return index;
    }
}


void run() {

    char toCheck;
    transition * currentT;
    node * temp = coda[0];
    int existT = 0;
    int exceedL = 0;
    int exceedR = 0;
    int tempIndex = 0;
    int indexX = 0;
    while (temp->currentState != -1) {

        if (temp->level == max_counter){
            finalConf[2]++;
            return;
        }

        toCheck = temp->tape->stringa[temp->index];

        currentT = NULL;

        if (adjList[temp->currentState] != NULL)
            currentT = adjList[temp->currentState]->table[getIndexFromChar(toCheck)];

        while (currentT != NULL){
            //se sono qua dentro significa che ho almeno una transizione quindi
            existT = 1;

            if (q[currentT->nextState]) {
                finalConf[1]++;
                return;
            }

            if((toCheck == '_' && currentT->move == 'R' && temp->index == temp->lenght && currentT->nextState == temp->currentState) || (toCheck == '_' && currentT->move == 'L' && temp->index == 0 && currentT->nextState == temp->currentState) || (toCheck == currentT->out && currentT->move == 'S' && currentT->nextState == temp->currentState)) {
                finalConf[2]++;
                break;
            }
                q_tail++;
            if (q_tail == DIM_CODA)
                q_tail = 0;

            indexX = temp->index;

            coda[q_tail]->currentState = currentT->nextState;
            coda[q_tail]->level = temp->level + 1;
            tempIndex = parseMove(indexX, currentT->move);
            coda[q_tail]->lenght = temp->lenght;

            if (tempIndex == -1)
                exceedL = 1;
            if (tempIndex == temp->lenght)
                exceedR = 1;

            /*
             * Lavoro sempre su q_tail dato che sto inserendo in coda in modo ciclico.
             * Ho queste situazioni possibili:
             * 1) T DETERMINISTICA: faccio puntare il nastro passato a coda[q_tail], e cosi posso modificare direttamente
             * quel nastro se sforo a destra o a sinistra. Poi scrivo quello che devo scrivere per la transizione.
             * 2) T NON DETERMINISTICA:
             *      - se non sforo a destra o a sinistra creo una nuova struct nastro copiando quella passata
             *      come parametro, poi scrivo output.
             *      - se sforo creo una nuova struct nastro con già le modifiche fatte (blank a dx o a sx) e poi ci copio
             *      dentro la stringa passata come parametro. poi output.
             */

            if (currentT->nextTransition == NULL){ //significa che è l'ultima transizione oppure che è det                newTape->pointers++;
                if (exceedL){
                    //rialloco stringa a sx
                    char tempString[temp->lenght + 1 + INC_TAPE];
                    strcpy(tempString, "________________________________________________________________________________________________________________________________");
                    strcat(tempString, temp->tape->stringa);
                    temp->tape->stringa = realloc(temp->tape->stringa, sizeof(char) * (temp->lenght + 1 + INC_TAPE));
                    strcpy(temp->tape->stringa, tempString);
                    coda[q_tail]->lenght += INC_TAPE;
                    tempIndex += INC_TAPE;
                    indexX += INC_TAPE;
                }
                else if (exceedR){
                    temp->tape->stringa = realloc(temp->tape->stringa, sizeof(char) * (temp->lenght + 1 + INC_TAPE));
                    strcat(temp->tape->stringa, "________________________________________________________________________________________________________________________________");
                    coda[q_tail]->lenght += INC_TAPE;

                }
                temp->tape->pointers++;
                coda[q_tail]->tape = temp->tape;
                coda[q_tail]->index = tempIndex;
                coda[q_tail]->tape->stringa[indexX] = currentT->out;
            }
            else{
                nastro * nuovo = malloc(sizeof(nastro));
                nuovo->pointers = 1;

                if (exceedL){
                    nuovo->stringa = malloc(sizeof(char) * (temp->lenght + 1 + INC_TAPE));
                    strcpy(nuovo->stringa, "________________________________________________________________________________________________________________________________");
                    strcat(nuovo->stringa, temp->tape->stringa);
                    tempIndex += INC_TAPE;
                    indexX += INC_TAPE;
                    coda[q_tail]->lenght += INC_TAPE;

                }
                else if (exceedR){

                    nuovo->stringa = malloc(sizeof(char) * (temp->lenght + 1 + INC_TAPE));
                    strcpy(nuovo->stringa, temp->tape->stringa);
                    strcat(nuovo->stringa, "________________________________________________________________________________________________________________________________");
                    coda[q_tail]->lenght += INC_TAPE;

                }
                else{
                    nuovo->stringa = malloc(sizeof(char) * (temp->lenght + 1));
                    strcpy(nuovo->stringa, temp->tape->stringa);
                }

                coda[q_tail]->tape = nuovo;
                coda[q_tail]->index = tempIndex;
                coda[q_tail]->tape->stringa[indexX] = currentT->out;
            }

            exceedL = 0;
            exceedR = 0;

            currentT = currentT->nextTransition;
        }

        if(!existT)
            finalConf[0]++;

        temp->currentState = -1;
        temp->tape->pointers--;

        if (temp->tape->pointers == 0) {
            free(temp->tape->stringa);
            temp->tape->stringa = NULL;
            free(temp->tape);
            temp->tape = NULL;
        }

        temp->tape = NULL;

        q_head++;
        if (q_head == DIM_CODA)
            q_head = 0;

        temp = coda[q_head];
    }
}

void printResult() {

    if(finalConf[1] > 0){
        printf("1");
        return;
    }

    if(finalConf[2] > 0){
        printf("U");
        return;
    }

    printf("0");

}

void reset() {

    q_head = 0;
    q_tail = 0;
    finalConf[0] = 0;
    finalConf[1] = 0;
    finalConf[2] = 0;

}

void insertList(int origin, int dest, char leggo, char scrivo, char muovo) {

    transition * newNode = (transition*) malloc(sizeof(transition));

    if (adjList[origin] == NULL) {
        adjList[origin] = malloc(sizeof(pointer));
        for (int c = 0; c < ARRAY_SIZE; c++) {
            adjList[origin]->table[c] = NULL;
        }
    }

    newNode->out = scrivo;
    newNode->move = muovo;
    newNode->nextState = dest;

    if (adjList[origin]->table[getIndexFromChar(leggo)] == NULL) {
        newNode->nextTransition = NULL;
        adjList[origin]->table[getIndexFromChar(leggo)] = newNode;

    }
    else {
        newNode->nextTransition = adjList[origin]->table[getIndexFromChar(leggo)];
        adjList[origin]->table[getIndexFromChar(leggo)] = newNode;
    }

}

int getIndexFromChar(char in){
    return (int)in - 48;
}
