#ifndef IA_H
#define IA_H
#include "type.h"

board* flatten_possibilites(board gameboard, int color, int* len, int* ind, bool endgame);
void ia(board gameboard, int color, bool endgame);
void training(board gameboard, int color);
double neural_network(board gameboard, int color);

#endif