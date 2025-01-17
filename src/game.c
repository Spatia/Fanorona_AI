#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "ia.h"
#include "type.h"
#include "game.h"

bool endgame_occured_once = false;

typedef struct {
    int x;
    int y;
} coordinate;

void print_bool(bool* table, int len) {
    for (int i=0; i<len; i++) {
        printf("%d ", table[i]);
    }
    printf("\n");
}

void free_gameboard(board gameboard) {
    for (int x=0; x<gameboard.longueur; x++){
        free(gameboard.plateau[x]);
    }
    free(gameboard.plateau);
}

void print_game(board gameboard) {
    //system("clear\n");
    if(gameboard.longueur>10) {
        printf("  ");
        for (int i=0; i<10; i++) printf("  ");
        for (int i=10; i<gameboard.longueur; i++) printf("%d ",i/10);
        printf("\n");
    }
    printf("  ");
    for (int i=0; i<gameboard.longueur; i++) printf("%d ",i%10);
    printf("\n");
    for (int y=0; y<gameboard.hauteur; y++) {
        printf("%s%d",y>=10?"":" ",y);
        for (int x=0; x<gameboard.longueur; x++) {
            printf("%s", gameboard.plateau[x][y]==1?"○ ":
            (gameboard.plateau[x][y]==-1?"● ":
            (gameboard.plateau[x][y]==2?"↖ ":
            (gameboard.plateau[x][y]==3?"↑ ":
            (gameboard.plateau[x][y]==4?"↗ ":
            (gameboard.plateau[x][y]==5?"→ ":
            (gameboard.plateau[x][y]==6?"↘ ":
            (gameboard.plateau[x][y]==7?"↓ ":
            (gameboard.plateau[x][y]==8?"↙ ":
            (gameboard.plateau[x][y]==9?"← ":
            (gameboard.plateau[x][y]==10?"■ ":
            (gameboard.plateau[x][y]==11?"□ ": "  "
            ))))))))))));
        }
        printf("\n");
    }
}

void ajouter_fleche_plateau(board gameboard, int x, int y, bool* coups) {
    if (coups[0]==true) gameboard.plateau[x-1][y-1]=2;
    if (coups[1]==true) gameboard.plateau[x][y-1]=3;
    if (coups[2]==true) gameboard.plateau[x+1][y-1]=4;
    if (coups[3]==true) gameboard.plateau[x+1][y]=5;
    if (coups[4]==true) gameboard.plateau[x+1][y+1]=6;
    if (coups[5]==true) gameboard.plateau[x][y+1]=7;
    if (coups[6]==true) gameboard.plateau[x-1][y+1]=8;
    if (coups[7]==true) gameboard.plateau[x-1][y]=9;
    if (gameboard.plateau[x][y]==-1) gameboard.plateau[x][y]=10;
    if (gameboard.plateau[x][y]==1) gameboard.plateau[x][y]=11;
}

/*
● :-1
  : 0
○ : 1
↖ : 2 (0)
↑ : 3 (1)
↗ : 4 (2)
→ : 5 (3)
↘ : 6 (4)
↓ : 7 (5)
↙ : 8 (6)
← : 9 (7)
■ : 10
□ : 11
*/

int** creer_tab2D(board gameboard) {
    int** res = malloc(sizeof(int*)*gameboard.longueur);
    assert(res!=NULL);
    for(int x=0; x<gameboard.longueur; x++) {
        res[x] = calloc(sizeof(int)*gameboard.hauteur, sizeof(int));
        assert(res[x]!=NULL);
    }
    return res;
}

void free_tab2D(int** tab, board gameboard) {
    for (int x=0; x<gameboard.longueur; x++) {
        free(tab[x]);
    }
    free(tab);
}

int** copy_tab2D(int** tab, board gameboard) {
    int** res = malloc(sizeof(int*)*gameboard.longueur);
    assert(res!=NULL);
    for(int x=0; x<gameboard.longueur; x++) {
        res[x] = malloc(sizeof(int)*gameboard.hauteur);
        assert(res[x]!=NULL);
        for(int y=0; y<gameboard.hauteur; y++) {
            res[x][y]=tab[x][y];
        }
    }
    return res;
}

int** copier_plateau(board gameboard) {
    int** res = malloc(sizeof(int*)*gameboard.longueur);
    assert(res!=NULL);
    for(int x=0; x<gameboard.longueur; x++) {
        res[x] = malloc(sizeof(int)*gameboard.hauteur);
        assert(res[x]!=NULL);
        for(int y=0; y<gameboard.hauteur; y++) {
            res[x][y]=gameboard.plateau[x][y];
        }
    }
    return res;
}

board copy_gameboard(board gameboard) {
    board res;
    res.plateau=copier_plateau(gameboard);
    res.longueur=gameboard.longueur;
    res.hauteur=gameboard.hauteur;
    res.nb_pion_blanc=malloc(sizeof(int));
    assert(res.nb_pion_blanc!=NULL);
    *res.nb_pion_blanc=*gameboard.nb_pion_blanc;
    res.nb_pion_noir=malloc(sizeof(int));
    assert(res.nb_pion_noir!=NULL);
    *res.nb_pion_noir=*gameboard.nb_pion_noir;
    return res;
}

board creer_structure(int longueur,int hauteur) {
    int** plateau=malloc(sizeof(int*)*longueur);
    for (int x=0; x<longueur; x++) {
        plateau[x]=calloc(sizeof(int)*hauteur, sizeof(int));
        for (int y=0; y<hauteur; y++) {
            plateau[x][y]=(y<hauteur/2)?1:((y==hauteur/2)?((x<longueur/2)?(x%2==0?1:(-1)):((x==longueur/2)?0:(x%2==0?-1:1))):(-1));
        }
    }
    board res;
    res.plateau=plateau;
    res.longueur=longueur;
    res.hauteur=hauteur;
    res.nb_pion_blanc=malloc(sizeof(int));
    *res.nb_pion_blanc=longueur*hauteur/2;
    res.nb_pion_noir=malloc(sizeof(int));
    *res.nb_pion_noir=longueur*hauteur/2;
    return res;
}

void init_taille(int* longueur, int* largeur) {
    printf("Entrez une longueur et une hauteur impaires séparé d'un espace : ");
    scanf("%d %d",longueur,largeur);
    while((*longueur)%2 == 0 || (*largeur)%2 == 0) {
        printf("Entrez une longueur et une hauteur IMPAIRES séparé d'un espace : ");
        scanf("%d %d",longueur,largeur);
    }
}

int existence_gagnant(board gameboard) { //-1 si les blancs ont gagnés, 1 si les noirs ont gagnés, 0 sinon
    return *(gameboard.nb_pion_blanc)==0?1:(*(gameboard.nb_pion_noir)==0?-1:0);
}

void dir_to_dx_dy(int dir, int* dx, int* dy) {
    switch (dir) {
        case 0: *dx = -1; *dy = -1; break;
        case 1: *dx = 0; *dy = -1; break;
        case 2: *dx = 1; *dy = -1; break;
        case 3: *dx = 1; *dy = 0; break;
        case 4: *dx = 1; *dy = 1; break;
        case 5: *dx = 0; *dy = 1; break;
        case 6: *dx = -1; *dy = 1; break;
        case 7: *dx = -1; *dy = 0; break;
    }
}



/*
0 1 2
7   3
6 5 4
*/

//Vérifie si le joueur peut jouer, sous contrainte de l'aspiration ou de la percution.
bool* jouable(board gameboard, int x, int y, bool* possible, int color) {
    bool* coups_potentiels = calloc(sizeof(bool)*8,sizeof(bool));
    assert(coups_potentiels!=NULL);
    *possible=false;
    if (gameboard.plateau[x][y]==-color || gameboard.plateau[x][y]==0) return coups_potentiels;
    if (y-1>=0 && x-1>=0 && (x+y)%2==0 && gameboard.plateau[x-1][y-1]==0 && ( (y+1<gameboard.hauteur && x+1<gameboard.longueur && gameboard.plateau[x+1][y+1] == -gameboard.plateau[x][y]) || (x-2>=0 && y-2>=0 && gameboard.plateau[x-2][y-2]==-gameboard.plateau[x][y]))) {coups_potentiels[0]=true;*possible=true;}
    if (y-1>=0 && gameboard.plateau[x][y-1]==0 && ((y+1<gameboard.hauteur && gameboard.plateau[x][y+1]==-gameboard.plateau[x][y])||(y-2>=0 && gameboard.plateau[x][y-2]==-gameboard.plateau[x][y]))) {coups_potentiels[1]=true;*possible=true;}
    if (y-1>=0 && x+1<gameboard.longueur && (x+y)%2==0 && gameboard.plateau[x+1][y-1]==0 && ( (y+1<gameboard.hauteur && x-1>=0 && gameboard.plateau[x-1][y+1] == -gameboard.plateau[x][y]) || (x+2<gameboard.longueur && y-2>=0 &&gameboard.plateau[x+2][y-2]==-gameboard.plateau[x][y]))) {coups_potentiels[2]=true;*possible=true;}
    if (x+1<gameboard.longueur && gameboard.plateau[x+1][y]==0 && ((x-1>=0 && gameboard.plateau[x-1][y]==-gameboard.plateau[x][y]) || (x+2<gameboard.longueur && gameboard.plateau[x+2][y]==-gameboard.plateau[x][y]))) {coups_potentiels[3]=true;*possible=true;}
    if (y+1<gameboard.hauteur && x+1<gameboard.longueur && (x+y)%2==0 && gameboard.plateau[x+1][y+1]==0 && ( (y-1>=0 && x-1>=0 && gameboard.plateau[x-1][y-1] == -gameboard.plateau[x][y]) || (x+2<gameboard.longueur && y+2<gameboard.hauteur && gameboard.plateau[x+2][y+2]==-gameboard.plateau[x][y]))) {coups_potentiels[4]=true;*possible=true;}
    if (y+1<gameboard.hauteur && gameboard.plateau[x][y+1]==0 && ((y-1>=0 && gameboard.plateau[x][y-1]==-gameboard.plateau[x][y]) || (y+2<gameboard.hauteur && gameboard.plateau[x][y+2]==-gameboard.plateau[x][y]))) {coups_potentiels[5]=true;*possible=true;}
    if (x-1>=0 && y+1<gameboard.hauteur && (x+y)%2==0 && gameboard.plateau[x-1][y+1]==0 && ( (y-1>=0 && x+1<gameboard.longueur && gameboard.plateau[x+1][y-1] == -gameboard.plateau[x][y]) || (x-2>=0 && y+2<gameboard.hauteur && gameboard.plateau[x-2][y+2]==-gameboard.plateau[x][y]))) {coups_potentiels[6]=true;*possible=true;}
    if (x-1>=0 && gameboard.plateau[x-1][y]==0 && ((x+1<gameboard.longueur && gameboard.plateau[x+1][y]==-gameboard.plateau[x][y]) || (x-2>=0 && gameboard.plateau[x-2][y]==-gameboard.plateau[x][y]))) {coups_potentiels[7]=true;*possible=true;}
    return coups_potentiels;
}

coordinate* liste_jouable(board gameboard, int color) {
    int count=0;
    for (int x=0; x<gameboard.longueur; x++) {
        for (int y=0; y<gameboard.hauteur; y++) {
            if (gameboard.plateau[x][y]==10+((1+color)/2)) count++;
        }
    }
    coordinate* res = malloc(sizeof(coordinate)*count);
    assert(res!=NULL);
    for (int x=0; x<gameboard.longueur; x++) {
        for (int y=0; y<gameboard.hauteur; y++) {
            if (gameboard.plateau[x][y]==10+((1+color)/2)){
                res[count-1].x=x;
                res[count-1].y=y;
                count--;
            } 
        }
    }
    return res;
}

//Phase du jeu lorsque qu'un joueur ne peut plus bouger de pièce avec la condition "doit percuter ou aspirer"
bool* jouable_endgame(board gameboard, int x, int y, bool* possible, int color) {
    bool* coups_potentiels = calloc(sizeof(bool)*8,sizeof(bool));
    assert(coups_potentiels!=NULL);
    *possible=false;
    if (gameboard.plateau[x][y]==-color || gameboard.plateau[x][y]==0) return coups_potentiels;
    if (y-1>=0 && x-1>=0 && (x+y)%2==0 && gameboard.plateau[x-1][y-1]==0) {coups_potentiels[0]=true;*possible=true;}
    if (y-1>=0 && gameboard.plateau[x][y-1]==0) {coups_potentiels[1]=true;*possible=true;}
    if (y-1>=0 && x+1<gameboard.longueur && (x+y)%2==0 && gameboard.plateau[x+1][y-1]==0) {coups_potentiels[2]=true;*possible=true;}
    if (x+1<gameboard.longueur && gameboard.plateau[x+1][y]==0) {coups_potentiels[3]=true;*possible=true;}
    if (y+1<gameboard.hauteur && x+1<gameboard.longueur && (x+y)%2==0 && gameboard.plateau[x+1][y+1]==0) {coups_potentiels[4]=true;*possible=true;}
    if (y+1<gameboard.hauteur && gameboard.plateau[x][y+1]==0) {coups_potentiels[5]=true;*possible=true;}
    if (x-1>=0 && y+1<gameboard.hauteur && (x+y)%2==0 && gameboard.plateau[x-1][y+1]==0) {coups_potentiels[6]=true;*possible=true;}
    if (x-1>=0 && gameboard.plateau[x-1][y]==0) {coups_potentiels[7]=true;*possible=true;}
    return coups_potentiels;
}

//Vérifie si le joueur peut jouer, sous contrainte qu'il ne soit pas passé sur la case et qu'il ne se déplace pas dans la meme direction qu'au coup précédent
bool* jouable_under_condition(board gameboard, int x, int y, bool* possible, int color, int** already_passed_on) {
    bool* res=jouable(gameboard,x,y,possible,color);
    for (int i=0; i<8; i++) {
        int dx,dy;
        dir_to_dx_dy(i,&dx,&dy);
        if (res[i]==true && (
            already_passed_on[x+dx][y+dy]>0 ||
            already_passed_on[x][y]==already_passed_on[x+dx][y+dy]+1 ||
            (
                x-dx>=0 &&
                x-dx<gameboard.longueur &&
                y-dy>=0 && y-dy<gameboard.hauteur &&
                already_passed_on[x][y]==already_passed_on[x-dx][y-dy]+1
            ) 
        )) res[i]=false;
    }
    return res;
}

//Nettoie le plateau en supprimant les flèches et les cases carrées
void clean_board(board gameboard) {
    for (int x=0; x<gameboard.longueur; x++) {
        for(int y=0; y<gameboard.hauteur; y++) {
            if (gameboard.plateau[x][y]==10) gameboard.plateau[x][y]=-1;
            if (gameboard.plateau[x][y]==11) gameboard.plateau[x][y]=1;
            if (gameboard.plateau[x][y]>1) gameboard.plateau[x][y]=0;
        }
    }
}

//Vérifie si la pièce séléctionnée peut capturer des pièces adverses
bool canCapture(board gameboard, int x, int y, int** already_passed_on) {
    bool possible;
    bool* coups = jouable_under_condition(gameboard, x, y, &possible, gameboard.plateau[x][y],already_passed_on);
    for (int i=0; i<8; i++) {
        if (coups[i]==true) return true;
    }
    free(coups);
    return false;
}

//Déplace la pièce
void movePiece(board gameboard, int* x, int* y, int dx, int dy, int color) {
    gameboard.plateau[*x + dx][*y + dy] = color;
    gameboard.plateau[*x][*y] = 0;
    *x += dx;
    *y += dy;
}


//Percute les pièces adverses
void percuter(board gameboard, int x, int y, int dx, int dy, int color) {
    for (int i = 1; i < gameboard.longueur; i++) {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx >= 0 && nx < gameboard.longueur && ny >= 0 && ny < gameboard.hauteur && gameboard.plateau[nx][ny] == -color) {
            if (gameboard.plateau[nx][ny]==1) (*gameboard.nb_pion_noir)--;
            if (gameboard.plateau[nx][ny]==-1) (*gameboard.nb_pion_blanc)--;
            gameboard.plateau[nx][ny] = 0;
        } else {
            break;
        }
    }
} 

//Aspire les pièces adverses
void aspirer(board gameboard, int x, int y, int dx, int dy, int color) {
    for (int i = 2; i < gameboard.longueur; i++) {
        int nx = x - i * dx;
        int ny = y - i * dy;
        if (nx >= 0 && nx < gameboard.longueur && ny >= 0 && ny < gameboard.hauteur && gameboard.plateau[nx][ny] == -color) {
            if (gameboard.plateau[nx][ny]==1) (*gameboard.nb_pion_noir)--;
            if (gameboard.plateau[nx][ny]==-1) (*gameboard.nb_pion_blanc)--;
            gameboard.plateau[nx][ny] = 0;
        } else {
            break;
        }
    }
}

//Capture les pièces adverses
void capturePieces(board gameboard, int x, int y, int dx, int dy, int color) {
    if (x+dx>=0 && x+dx<gameboard.longueur && y+dy>=0 && y+dy<gameboard.hauteur && gameboard.plateau[x+dx][y+dy] == -color && x-(2*dx)>=0 && x-(2*dx)<gameboard.longueur && y-(2*dy)>=0 && y-(2*dy)<gameboard.hauteur && gameboard.plateau[x-2*dx][y-2*dy]==-color) {//J'ai donc deux choix 
        printf("Vous avez deux choix, veuillez choisir un nombre entre 0: Percuter et 1: Aspirer\n");
        int choix;
        scanf("%d", &choix);
        while (choix < 0 || choix > 1) {
            printf("Choix invalide, réessayez parmi les choix possibles.\n");
            printf("Vous avez deux choix, veuillez choisir un nombre entre 0 et 1\n");
            scanf("%d", &choix);
        }
        if (choix == 0) {
            percuter(gameboard, x, y, dx, dy, color);
        } else {
            aspirer(gameboard, x, y, dx, dy, color);
        }
    }
    else if (x+dx>=0 && x+dx<gameboard.longueur && y+dy>=0 && y+dy<gameboard.hauteur && gameboard.plateau[x+dx][y+dy] == -color) { //Si je pousse vers une pièce adverse, je la percute
        percuter(gameboard, x, y, dx, dy, color);
    } else if (x-(2*dx)>=0 && x-(2*dx)<gameboard.longueur && y-(2*dy)>=0 && y-(2*dy)<gameboard.hauteur && gameboard.plateau[x-(2*dx)][y-(2*dy)]==-color) { //Sinon, je pousse vers une case vide, donc j'aspire les pièces adverses
        aspirer(gameboard, x, y, dx, dy, color);
    }
    else {
        printf("ERREUR: Capture de pièces impossible\n");
    }
}

//Fonction principale régissant le déplacement des pièces et les captures
void makeMove(board gameboard, int* x, int* y, int color) {
    int dir;
    int** already_passed_on = creer_tab2D(gameboard);
    int cpt=2;
    while (canCapture(gameboard, *x, *y, already_passed_on)) {
        already_passed_on[*x][*y] = cpt++;
        bool tmp;
        bool* coups = jouable_under_condition(gameboard,*x,*y,&tmp,color,already_passed_on);
        ajouter_fleche_plateau(gameboard,*x,*y,coups);
        print_game(gameboard);
        printf("Entrez un nombre entre %d et 7 pour choisir un mouvement\n", cpt>=4?(-1):0);
        printf("0 : ↖\t 1 : ↑\t 2 : ↗\t 3 : →\t 4 : ↘\t 5 : ↓\t 6 : ↙\t 7 : ←%s\n", cpt>=4?"\t -1: Finir le tour":"");
        scanf("%d", &dir);
        while (dir < -1 || dir > 7 || (dir != -1 && coups[dir] == false) || (dir==-1 && cpt<4)) {
            printf("Mouvement invalide, réessayez parmi les mouvements possibles.\n");
            printf("Entrez un nombre entre %d et 7 pour choisir un mouvement\n", cpt>=4?(-1):0);
            scanf("%d", &dir);
        }
        clean_board(gameboard);
        if (dir == -1) break;
        int dx = 0, dy = 0;
        dir_to_dx_dy(dir, &dx, &dy);
        movePiece(gameboard, x, y, dx, dy, color);
        capturePieces(gameboard, *x, *y, dx, dy, color);
    } 
    free_tab2D(already_passed_on, gameboard);
}

//Alternative à makeMove pour la phase de fin de jeu
void makeMove_endgame(board gameboard, int* x, int * y, int color) {
    int dir;
    bool tmp;
    bool* coups = jouable_endgame(gameboard,*x,*y,&tmp,color);
    ajouter_fleche_plateau(gameboard,*x,*y,coups);
    print_game(gameboard);
    printf("Entrez un nombre entre 0 et 7 pour choisir un mouvement\n");
    printf("0 : ↖\t 1 : ↑\t 2 : ↗\t 3 : →\t 4 : ↘\t 5 : ↓\t 6 : ↙\t 7 : ←\n");
    scanf("%d", &dir);
    while (dir < 0 || dir > 7 || coups[dir] == false) {
        printf("Mouvement invalide, réessayez parmi les mouvements possibles.\n");
        printf("Entrez un nombre entre 0 et 7 pour choisir un mouvement\n");
        scanf("%d", &dir);
    }
    clean_board(gameboard);
    int dx = 0, dy = 0;
    dir_to_dx_dy(dir, &dx, &dy);
    movePiece(gameboard, x, y, dx, dy, color);
}

int square_colored(int color) {
    return 10+((1+color)/2);
}

bool is_endgame(board gameboard, int color) {
    bool endgame = true;
    bool tmp = false;
    for (int x=0; x<gameboard.longueur; x++) {
        for (int y=0; y<gameboard.hauteur; y++) {
            bool* jeux=jouable(gameboard,x,y,&tmp,color);
            free(jeux);
            if (tmp) {
                endgame=false;
            }
        }
    }
    return endgame;
}

void put_square(board gameboard, int color) {
    bool is_end = is_endgame(gameboard,color);
    bool tmp = false;
    for (int x=0; x<gameboard.longueur; x++) {
        for (int y=0; y<gameboard.hauteur; y++) {
            if (is_end) {
                bool* jeux=jouable_endgame(gameboard,x,y,&tmp,color);
            } else {
                bool* jeux=jouable(gameboard,x,y,&tmp,color);
            }
            if (tmp) {
                gameboard.plateau[x][y]=square_colored(color);
            }
        }
    }
}

//Vérifie d'abord si l'on est dans une phase de fin de jeu, puis invite le joueur à sélectionner une pièce (sans choix de direction pour le moment, car c'est makeMove qui s'en charge)
void joue(board gameboard, int tour, int start_color) {
    int color=tour%2==0?start_color:-start_color;
    int x,y;
    /*On regardes tous les coups possibles*/
    bool endgame = true;
    bool tmp = false;
    for (int x=0; x<gameboard.longueur; x++) {
        for (int y=0; y<gameboard.hauteur; y++) {
            bool* jeux=jouable(gameboard,x,y,&tmp,color);
            free(jeux);
            if (tmp) {
                gameboard.plateau[x][y]=square_colored(color);
                endgame=false;
            }
        }
    }
    if (endgame) {
        endgame_occured_once = true;
        for (int x=0; x<gameboard.longueur; x++) {
            for (int y=0; y<gameboard.hauteur; y++) {
                bool* jeux=jouable_endgame(gameboard,x,y,&tmp,color);
                free(jeux);
                if (tmp) {
                    gameboard.plateau[x][y]=square_colored(color);
                }
            }
        }
    }
    print_game(gameboard);
    if (color==-1) {
        printf("Au tour de %s, entrez x puis y séparé d'un espace : \n",color==-1?"■":"□");
        scanf("%d %d",&x,&y);
        while (x>=gameboard.longueur || y>=gameboard.hauteur || y<0 || x<0 || gameboard.plateau[x][y]!=square_colored(color)) {
            printf("Coordonnée invalide, réessayez parmi les cases carrées.\n");
            printf("Au tour de %s, entrez x puis y séparé d'un espace : ",color==-1?"●":"○");
            scanf("%d %d",&x,&y);
            }
        clean_board(gameboard);
        printf("Pièce %d %d sélectionnée avec succès.\n",x,y);
        if (endgame) {
            makeMove_endgame(gameboard,&x,&y,color);
        } else {
            makeMove(gameboard, &x, &y, color);
        }
    } else {
        ia(gameboard,color,endgame);
    }
    
}

//Boucle principale du jeu, post-condition: renvoie le gagnant
int jeu(board gameboard, int (*gagnant)(board), int* tour, int start_color) {
    while((*gagnant)(gameboard)==0&&(*tour)<1000) {
        joue(gameboard,*tour,start_color);
        (*tour)++;
    }
    if (*tour>=100) {
        return 0;
    }
    return (*gagnant)(gameboard);
}

int main() {
    int longueur,largeur=0;
    init_taille(&longueur,&largeur); // On récupère la taille via le terminal
    board gameboard=creer_structure(longueur,largeur); // On crée la structure
    // training(gameboard,-1);
    int tour=0;
    int gagnant = jeu(gameboard,&existence_gagnant,&tour,-1);
    printf("Le gagnant est %s\n",gagnant==-1?"le joueur blanc":(gagnant==1?"le joueur noir":"Égalité"));
    print_game(gameboard);
    return 0;
}

// Obsolète: ne prends pas en charge le choix entre percussion et aspiration; 9 5 4 3 1 5 1 0 5 7 4 1 3 1 4 5 3 3 2 1 2 7 2 1 5 2 5 8 2 0 6 3 3 2 1 1 2 3 4 1 0 1 5 3 2 1 4 1 3 1 4 1 8 1 7 1 3 4 7 1 1 5 4 1 5 1 5 2 4 2 5 0 5 3 3 2 7 6 1 5 3 2 3 6 2 7
// 9 5 4 3 1 3 2 3 0 5 