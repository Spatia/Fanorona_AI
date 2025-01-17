// #include <stdlib.h>
// #include <stdio.h>
// #include <stdbool.h>

// typedef struct {
//     int** plateau;
//     int longueur;
//     int hauteur;
//     int nb_pion_blanc;
//     int nb_pion_noir;
// } board;

// typedef struct {
//     int x;
//     int y;
// } coordinate;

// void print_bool(bool* table, int len) {
//     for (int i=0; i<len; i++) {
//         printf("%d ", table[i]);
//     }
//     printf("\n");
// }

// void free_gameboard(board gameboard) {
//     for (int x=0; x<gameboard.longueur; x++){
//         free(gameboard.plateau[x]);
//     }
//     free(gameboard.plateau);
// }

// void print_game(board gameboard) {
//     if(gameboard.longueur>10) {
//         printf("  ");
//         for (int i=0; i<10; i++) printf("  ");
//         for (int i=10; i<gameboard.longueur; i++) printf("%d ",i/10);
//         printf("\n");
//     }
//     printf("  ");
//     for (int i=0; i<gameboard.longueur; i++) printf("%d ",i%10);
//     printf("\n");
//     for (int y=0; y<gameboard.hauteur; y++) {
//         printf("%s%d",y>=10?"":" ",y);
//         for (int x=0; x<gameboard.longueur; x++) {
//             printf("%s", gameboard.plateau[x][y]==1?"● ":
//             (gameboard.plateau[x][y]==-1?"○ ":
//             (gameboard.plateau[x][y]==2?"↖ ":
//             (gameboard.plateau[x][y]==3?"↑ ":
//             (gameboard.plateau[x][y]==4?"↗ ":
//             (gameboard.plateau[x][y]==5?"→ ":
//             (gameboard.plateau[x][y]==6?"↘ ":
//             (gameboard.plateau[x][y]==7?"↓ ":
//             (gameboard.plateau[x][y]==8?"↙ ":
//             (gameboard.plateau[x][y]==9?"← ":
//             (gameboard.plateau[x][y]==10?"□ ":
//             (gameboard.plateau[x][y]==11?"■ ": "  "
//             ))))))))))));
//         }
//         printf("\n");
//     }
// }

// /*
// ○ :-1
//   : 0
// ● : 1
// ↖ : 2 (0)
// ↑ : 3 (1)
// ↗ : 4 (2)
// → : 5 (3)
// ↘ : 6 (4)
// ↓ : 7 (5)
// ↙ : 8 (6)
// ← : 9 (7)
// □ : 10
// ■ : 11
// */

// int** copier_plateau(board gameboard) {
//     int** res = malloc(sizeof(int*)*gameboard.longueur);
//     for(int x=0; x<gameboard.longueur; x++) {
//         res[x] = malloc(sizeof(int)*gameboard.hauteur);
//         for(int y=0; y<gameboard.hauteur; y++) {
//             res[x][y]=gameboard.plateau[x][y];
//         }
//     }
//     return res;
// }

// board creer_structure(int longueur,int hauteur) {
//     int** plateau=malloc(sizeof(int*)*longueur);
//     for (int x=0; x<longueur; x++) {
//         plateau[x]=malloc(sizeof(int)*hauteur);
//         for (int y=0; y<hauteur; y++) {
//             plateau[x][y]=(y<hauteur/2)?1:((y==hauteur/2)?((x<longueur/2)?(x%2==0?1:(-1)):((x==longueur/2)?0:(x%2==0?-1:1))):(-1));
//         }
//     }
//     board res;
//     res.plateau=plateau;
//     res.longueur=longueur;
//     res.hauteur=hauteur;
//     res.nb_pion_blanc=longueur*hauteur/2;
//     res.nb_pion_noir=longueur*hauteur/2;
//     return res;
// }

// void init_taille(int* longueur, int* largeur) {
//     printf("Entrez une longueur et une largeur impaires séparé d'un espace : ");
//     scanf("%d %d",longueur,largeur);
//     while((*longueur)%2 == 0 || (*largeur)%2 == 0) {
//         printf("Entrez une longueur et une largeur IMPAIRES séparé d'un espace : ");
//         scanf("%d %d",longueur,largeur);
//     }
// }

// int existence_gagnant(board gameboard) { //-1 si les blancs ont gagnés, 1 si les noirs ont gagnés, 0 sinon
//     return gameboard.nb_pion_blanc==0?1:(gameboard.nb_pion_noir==0?-1:0);
// }


// /*
// 0 1 2
// 7   3
// 6 5 4
// */

// /*
// Vérifie si le joueur peu jouer, sous contrainte de l'aspiration ou de la percution.
// */
// bool* jouable(board gameboard, int x, int y, bool* possible, int color) {
//     bool* coups_potentiels = calloc(sizeof(bool)*8,sizeof(bool));
//     *possible=false;
//     if (gameboard.plateau[x][y]==-color) return coups_potentiels;
//     if (y-1>=0 && x-1>=0 && (x+y)%2==0 && gameboard.plateau[x-1][y-1]==0 && ( (y+1<gameboard.hauteur && x+1<gameboard.longueur && gameboard.plateau[x+1][y+1] == -gameboard.plateau[x][y]) || (gameboard.plateau[x-2][y-2]==-gameboard.plateau[x][y]))) {coups_potentiels[0]=true;*possible=true;}
//     if (y-1>=0 && gameboard.plateau[x][y-1]==0 && ((y+1<gameboard.hauteur && gameboard.plateau[x][y+1]==-gameboard.plateau[x][y])||(gameboard.plateau[x][y-2]==-gameboard.plateau[x][y]))) {coups_potentiels[1]=true;*possible=true;}
//     if (y-1>=0 && x+1<gameboard.longueur && (x+y)%2==0 && gameboard.plateau[x+1][y-1]==0 && ( (y+1<gameboard.hauteur && x-1<gameboard.longueur && gameboard.plateau[x-1][y+1] == -gameboard.plateau[x][y]) || (gameboard.plateau[x+2][y-2]==-gameboard.plateau[x][y]))) {coups_potentiels[2]=true;*possible=true;}
//     if (x+1<gameboard.longueur && gameboard.plateau[x+1][y]==0 && ((x-1>0 && gameboard.plateau[x-1][y]==-gameboard.plateau[x][y]) || (gameboard.plateau[x+2][y]==-gameboard.plateau[x][y]))) {coups_potentiels[3]=true;*possible=true;}
//     if (y+1<gameboard.hauteur && x+1<gameboard.longueur && (x+y)%2==0 && gameboard.plateau[x+1][y+1]==0 && ( (y-1<gameboard.hauteur && x-1<gameboard.longueur && gameboard.plateau[x-1][y-1] == -gameboard.plateau[x][y]) || (gameboard.plateau[x+2][y+2]==-gameboard.plateau[x][y]))) {coups_potentiels[4]=true;*possible=true;}
//     if (y+1<gameboard.hauteur && gameboard.plateau[x][y+1]==0 && ((y-1>0 && gameboard.plateau[x][y-1]==-gameboard.plateau[x][y]) || (gameboard.plateau[x][y+1]==-gameboard.plateau[x][y]))) {coups_potentiels[5]=true;*possible=true;}
//     if (x-1>=0 && y+1<gameboard.hauteur && (x+y)%2==0 && gameboard.plateau[x-1][y+1]==0 && ( (y-1<gameboard.hauteur && x+1<gameboard.longueur && gameboard.plateau[x+1][y-1] == -gameboard.plateau[x][y]) || (gameboard.plateau[x-2][y+2]==-gameboard.plateau[x][y]))) {coups_potentiels[6]=true;*possible=true;}
//     if (x-1>=0 && gameboard.plateau[x-1][y]==0 && ((x+1<gameboard.longueur && gameboard.plateau[x+1][y]==-gameboard.plateau[x][y]) || (gameboard.plateau[x-2][y]==-gameboard.plateau[x][y]))) {coups_potentiels[7]=true;*possible=true;}
//     return coups_potentiels;
// }

// coordinate* liste_jouable(board gameboard, int color) {
//     int count=0;
//     for (int x=0; x<gameboard.longueur; x++) {
//         for (int y=0; y<gameboard.hauteur; y++) {
//             if (gameboard.plateau[x][y]==10+((1+color)/2)) count++;
//         }
//     }
//     coordinate* res = malloc(sizeof(coordinate)*count);
//     for (int x=0; x<gameboard.longueur; x++) {
//         for (int y=0; y<gameboard.hauteur; y++) {
//             if (gameboard.plateau[x][y]==10+((1+color)/2)){
//                 res[count-1].x=x;
//                 res[count-1].y=y;
//                 count--;
//             } 
//         }
//     }
//     return res;
// }

// bool* jouable_endgame(board gameboard, int x, int y, bool* possible, int color) {
//     bool* coups_potentiels = calloc(sizeof(bool)*8,sizeof(bool));
//     *possible=false;
//     if (gameboard.plateau[x][y]==-color) return coups_potentiels;
//     if (y-1>=0 && x-1>=0 && x+y%2==0 && gameboard.plateau[x-1][y-1]==0) {coups_potentiels[0]=true;*possible=true;}
//     if (y-1>=0 && gameboard.plateau[x][y-1]==0) {coups_potentiels[1]=true;*possible=true;}
//     if (y-1>=0 && x+1<gameboard.longueur && x+y%2==0 && gameboard.plateau[x+1][y-1]==0) {coups_potentiels[2]=true;*possible=true;}
//     if (x+1<gameboard.longueur && gameboard.plateau[x+1][y]==0) {coups_potentiels[3]=true;*possible=true;}
//     if (y+1<gameboard.hauteur && x+1<gameboard.longueur && x+y%2==0 && gameboard.plateau[x+1][y+1]==0) {coups_potentiels[4]=true;*possible=true;}
//     if (y+1<gameboard.hauteur && gameboard.plateau[x][y+1]==0) {coups_potentiels[5]=true;*possible=true;}
//     if (x-1>=0 && y+1<gameboard.hauteur && x+y%2==0 && gameboard.plateau[x-1][y+1]==0) {coups_potentiels[6]=true;*possible=true;}
//     if (x-1>=0 && gameboard.plateau[x-1][y]==0) {coups_potentiels[7]=true;*possible=true;}
//     return coups_potentiels;
// }

// void clean_board(board gameboard) {
//     for (int x=0; x<gameboard.longueur; x++) {
//         for(int y=0; y<gameboard.hauteur; y++) {
//             if (gameboard.plateau[x][y]==10) gameboard.plateau[x][y]=-1;
//             if (gameboard.plateau[x][y]==11) gameboard.plateau[x][y]=1;
//             if (gameboard.plateau[x][y]>1) gameboard.plateau[x][y]=0;
//         }
//     }
// }

// void movePiece(board gameboard, int* x, int* y, int dx, int dy, int color) {
//     gameboard.plateau[*x + dx][*y + dy] = color;
//     gameboard.plateau[*x][*y] = 0;
//     *x += dx;
//     *y += dy;
// }

// void capturePieces(board gameboard, int x, int y, int dx, int dy, int color) {
//     for (int i = 1; i < gameboard.longueur; i++) {
//         int nx = x + i * dx;
//         int ny = y + i * dy;
//         if (nx >= 0 && nx < gameboard.longueur && ny >= 0 && ny < gameboard.hauteur && gameboard.plateau[nx][ny] == -color) {
//             gameboard.plateau[nx][ny] = 0;
//         } else {
//             break;
//         }
//     }
// }

// void makeMove(board gameboard, int* x, int* y, int dir, int color) {
//     int dx = 0, dy = 0;
//     switch (dir) {
//         case 0: dx = -1; dy = -1; break;
//         case 1: dy = -1; break;
//         case 2: dx = 1; dy = -1; break;
//         case 3: dx = 1; break;
//         case 4: dx = 1; dy = 1; break;
//         case 5: dy = 1; break;
//         case 6: dx = -1; dy = 1; break;
//         case 7: dx = -1; break;
//     }
//     movePiece(gameboard, x, y, dx, dy, color);
//     capturePieces(gameboard, *x, *y, dx, dy, color);
// }

// void joue(board gameboard, int tour, int start_color) {
//     int color=tour%2==0?start_color:-start_color;
//     printf("Au tour de %s, entrez x puis y séparé d'un espace : \n",color==-1?"□":"■");
//     int x,y=0;
//     /*On regardes tous les coups possibles*/
//     bool endgame = true;
//     bool tmp = false;
//     for (int x=0; x<gameboard.longueur; x++) {
//         for (int y=0; y<gameboard.hauteur; y++) {
//             bool* jeux=jouable(gameboard,x,y,&tmp,color);
//             free(jeux);
//             if (tmp) {
//                 gameboard.plateau[x][y]=10+((1+color)/2);
//                 endgame=false;
//                 }
//         }
//     }
//     print_game(gameboard);
//     scanf("%d %d",&x,&y);
//     while (x>=gameboard.longueur || y>=gameboard.hauteur || y<0 || x<0 || gameboard.plateau[x][y]!=(10+((1+color)/2))) {
//         printf("Coordonnée invalide, réessayez parmi les cases carrées.\n");
//         printf("Au tour de %s, entrez x puis y séparé d'un espace : ",color==-1?"○":"●");
//         scanf("%d %d",&x,&y);
//     }
//     clean_board(gameboard);
//     printf("Pièce %d %d sélectionnée avec succès.\n Mouvements possibles :\n",x,y);
//     bool* coups = jouable(gameboard,x,y,&tmp,color);
//     if (coups[0]==true) gameboard.plateau[x-1][y-1]=2;
//     if (coups[1]==true) gameboard.plateau[x][y-1]=3;
//     if (coups[2]==true) gameboard.plateau[x+1][y-1]=4;
//     if (coups[3]==true) gameboard.plateau[x+1][y]=5;
//     if (coups[4]==true) gameboard.plateau[x+1][y+1]=6;
//     if (coups[5]==true) gameboard.plateau[x][y+1]=7;
//     if (coups[6]==true) gameboard.plateau[x-1][y+1]=8;
//     if (coups[7]==true) gameboard.plateau[x-1][y]=9;
//     if (gameboard.plateau[x][y]==-1) gameboard.plateau[x][y]=10;
//     if (gameboard.plateau[x][y]==1) gameboard.plateau[x][y]=11;
//     print_game(gameboard);
//     printf("Entrez un nombre entre 0 et 7 pour choisir un mouvement\n");
//     printf("0 : ↖\t 1 : ↑\t 2 : ↗\t 3 : →\t 4 : ↘\t 5 : ↓\t 6 : ↙\t 7 : ←\n");
//     int dir;
//     scanf("%d",&dir);
//     while (dir<0 || dir>7 || coups[dir]==false) {
//         printf("Mouvement invalide, réessayez parmi les mouvements possibles.\n");
//         printf("Entrez un nombre entre 0 et 7 pour choisir un mouvement\n");
//         scanf("%d",&dir);
//     }
//     clean_board(gameboard);
//     makeMove(gameboard, &x, &y, dir, color);
    
//     print_game(gameboard);
//     scanf("%d", NULL);
// }

// int jeu(board gameboard, int (*gagnant)(board), int* tour, int start_color) { //la boucle principale, post-condition: renvoie le gagnant
//     while((*gagnant)(gameboard)==0) {
//         joue(gameboard,*tour,start_color);
//         (*tour)++;
//     }
//     return (*gagnant)(gameboard);
// }

// int main() {
//     int longueur,largeur=0;
//     init_taille(&longueur,&largeur); // On récupère la taille via le terminal
//     board gameboard=creer_structure(longueur,largeur); // On crée la structure
//     int tour=0;
//     jeu(gameboard,&existence_gagnant,&tour,-1);
//     return 0;
// }