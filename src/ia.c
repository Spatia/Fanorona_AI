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
extern bool endgame_occured_once;

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

double alphabeta(board gb, int color, int depth, double alpha, double beta, int icolor, int iol, int* iom) {//Alpha beta non threadé
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

void write_in_file(double** weights1, double** weights2) {
    FILE* f = fopen("neural_network_9_5","w");
    for (int i=0; i<45; i++) {
        for (int j=0; j<7; j++) {
            fprintf(f,"%#.32g ",weights1[i][j]);
        }
        fprintf(f,"\n");
    }
    for (int i=0; i<7; i++) {
        fprintf(f,"%#.32g ",weights2[i][0]);
    }
    fclose(f);
}

void read_weights(double** weights1, double** weights2) {
    FILE* f = fopen("neural_network_9_5","r");
    if (f==NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier neural_network_9_5\n");
        exit(1);
    }
    for (int i=0; i<45; i++) {
        for (int j=0; j<7; j++) {
            fscanf(f,"%lf",&weights1[i][j]);
        }
    }
    for (int i=0; i<7; i++) {
        fscanf(f,"%lf",&weights2[i][0]);
    }
    fclose(f);
}

//Fonctions d'activation

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

void backpropagation(board gb, double** weights1, double** weights2, double output, int color, double real_output) {
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
    double* hidden_layer = malloc(7*sizeof(double));
    for (int i=0; i<7; i++) {
        hidden_layer[i] = 0;
    }
    for (int i=0; i<45; i ++) {
        for (int j=0; j<7; j++) {
            hidden_layer[j] += input[i]*weights1[i][j];
        }
    }
    for (int i=0; i<7; i++) {
        hidden_layer[i] = sigmoid(hidden_layer[i]);
    }
    double delta_output = output - real_output;
    double lr=0.00001;
    double* delta_hidden = malloc(7*sizeof(double));
    for (int i=0; i<7; i++) {
        delta_hidden[i] = 0;
    }
    for (int i=0; i<7; i++) {
        delta_hidden[i] = delta_output*weights2[i][0]*sigmoid_prime(hidden_layer[i]);
    }
    for (int i=0; i<7; i++) {
        weights2[i][0] -= lr*delta_output*hidden_layer[i];
    }
    for (int i=0; i<45; i++) {
        for (int j=0; j<7; j++) {
            weights1[i][j] -= lr*delta_hidden[j]*input[i];
        }
    }
    free(input);
    free(hidden_layer);
    free(delta_hidden);
}

double feedforward(board gb, double** weights1, double** weights2, int color) {
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
    double* hidden_layer = malloc(7*sizeof(double));
    for (int i=0; i<7; i++) {
        hidden_layer[i] = 0;
    }
    for (int i=0; i<45; i ++) {
        for (int j=0; j<7; j++) {
            hidden_layer[j] += input[i]*weights1[i][j];
        }
    }
    for (int i=0; i<7; i++) {
        hidden_layer[i] = sigmoid(hidden_layer[i]);
    }
    double output = 0;
    for (int i=0; i<7; i++) {
        output += hidden_layer[i]*weights2[i][0];
    }
    //Pas de fonction d'activation pour la couche de sortie (on veut un resultat reel)
    free(input);
    free(hidden_layer);
    if (isnan(output)) {
        for (int i=0; i<7; i++) {
            printf("%lf, hl = %lf, w =%lf \n",hidden_layer[i]*weights2[i][0], hidden_layer[i], weights2[i][0]);
        }
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
    //ouvre un fichier nommé "neural_network_{width}_{height}" où la largeur et la longueur sont les dimensions du plateau
    FILE* f = fopen("neural_network_9_5","r");
    if (f==NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier neural_network_9_5\n");
        return -1;
    }
    if(fgetc(f)==EOF) {
        double** weights1 = malloc(45*sizeof(double*));
        double** weights2 = malloc(7*sizeof(double*));
        for (int i=0; i<45; i++) {
            weights1[i] = malloc(7*sizeof(double));
        }
        for (int i=0; i<7; i++) {
            weights2[i] = malloc(sizeof(double));
        }
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                weights1[i][j] = (double)rand()/(double)RAND_MAX;
            }
        }
        for (int i=0; i<7; i++) {
            weights2[i][0] = (double)rand()/(double)RAND_MAX;
        }
        fclose(f);
        f = fopen("neural_network_9_5","w");
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                fprintf(f,"%#.32g ",weights1[i][j]);
            }
            fprintf(f,"\n");
        }
        for (int i=0; i<7; i++) {
            fprintf(f,"%#.32g ",weights2[i][0]);
        }
        fclose(f);
    }
    double** weights1 = malloc(45*sizeof(double*));
    double** weights2 = malloc(7*sizeof(double*));
    for (int i=0; i<45; i++) {
        weights1[i] = malloc(7*sizeof(double));
    }
    for (int i=0; i<7; i++) {
        weights2[i] = malloc(sizeof(double));
    }
    for (int i=0; i<45; i++) {
        for (int j=0; j<7; j++) {
            fscanf(f,"%lf",&weights1[i][j]);
        }
    }
    for (int i=0; i<7; i++) {
        fscanf(f,"%lf",&weights2[i][0]);
    }
    fclose(f);
    return feedforward(gb,weights1,weights2,color);
}

double minimax_training_real_score(board gb, int color, int depth, int max_depth, int icolor, double** weights1, double** weights2, int* node_counter) { //Permet de donner le vrai score du plateau, donné en un temps peu raisonnable
    if (depth==1) {
        printf("1!\n");
    }
    board cb = copy_gameboard(gb);
    clean_board(cb);
    bool is_end = is_endgame(cb,color);
    put_square(gb,color);
    if (depth>=max_depth || *gb.nb_pion_blanc == 0 || *gb.nb_pion_noir == 0) { //Match with leaf
        *node_counter = *node_counter +1;
        if (*gb.nb_pion_blanc==0) {
            for (int i=0; i<2*max_depth; i++) {
                    backpropagation(cb,weights1,weights2,feedforward(cb,weights1,weights2,-1),-1,-1);
                    backpropagation(cb,weights1,weights2,feedforward(cb,weights1,weights2,1),1,1);
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
                    backpropagation(cb,weights1,weights2,feedforward(gb,weights1,weights2,-1),-1,1);
                    backpropagation(cb,weights1,weights2,feedforward(cb,weights1,weights2,1),1,-1);
            }
            if (icolor == 1) {
                free_gameboard(cb);
                return -1;
            } else {
                free_gameboard(cb);
                return 1;
            }
        } else {
            backpropagation(cb,weights1,weights2,feedforward(cb,weights1,weights2,color),color,0);
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
                double score = minimax_training_real_score(etats[i],-color,depth+1,max_depth, icolor, weights1, weights2, &tmp);
                *node_counter = *node_counter + tmp;
                if (score>max) {
                    max = score;
                }
            }
            free_etats(etats,len);
            int c = 1 + (int)floor(log2(*node_counter+1));
            if (max!=0) {
                for (int i=0; i<c; i++) {
                    double output = feedforward(cb,weights1,weights2,color);
                    backpropagation(cb,weights1,weights2,output,color,max);
                }
            }
            COUNTER++;
            if (COUNTER>100000) {
                write_in_file(weights1,weights2);
            }
            free_gameboard(cb);
            return max;
        } else { //min node
            double min = 1;
            for (int i=0; i<len; i++) {
                int tmp = 0;
                double score = minimax_training_real_score(etats[i],-color,depth+1,max_depth, icolor, weights1, weights2, &tmp);
                *node_counter = *node_counter + tmp;
                if (score<min) {
                    min = score;
                }
            }
            free_etats(etats,len);
            int c = 1 + (int)floor(log2(*node_counter+1));
            if (min!=0) {
                for (int i=0; i<c; i++) {
                    double output = feedforward(cb,weights1,weights2,color);
                    backpropagation(cb,weights1,weights2,output,color,-min);
                }
            }
            COUNTER++;
            if (COUNTER>100000) {
                COUNTER = 0;
                write_in_file(weights1,weights2);
            }
            free_gameboard(cb);
            return min;
        }
    }
}

void search(board gb, int color, double** weights1, double** weights2) { //Réalise l'exploration de l'arbre de jeu jusqu'à tomber sur un état de jeu endgame
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
        minimax_training_real_score(gb,color,0,7,color,weights1,weights2,&nc);
    }
    else {
        for (int i=0; i<len; i++) {
            printf("Fini 1 tour\n");
            search(etats[i],-color,weights1,weights2);
            
        }
        free_etats(etats,len);
    }
    return;
}

void training(board gb, int color) { //Lance l'entrainement en lançant la recherche sur l'arbre de jeu
    FILE* f = fopen("neural_network_9_5","r");
    assert(f!=NULL);
    if(fgetc(f)==EOF) {
        double** weights1 = malloc(45*sizeof(double*));
        double** weights2 = malloc(7*sizeof(double*));
        for (int i=0; i<45; i++) {
            weights1[i] = malloc(7*sizeof(double));
        }
        for (int i=0; i<7; i++) {
            weights2[i] = malloc(sizeof(double));
        }
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                weights1[i][j] = (double)rand()/(double)RAND_MAX;
            }
        }
        for (int i=0; i<7; i++) {
            weights2[i][0] = (double)rand()/(double)RAND_MAX;
        }
        fclose(f);
        f = fopen("neural_network_9_5","w");
        for (int i=0; i<45; i++) {
            for (int j=0; j<7; j++) {
                fprintf(f,"%#.32g ",weights1[i][j]);
            }
            fprintf(f,"\n");
        }
        for (int i=0; i<7; i++) {
            fprintf(f,"%#.32g ",weights2[i][0]);
        }
        fclose(f);
    }
    double** weights1 = malloc(45*sizeof(double*));
    double** weights2 = malloc(7*sizeof(double*));
    for (int i=0; i<45; i++) {
        weights1[i] = malloc(7*sizeof(double));
    }
    for (int i=0; i<7; i++) {
        weights2[i] = malloc(sizeof(double));
    }
    read_weights(weights1,weights2);
    search(gb,color,weights1,weights2);
}

void* ThreadedAlphaBeta(void* args) {//Version Threadée de l'alphabeta
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
    int len=0;
    int ind=0;
    // assert(!is_end);
    if (is_end) {
        double* tmp = malloc(sizeof(double));
        *tmp = neural_network(cb,color);
        free_gameboard(cb);
        return tmp;
        }
    free_gameboard(cb);
    board* etats = flatten_possibilites(gb,color, &len, &ind, false);
    if (len==0) printf("LEN=0 IN THREAD\n");
    assert(len>0);
    if (color==icolor) { //max node
        if (iol>=len) { //max node quand la liste est vide
            double* alp = malloc(sizeof(double));
            *alp = alpha;
            free_etats(etats,len);
            return alp;
        } else { //max node quand la liste n'est pas vide
            if (depth==0) {
                // Alloue la mémoire pour les IDs des threads, les arguments, et les resultats
                pthread_t* threads = malloc(sizeof(pthread_t) * len);
                ThreadedAlphaBetaArgs* args = malloc(sizeof(ThreadedAlphaBetaArgs) * len);
                double* results = malloc(sizeof(double) * len);

                // Crée les threads
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

                // Attends que les autres aient fini
                for (int i = 0; i < len; i++) {
                    double* result = malloc(sizeof(double));
                    pthread_join(threads[i], (void**)&result);
                    results[i] = *result;
                    free(result);
                }

                // Trouve le max et l'index du max
                int number_of_max = 0;
                double max_value = -1;
                int max_index = -1;
                for (int i = 0; i < len; i++) {
                    if (results[i] > max_value) {
                        number_of_max = 1;
                        max_value = results[i];
                        max_index = i;
                    } else if (results[i] == max_value) {
                        number_of_max++;
                    }
                }
                int r = rand() % number_of_max;

                for (int i = 0; i < len; i++) {
                    if (results[i] == max_value) {
                        if (r == 0) {
                            max_index = i;
                            break;
                        } else {
                            r--;
                        }
                    }
                }

                // Stock l'index du meilleur coup dans iom
                *iom = max_index;

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
        if (iol>=len) { //min node quand la liste est vide
            double* bet = malloc(sizeof(double));
            *bet = beta;
            free_etats(etats,len);
            return bet;
        } else { //min node quand la liste n'est pas vide
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
    assert(false); //Ne devrait jamais arriver ici
    return NULL;
}

void ia(board gameboard, int color, bool endgame) {
    srand(time(NULL));
    int len=0;
    int ind=0;
    board* etats = flatten_possibilites(gameboard,color, &len, &ind, endgame); //IN: gameboard avec case carrées
    if (endgame_occured_once) {
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