#ifndef BOARD_H
#define BOARD_H

typedef struct {
    int** plateau;
    int longueur;
    int hauteur;
    int* nb_pion_blanc;
    int* nb_pion_noir;
} board;

typedef struct {
    int index;
    double proofnumber;
    double disproofnumber;
} node;

typedef struct {
    board gb;
    int color;
    int depth;
    double alpha;
    double beta;
    int icolor;
    int iol;
    int* iom;

} ThreadedAlphaBetaArgs;

#endif