#include "game.h"
#include "type.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

int COUNTER = 0;

void capturePiecesIA(board gameboard, int x, int y, int dx, int dy, int color, int choix) {
    if (choix == 0) {
        percuter(gameboard, x, y, dx, dy, color);
    } else if (choix == 1) {
        aspirer(gameboard, x, y, dx, dy, color);
    } else if (x+dx>=0 && x+dx<gameboard.longueur && y+dy>=0 && y+dy<gameboard.hauteur && gameboard.plateau[x+dx][y+dy] == -color) { //Si je pousse vers une pièce adverse, je la percute
        percuter(gameboard, x, y, dx, dy, color);
    } else if (x-(2*dx)>=0 && x-(2*dx)<gameboard.longueur && y-(2*dy)>=0 && y-(2*dy)<gameboard.hauteur && gameboard.plateau[x-(2*dx)][y-(2*dy)]==-color) { //Sinon, je pousse vers une case vide, donc j'aspire les pièces adverses
        aspirer(gameboard, x, y, dx, dy, color);
    }
    else {
        printf("ERREUR: Capture de pièces impossible\n");
    }
}

int size_arrows(board gameboard, int i, int j, int color, int** already_passed_on, int cpt) {
    int sz = 0;
    if (canCapture(gameboard, i, j, already_passed_on)) {
        already_passed_on[i][j] = cpt++;
        bool tmp;
        bool* coups = jouable_under_condition(gameboard,i,j,&tmp,color,already_passed_on);
        for (int dir=0; dir<8; dir++) {
            if (coups[dir]) {
                int *x = malloc(sizeof(int));
                int *y = malloc(sizeof(int));
                assert(x!=NULL);
                assert(y!=NULL);
                *x = i; 
                *y = j;
                int** apo = copy_tab2D(already_passed_on, gameboard);
                board gb = copy_gameboard(gameboard);
                int dx = 0, dy = 0;
                dir_to_dx_dy(dir, &dx, &dy);
                movePiece(gb, x, y, dx, dy, color);
                if (*x+dx>=0 && *x+dx<gb.longueur && *y+dy>=0 && *y+dy<gb.hauteur && gb.plateau[*x+dx][*y+dy] == -color && *x-(2*dx)>=0 && *x-(2*dx)<gb.longueur && *y-(2*dy)>=0 && *y-(2*dy)<gb.hauteur && gb.plateau[*x-2*dx][*y-2*dy]==-color) {
                    board gb1 = copy_gameboard(gb);
                    board gb2 = copy_gameboard(gb);
                    int** apo1 = copy_tab2D(apo, gb);
                    int** apo2 = copy_tab2D(apo, gb);
                    capturePiecesIA(gb1, *x, *y, dx, dy, color, 0);
                    capturePiecesIA(gb2, *x, *y, dx, dy, color, 1);
                    int sz1 = size_arrows(gb1, *x, *y, color, apo1,cpt);
                    int sz2 = size_arrows(gb2, *x, *y, color, apo2,cpt);
                    sz += sz1+sz2;
                    free_tab2D(apo1, gb1);
                    free_tab2D(apo2, gb2);
                    free_gameboard(gb1);
                    free_gameboard(gb2);
                } else {
                    capturePiecesIA(gb, *x, *y, dx, dy, color, 2);
                    sz += size_arrows(gb, *x, *y, color, apo,cpt);
                }
                free_tab2D(apo, gb);
                free_gameboard(gb);
                free(x);
                free(y);
            }
        }
        if (cpt>=4) {
            sz++;
        }
    } else {
        return 1;
    }
    return sz;
}

void arrows(board gameboard, int i, int j, int color, int** already_passed_on, int cpt, board* res, int* index) {
    if (canCapture(gameboard, i, j, already_passed_on)) {
        already_passed_on[i][j] = cpt++;
        bool tmp;
        bool* coups = jouable_under_condition(gameboard,i,j,&tmp,color,already_passed_on);
        for (int dir=0; dir<8; dir++) {
            if (coups[dir]) {
                int *x = malloc(sizeof(int));
                int *y = malloc(sizeof(int));
                assert(x!=NULL);
                assert(y!=NULL);
                *x = i; 
                *y = j;
                int** apo = copy_tab2D(already_passed_on, gameboard);
                board gb = copy_gameboard(gameboard);
                int dx = 0, dy = 0;
                dir_to_dx_dy(dir, &dx, &dy);
                movePiece(gb, x, y, dx, dy, color);
                if (*x+dx>=0 && *x+dx<gb.longueur && *y+dy>=0 && *y+dy<gb.hauteur && gb.plateau[*x+dx][*y+dy] == -color && *x-(2*dx)>=0 && *x-(2*dx)<gb.longueur && *y-(2*dy)>=0 && *y-(2*dy)<gb.hauteur && gb.plateau[*x-2*dx][*y-2*dy]==-color) {
                    board gb1 = copy_gameboard(gb);
                    board gb2 = copy_gameboard(gb);
                    int** apo1 = copy_tab2D(apo, gb);
                    int** apo2 = copy_tab2D(apo, gb);
                    capturePiecesIA(gb1, *x, *y, dx, dy, color, 0);
                    capturePiecesIA(gb2, *x, *y, dx, dy, color, 1);
                    arrows(gb1, *x, *y, color, apo1,cpt, res, index);
                    arrows(gb2, *x, *y, color, apo2,cpt, res, index);
                    free_tab2D(apo1, gb1);
                    free_tab2D(apo2, gb2);
                    free_gameboard(gb1);
                    free_gameboard(gb2);
                } else {
                    capturePiecesIA(gb, *x, *y, dx, dy, color, 2);
                    arrows(gb, *x, *y, color, apo, cpt, res, index);
                }
                free_tab2D(apo, gb);
                free_gameboard(gb);
                free(x);
                free(y);
            }
        }
        if (cpt>=4) {
            res[*index] = copy_gameboard(gameboard);
            (*index)++;
        }
        free(coups);
        
    } else {
        res[*index] = copy_gameboard(gameboard);
        (*index)++;
    }
}

board* flatten_possibilites(board gameboard, int color, int* len, int* ind, bool endgame) { //input : gameboard avec pièces jouables
    int size = 0;
    board cleanboard = copy_gameboard(gameboard);
    clean_board(cleanboard);
    for (int i=0; i<gameboard.longueur; i++) {
        for (int j=0; j<gameboard.hauteur; j++) {
            if (gameboard.plateau[i][j] == 10+((1+color)/2)) {
                if (endgame) {
                    bool tmp;
                    bool* coups = jouable_endgame(gameboard, i, j, &tmp, color);
                    for (int dir=0; dir<8; dir++) {
                        if (coups[dir]) {
                            size++;
                        }
                    }

                } else {
                    int** already_passed_on = creer_tab2D(cleanboard);
                    size+=size_arrows(cleanboard, i, j, color, already_passed_on,2);
                    free_tab2D(already_passed_on, cleanboard);
                }
            }
        }
    }
    board* res = malloc(size*sizeof(board));
    assert(res!=NULL);
    int index = 0;
    for (int i=0; i<gameboard.longueur; i++) {
        for (int j=0; j<gameboard.hauteur; j++) {
            if (gameboard.plateau[i][j] == 10+((1+color)/2)) {
                if (endgame) {
                    bool tmp;
                    bool* coups = jouable_endgame(gameboard, i, j, &tmp, color);
                    for (int dir=0; dir<8; dir++) {
                        if (coups[dir]) {
                            int *x = malloc(sizeof(int));
                            int *y = malloc(sizeof(int));
                            *x = i; 
                            *y = j;
                            board gb = copy_gameboard(cleanboard);
                            int dx = 0, dy = 0;
                            dir_to_dx_dy(dir, &dx, &dy);
                            movePiece(gb, x, y, dx, dy, color);
                            res[index++] = gb;
                            free(x);
                            free(y);
                        }
                    }
                    free(coups);
                } else {
                    int** already_passed_on = creer_tab2D(cleanboard);
                    arrows(cleanboard, i, j, color, already_passed_on,2, res, &index);
                    free_tab2D(already_passed_on, cleanboard);
                }
            }
        }
    }
    free_gameboard(cleanboard);
    *ind = index;
    *len = size;
    return res;
}

void free_etats(board* etats, int len) {
    for (int i=0; i<len; i++) {
        free_gameboard(etats[i]);
    }
    free(etats);
}

int delta(int x1, int y1, int x2, int y2) {
    return abs(x1-x2)+abs(y1-y2);
}

//Modification de l'état du jeu par l'état désiré par l'ia
void state_to_gameboard(board gameboard, board state) {
    for (int i=0; i<gameboard.longueur; i++) {
        for (int j=0; j<gameboard.hauteur; j++) {
            gameboard.plateau[i][j] = state.plateau[i][j];
        }
    }
    *gameboard.nb_pion_blanc = *state.nb_pion_blanc;
    *gameboard.nb_pion_noir = *state.nb_pion_noir;
}

void free_where_are_the(board gameboard, int*** where_are_the, int len, int color) {
    for (int i=0; i<len; i++) {
        if (color==-1) {
            for(int j=0; j<(*gameboard.nb_pion_blanc); j++) {
                free(where_are_the[i][j]);
            }
        } else if (color==1) {
            for(int j=0; j<(*gameboard.nb_pion_noir); j++) {
                free(where_are_the[i][j]);
            }
        }
        free(where_are_the[i]);
    }
    free(where_are_the);
}

double alphabeta(board gb, int color, int depth, double alpha, double beta, int icolor, int iol, int* iom) {//TODO: Résoudre problème avec le cas iol>=len
    board cb = copy_gameboard(gb);
    clean_board(cb);
    bool is_end = is_endgame(cb,color);
    free_gameboard(cb);
    if (is_end || depth>=7) { //Match with leaf
        if (icolor==-1) {
            return (double)(*gb.nb_pion_blanc)/(*gb.nb_pion_noir);
        } else {
            return (double)(*gb.nb_pion_noir)/(*gb.nb_pion_blanc);
        }
    } else {
        int len=0;
        int ind=0;
        board* etats = flatten_possibilites(gb,color, &len, &ind, false);
        if (depth==0) {
             printf("%d / %d\n",iol,len);
        }
        assert(len>0);
        if (color==icolor) { //max node
            if (iol>=len) { //max node when the list is empty
                free_etats(etats,len);
                return alpha;
            } else { //max node when the list is not empty
                put_square(etats[iol],-color);
                double valeur = alphabeta(etats[iol],-color,depth+1,alpha,beta,icolor,0,0);
                double alpha1 = (alpha>valeur)?alpha:valeur;
                if (depth==0) {
                    if (valeur>alpha) {
                        *iom = iol;
                    }
                }
                if (alpha1>=beta) {
                    free_etats(etats,len);
                    return alpha1;
                } else {
                    if (depth==0) {
                        free_etats(etats,len);
                        return alphabeta(gb,color,depth,alpha1,beta,icolor,iol+1,iom);
                    }
                    free_etats(etats,len);
                    return alphabeta(gb,color,depth,alpha1,beta,icolor,iol+1,0);
                }
            }
        } else { //min node
            if (iol>=len) { //min node when the list is empty
                free_etats(etats,len);
                return beta;
            } else { //min node when the list is not empty
                put_square(etats[iol],-color);
                double valeur = alphabeta(etats[iol],-color,depth+1,alpha,beta,icolor,0,0);
                double beta1 = (beta<valeur)?beta:valeur;
                if (alpha>=beta1) {
                    free_etats(etats,len);
                    return beta1;
                } else {
                    free_etats(etats,len);
                    return alphabeta(gb,color,depth,alpha,beta1,icolor,iol+1,0);
                }
            }
        
        }
        free_etats(etats,len);
    }
}

void* ThreadedAlphaBeta(void* args) {//Threaded version of alphabeta
    ThreadedAlphaBetaArgs* targs = (ThreadedAlphaBetaArgs*) args;
    board gb = targs->gb;
    int color = targs->color;
    int depth = targs->depth;
    double alpha = targs->alpha;
    double beta = targs->beta;
    int icolor = targs->icolor;
    int iol = targs->iol;
    int* iom = targs->iom;
    board cb = copy_gameboard(gb);
    clean_board(cb);
    bool is_end = is_endgame(cb,color);
    free_gameboard(cb);
    int len=0;
    int ind=0;
    assert(!is_end);
    board* etats = flatten_possibilites(gb,color, &len, &ind, false);
    if (len==0) printf("LEN=0 IN THREAD\n");
    assert(len>0);
    if (color==icolor) { //max node
        if (iol>=len) { //max node when the list is empty
            double* alp = malloc(sizeof(double));
            *alp = alpha;
            free_etats(etats,len);
            return alp;
        } else { //max node when the list is not empty
            if (depth==0) {
                // Allocate memory for thread IDs, arguments, and results
                pthread_t* threads = malloc(sizeof(pthread_t) * len);
                ThreadedAlphaBetaArgs* args = malloc(sizeof(ThreadedAlphaBetaArgs) * len);
                double* results = malloc(sizeof(double) * len);

                // Create threads
                for (int i = 0; i < len; i++) {
                    put_square(etats[i],-color);
                    args[i].gb = etats[i];
                    args[i].color = -color;
                    args[i].depth = depth + 1;
                    args[i].alpha = alpha;
                    args[i].beta = beta;
                    args[i].icolor = icolor;
                    args[i].iol = 0;
                    args[i].iom = 0;
                    pthread_create(&threads[i], NULL, ThreadedAlphaBeta, &args[i]);
                }

                // Wait for threads to finish and collect results
                for (int i = 0; i < len; i++) {
                    double* result = malloc(sizeof(double));
                    pthread_join(threads[i], (void**)&result);
                    results[i] = *result;
                    free(result);
                }

                // Find the maximum value and its index
                double max_value = -1;
                int max_index = -1;
                for (int i = 0; i < len; i++) {
                    if (results[i] > max_value) {
                        max_value = results[i];
                        max_index = i;
                    }
                }

                // Store the index of the maximum value in iom
                *iom = max_index;

                // Free allocated memory
                free(threads);
                free(args);
                free(results);
                free_etats(etats,len);

                double* max_value_ptr = malloc(sizeof(double));
                *max_value_ptr = max_value;
                return max_value_ptr;
            }
        }
    } else { //min node
        if (iol>=len) { //min node when the list is empty
            double* bet = malloc(sizeof(double));
            *bet = beta;
            free_etats(etats,len);
            return bet;
        } else { //min node when the list is not empty
            put_square(etats[iol],-color);
            double valeur = alphabeta(etats[iol],-color,depth+1,alpha,beta,icolor,0,0);
            double beta1 = (beta<valeur)?beta:valeur;
            if (alpha>=beta1) {
                //free_etats(etats,len);
                double* beta2 = malloc(sizeof(double));
                *beta2 = beta1;
                free_etats(etats,len);
                return beta2;
            } else {
                //free_etats(etats,len);
                double* ret = malloc(sizeof(double));
                *ret = alphabeta(gb,color,depth,alpha,beta1,icolor,iol+1,0);
                free_etats(etats,len);
                return ret;
            }
        }
    
    }
    assert(false); //Should never reach this point
    return NULL;
}

void write_in_file(double** weights1, double** weights2) {
    FILE* f = fopen("neural_network_9_5","w");
    for (int i=0; i<45; i++) {
        for (int j=0; j<7; j++) {
            fprintf(f,"%#.12g ",weights1[i][j]);
        }
        fprintf(f,"\n");
    }
    for (int i=0; i<7; i++) {
        fprintf(f,"%#.12g ",weights2[i][0]);
    }
    fclose(f);
}

double ReLU(double x) {
    return (x>0)?x:0;
}

double ReLU_prime(double x) {
    return (x>0)?1:0;
}

double sigmoid(double x) {
    return 1/(1+exp(-x));
}

double sigmoid_prime(double x) {
    return sigmoid(x)*(1-sigmoid(x));
}

void backpropagation(board gb, double** weights1, double** weights2,double** weights3,double** weights4 ,double output, int color, double real_output) {
    double* input = malloc(45*sizeof(double));
    for (int i=0; i<45; i++) {
        if (gb.plateau[i%9][i/9] == color) {
            input[i] = 1;
        } else if (gb.plateau[i%9][i/9] == -color) {
            input[i] = -1;
        } else {
            input[i] = 0;
        }
    }
    double* hidden_layer1 = malloc(7*sizeof(double));
    double* hidden_layer2 = malloc(7*sizeof(double));
    double* hidden_layer3 = malloc(7*sizeof(double));
    for (int i=0; i<7; i++) {
        hidden_layer1[i] = 0;
        hidden_layer2[i] = 0;
        hidden_layer3[i] = 0;
    }

    for (int i=0; i<45; i ++) {
        for (int j=0; j<7; j++) {
            hidden_layer1[j] += input[i]*weights1[i][j];
        }
    }
    for (int i=0; i<7; i++) {
        hidden_layer1[i] = ReLU(hidden_layer1[i]);
    }
    for (int i=0; i<7; i ++) {
        for (int j=0; j<7; j++) {
            hidden_layer2[j] += hidden_layer1[i]*weights2[i][j];
        }
    }
    for (int i=0; i<7; i++) {
        hidden_layer2[i] = ReLU(hidden_layer2[i]);
    }
    for (int i=0; i<7; i ++) {
        for (int j=0; j<7; j++) {
            hidden_layer3[j] += hidden_layer2[i]*weights3[i][j];
        }
    }
    for (int i=0; i<7; i++) {
        hidden_layer3[i] = ReLU(hidden_layer3[i]);
    }
    for (int i=0; i<7; i++) {
        output += hidden_layer3[i]*weights4[i][0];
    }
    double delta_output = output - real_output;
    double* delta_hidden1 = malloc(7*sizeof(double));
    double* delta_hidden2 = malloc(7*sizeof(double));
    double* delta_hidden3 = malloc(7*sizeof(double));
    for (int i=0; i<7; i++) {
        delta_hidden1[i] = 0;
        delta_hidden2[i] = 0;
        delta_hidden3[i] = 0;
    }
    //retropopagation de l'erreur sur les couches cachées, sachant que delta_hidden3 est le dernier delta, donc le premier a calculer
    for (int i=0; i<7; i++) {
        delta_hidden3[i] = delta_output*weights4[i][0]*ReLU_prime(hidden_layer3[i]);
    }
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            delta_hidden2[i] += delta_hidden3[j]*weights3[i][j];
        }
        delta_hidden2[i] *= ReLU_prime(hidden_layer2[i]);
    }
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            delta_hidden1[i] += delta_hidden2[j]*weights2[i][j];
        }
        delta_hidden1[i] *= ReLU_prime(hidden_layer1[i]);
    }
    //Mise à jour des poids
    for (int i=0; i<45; i++) {
        for (int j=0; j<7; j++) {
            weights1[i][j] -= 0.1*delta_hidden1[j]*input[i];
        }
    }
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            weights2[i][j] -= 0.1*delta_hidden2[j]*hidden_layer1[i];
        }
    }
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            weights3[i][j] -= 0.1*delta_hidden3[j]*hidden_layer2[i];
        }
    }
    for (int i=0; i<7; i++) {
        weights4[i][0] -= 0.1*delta_output*hidden_layer3[i];
    }
    free(input);
    free(delta_hidden1);
    free(delta_hidden2);
    free(delta_hidden3);
    free(hidden_layer1);
    free(hidden_layer2);
    free(hidden_layer3);
    // printf("IMPRESSION DES POIDS\n");
    // for (int i=0; i<45; i++) {
    //     for (int j=0; j<7; j++) {
    //         printf("%lf ",weights1[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (int i=0; i<7; i++) {
    //     printf("%lf ",weights2[i][0]);
    // }
    // printf("\n");
}

double feedforward(board gb, double** weights1, double** weights2,double** weights3, double** weights4, int color) {
    double* input = malloc(45*sizeof(double));
    int len=0;
    int ind=0;
    for (int i=0; i<45; i++) {
        if (gb.plateau[i%9][i/9] == color) {
            input[i] = 1;
        } else if (gb.plateau[i%9][i/9] == -color) {
            input[i] = -1;
        } else {
            input[i] = 0;
        }
    }
    double* hidden_layer1 = malloc(7*sizeof(double));
    double* hidden_layer2 = malloc(7*sizeof(double));
    double* hidden_layer3 = malloc(7*sizeof(double));
    for (int i=0; i<7; i++) {
        hidden_layer1[i] = 0;
        hidden_layer2[i] = 0;
        hidden_layer3[i] = 0;
    }
    for (int i=0; i<45; i ++) {
        for (int j=0; j<7; j++) {
            hidden_layer1[j] += input[i]*weights1[i][j];
        }
    }
    for (int i=0; i<7; i++) {
        hidden_layer1[i] = ReLU(hidden_layer1[i]);
    }
    for (int i=0; i<7; i ++) {
        for (int j=0; j<7; j++) {
            hidden_layer2[j] += hidden_layer1[i]*weights2[i][j];
        }
    }
    for (int i=0; i<7; i++) {
        hidden_layer2[i] = ReLU(hidden_layer2[i]);
    }
    for (int i=0; i<7; i ++) {
        for (int j=0; j<7; j++) {
            hidden_layer3[j] += hidden_layer2[i]*weights3[i][j];
        }
    }
    for (int i=0; i<7; i++) {
        hidden_layer3[i] = ReLU(hidden_layer3[i]);
    }
    double output = 0;
    for (int i=0; i<7; i++) {
        output += hidden_layer3[i]*weights4[i][0];
    }
    //No activation function for the output layer
    free(input);
    free(hidden_layer1);
    free(hidden_layer2);
    free(hidden_layer3);
    if (isnan(output)) {
        printf("IMPRESSION DES POIDS\n");
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                printf("%lf ",weights1[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        for (int i=0; i<7; i++) {
            printf("%lf ",weights2[i][0]);
        }
        printf("\n");
        exit(1);
    }
    return output;
}

double neural_network(board gb, int color) {
    //open a file name "neural_network_{width}_{height}" where width and height are the dimensions of the gameboard and read the weights
    FILE* f = fopen("neural_network_9_5","r");
    if (f==NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier neural_network_9_5\n");
        return -1;
    }
    //The neural network will be composed of 3 layers, the input layer of size 9*5, the hidden layer of size 7 and the output layer of size 1
    //The weights will be stored in a 45*7 matrix and a 7*1 matrix
    //If the file is empty, create random weights
    if(fgetc(f)==EOF) {
        double** weights1 = malloc(45*sizeof(double*));
        double** weights2 = malloc(7*sizeof(double*));
        double** weights3 = malloc(7*sizeof(double*));
        double** weights4 = malloc(7*sizeof(double*));
        for (int i=0; i<45; i++) {
            weights1[i] = malloc(7*sizeof(double));
        }
        for (int i=0; i<7; i++) {
            weights2[i] = malloc(sizeof(double));
            weights3[i] = malloc(7*sizeof(double));
            weights4[i] = malloc(7*sizeof(double));
        }
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                weights1[i][j] = (double)rand()/(double)RAND_MAX;
            }
        }
        for (int i=0; i<7; i++) {
            for (int j=0; j<7; j++) {
                weights2[i][j] = (double)rand()/(double)RAND_MAX;
                weights3[i][j] = (double)rand()/(double)RAND_MAX;
            }
            weights4[i][0] = (double)rand()/(double)RAND_MAX;
        }
        fclose(f);
        f = fopen("neural_network_9_5","w");
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                fprintf(f,"%lf ",weights1[i][j]);
            }
            fprintf(f,"\n");
        }
        for (int i=0; i<7; i++) {
            for (int j=0; j<7; j++) {
                fprintf(f,"%lf ",weights2[i][j]);
            }
            fprintf(f,"\n");
        }
        for (int i=0; i<7; i++) {
            for (int j=0; j<7; j++) {
                fprintf(f,"%lf ",weights3[i][j]);
            }
            fprintf(f,"\n");
        }
        for (int i=0; i<7; i++) {
            fprintf(f,"%lf ",weights4[i][0]);
        }
        fclose(f);
    }
    double** weights1 = malloc(45*sizeof(double*));
    double** weights2 = malloc(7*sizeof(double*));
    double** weights3 = malloc(7*sizeof(double*));
    double** weights4 = malloc(7*sizeof(double*));
    for (int i=0; i<45; i++) {
        weights1[i] = malloc(7*sizeof(double));
    }
    for (int i=0; i<7; i++) {
        weights2[i] = malloc(7*sizeof(double));
    }
    for (int i=0; i<7; i++) {
        weights3[i] = malloc(7*sizeof(double));
    }
    for (int i=0; i<7; i++) {
        weights4[i] = malloc(sizeof(double));
    }
    for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                fscanf(f,"%lf ",&weights1[i][j]);
            }
        }
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            fscanf(f,"%lf ",&weights2[i][j]);
        }
    }
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            fscanf(f,"%lf ",&weights3[i][j]);
        }
    }
    for (int i=0; i<7; i++) {
        fscanf(f,"%lf ",&weights4[i][0]);
    }
    fclose(f);
    return feedforward(gb,weights1,weights2,weights3,weights4,color);
}

double minimax_training_real_score(board gb, int color, int depth, int max_depth, int icolor, double** weights1, double** weights2, double** weights3, double** weights4, int* node_counter) {
    board cb = copy_gameboard(gb);
    clean_board(cb);
    bool is_end = is_endgame(cb,color);
    put_square(gb,color);
    if (depth>=max_depth || *gb.nb_pion_blanc == 0 || *gb.nb_pion_noir == 0) { //Match with leaf
        *node_counter = *node_counter +1;
        if (*gb.nb_pion_blanc==0) {
            for (int i=0; i<2*max_depth; i++) {
                    backpropagation(cb,weights1,weights2,weights3, weights4,feedforward(cb,weights1,weights2,weights3, weights4,-1),-1,-1);
                    backpropagation(cb,weights1,weights2,weights3, weights4,feedforward(cb,weights1,weights2,weights3, weights4,1),1,1);
            }
            if (icolor == -1) {
                free_gameboard(cb);
                return -1;
            } else {
                free_gameboard(cb);
                return 1;
            }
        } else if (*gb.nb_pion_noir==0) {
            for (int i=0; i<2*max_depth; i++) {
                    backpropagation(cb,weights1,weights2,weights3, weights4,feedforward(gb,weights1,weights2,weights3, weights4,-1),-1,1);
                    backpropagation(cb,weights1,weights2,weights3, weights4,feedforward(cb,weights1,weights2,weights3, weights4,1),1,-1);
            }
            if (icolor == 1) {
                free_gameboard(cb);
                return -1;
            } else {
                free_gameboard(cb);
                return 1;
            }
        } else {
            backpropagation(cb,weights1,weights2,weights3, weights4,feedforward(cb,weights1,weights2,weights3, weights4,color),color,0);
            return 0;
        }
    } else {
        int len=0;
        int ind=0;
        board* etats = flatten_possibilites(gb,color, &len, &ind, is_end);
        if (len==0) {
            printf("Is endgame = %d, nbpionB = %d, nbpionN = %d \n",is_end, *gb.nb_pion_blanc, *gb.nb_pion_noir);
            print_game(gb);
        }
        assert(len>0);
        if (color==icolor) { //max node
            double max = -1;
            for (int i=0; i<len; i++) {
                int tmp = 0;
                double score = minimax_training_real_score(etats[i],-color,depth+1,max_depth, icolor, weights1, weights2,weights3, weights4, &tmp);
                *node_counter = *node_counter + tmp;
                if (score>max) {
                    max = score;
                }
            }
            free_etats(etats,len);
            int c = 1 + (int)floor(log2(*node_counter+1));
            if (max!=0) {
                    printf("================ max = %lf ===========\n",max);
                }
            if (max == 0) {
                double output = feedforward(cb,weights1,weights2,weights3, weights4,color);
                printf("Predit = %lf, Réel = %lf\n",output,max);
                backpropagation(cb,weights1,weights2,weights3, weights4,output,color,max);
            } else {
                for (int i=0; i<c; i++) {
                    double output = feedforward(cb,weights1,weights2,weights3, weights4,color);
                    printf("Predit = %lf, Réel = %lf\n",output,max);
                    backpropagation(cb,weights1,weights2,weights3, weights4,output,color,max);
                }
            }
            COUNTER++;
            free_gameboard(cb);
            return max;
        } else { //min node
            double min = 1;
            for (int i=0; i<len; i++) {
                int tmp = 0;
                double score = minimax_training_real_score(etats[i],-color,depth+1,max_depth, icolor, weights1, weights2,weights3, weights4, &tmp);
                *node_counter = *node_counter + tmp;
                if (score<min) {
                    min = score;
                }
            }
            free_etats(etats,len);
            int c = 1 + (int)floor(log2(*node_counter+1));
            // for (int i=0; i<c; i++) {
            //     double output = feedforward(cb,weights1,weights2,color);
            //     if (min!=0) {
            //         printf("============== min = %lf ===============\n",min);
            //     }
            //     printf("Predit = %lf, Réel = %lf\n",output,min);
            //     backpropagation(cb,weights1,weights2,output,color,min);
            // }
            COUNTER++;
            if (COUNTER>1000) {
                //write_in_file(weights1,weights2);
                exit(1);
            }
            free_gameboard(cb);
            return min;
        }
    }
}

void search(board gb, int color, double** weights1, double** weights2, double** weights3, double** weights4) {
    bool is_end = is_endgame(gb,color);
    put_square(gb,color);
    int len=0;
    int ind=0;
    board* etats = flatten_possibilites(gb,color, &len, &ind, is_end);
    if (len==0) {
        printf("Is endgame = %d\n",is_end);
        print_game(gb);
    }
    assert(len>0);
    if (is_end) {
        int nc = 0;
        minimax_training_real_score(gb,color,0,20,color,weights1,weights2,weights3,weights4,&nc);
    }
    else {
        for (int i=0; i<len; i++) {
            search(etats[i],-color,weights1,weights2, weights3, weights4);
            exit(1);
        }
        free_etats(etats,len);
    }
}

void training(board gb, int color) {
    FILE* f = fopen("neural_network_9_5","r");
    assert(f!=NULL);
    if(fgetc(f)==EOF) {
        double** weights1 = malloc(45*sizeof(double*));
        double** weights2 = malloc(7*sizeof(double*));
        double** weights3 = malloc(7*sizeof(double*));
        double** weights4 = malloc(7*sizeof(double*));
        for (int i=0; i<45; i++) {
            weights1[i] = malloc(7*sizeof(double));
        }
        for (int i=0; i<7; i++) {
            weights2[i] = malloc(sizeof(double));
            weights3[i] = malloc(7*sizeof(double));
            weights4[i] = malloc(7*sizeof(double));
        }
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                weights1[i][j] = (double)rand()/(double)RAND_MAX;
            }
        }
        for (int i=0; i<7; i++) {
            for (int j=0; j<7; j++) {
                weights2[i][j] = (double)rand()/(double)RAND_MAX;
                weights3[i][j] = (double)rand()/(double)RAND_MAX;
            }
            weights4[i][0] = (double)rand()/(double)RAND_MAX;
        }
        fclose(f);
        f = fopen("neural_network_9_5","w");
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                fprintf(f,"%lf ",weights1[i][j]);
            }
            fprintf(f,"\n");
        }
        for (int i=0; i<7; i++) {
            for (int j=0; j<7; j++) {
                fprintf(f,"%lf ",weights2[i][j]);
            }
            fprintf(f,"\n");
        }
        for (int i=0; i<7; i++) {
            for (int j=0; j<7; j++) {
                fprintf(f,"%lf ",weights3[i][j]);
            }
            fprintf(f,"\n");
        }
        for (int i=0; i<7; i++) {
            fprintf(f,"%lf ",weights4[i][0]);
        }
        fclose(f);
    }
    double** weights1 = malloc(45*sizeof(double*));
    double** weights2 = malloc(7*sizeof(double*));
    double** weights3 = malloc(7*sizeof(double*));
    double** weights4 = malloc(7*sizeof(double*));
    for (int i=0; i<45; i++) {
        weights1[i] = malloc(7*sizeof(double));
    }
    for (int i=0; i<7; i++) {
        weights2[i] = malloc(7*sizeof(double));
    }
    for (int i=0; i<7; i++) {
        weights3[i] = malloc(7*sizeof(double));
    }
    for (int i=0; i<7; i++) {
        weights4[i] = malloc(sizeof(double));
    }
    for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                fscanf(f,"%lf ",&weights1[i][j]);
            }
        }
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            fscanf(f,"%lf ",&weights2[i][j]);
        }
    }
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            fscanf(f,"%lf ",&weights3[i][j]);
        }
    }
    for (int i=0; i<7; i++) {
        fscanf(f,"%lf ",&weights4[i][0]);
    }
    fclose(f);
    // printf("IMPRESSION DES POIDS\n");
    // for (int i=0; i<45; i++) {
    //     for (int j=0; j<7; j++) {
    //         printf("%lf ",weights1[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (int i=0; i<7; i++) {
    //     printf("%lf ",weights2[i][0]);
    // }
    // printf("\n");
    fclose(f);
    search(gb,color,weights1,weights2, weights3, weights4);
}

void ia(board gameboard, int color, bool endgame) {
    srand(time(NULL));
    int len=0;
    int ind=0;
    board* etats = flatten_possibilites(gameboard,color, &len, &ind, endgame); //IN: gameboard avec case carrées
    //printf("Voici tous les états accessible depuis l'état actuel, il y en a %d / %d :\n",ind,len);
    if (endgame) {
        double max = -1;
        int indexofmax = 0;
        for (int i=0; i<len; i++) {
            double score = neural_network(etats[i],color);
            if (score>max) {
                max = score;
                indexofmax = i;
            }
        }
        state_to_gameboard(gameboard,etats[indexofmax]);
    } else {
        int indexofmax=0;
        ThreadedAlphaBetaArgs* args = malloc(sizeof(ThreadedAlphaBetaArgs));
        args->gb = gameboard;
        args->color = color;
        args->depth = 0;
        args->alpha = -1;
        args->beta = 9*5;
        args->icolor = color;
        args->iol = 0;
        args->iom = &indexofmax;
        void* res = ThreadedAlphaBeta(args);
        double res1 = *((double*)res);
        printf("SCORE = %lf, chosen = %d\n",res1, indexofmax);
        state_to_gameboard(gameboard,etats[indexofmax]);
        free(args);
        free((double*)res);
    }
    free_etats(etats,len);
    printf("L'ia a joué:\n");
}

// 9 5 4 3 1 3 2 3 0 5 6 3 1 7 1 3 1 2 3 1 4 5 1 2 1 3 5 8 3 1 0 5