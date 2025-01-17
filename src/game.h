#ifndef GAME_H
#define GAME_H
#include "type.h"
#include <stdbool.h>

void movePiece(board gameboard, int* x, int* y, int dx, int dy, int color);
void print_game(board gameboard);
bool canCapture(board gameboard, int x, int y, int** already_passed_on);
bool * jouable_endgame(board gameboard, int x, int y, bool * possible, int color);
bool* jouable_under_condition(board gameboard, int x, int y, bool* possible, int color, int** already_passed_on);
bool* jouable(board gameboard, int x, int y, bool* possible, int color);
void dir_to_dx_dy(int dir, int* dx, int* dy);
void print_bool(bool *table, int len);
void free_gameboard(board gameboard);
int** copier_plateau(board gameboard);
int** creer_tab2D(board gameboard);
void free_tab2D(int** tab, board gameboard);
int** copy_tab2D(int** tab, board gameboard);
board copy_gameboard(board gameboard);
void clean_board(board gameboard);
void aspirer(board gameboard, int x, int y, int dx, int dy, int color);
void percuter(board gameboard, int x, int y, int dx, int dy, int color);
void print_game(board gameboard);
bool is_endgame(board gameboard, int color);
void put_square(board gameboard, int color);
#endif