/*
    if (dir==0) {
        gameboard.plateau[x-1][y-1]=color;
        gameboard.plateau[x][y]=0;
        x--;
        y--;
        for (int i=1; i<gameboard.longueur; i++) {
            if (x-i>=0 && y-i>=0 && gameboard.plateau[x-i][y-i]==-color) gameboard.plateau[x-i][y-i]=0;
            else break;
        }
    }
    if (dir==1) {
        gameboard.plateau[x][y-1]=color;
        gameboard.plateau[x][y]=0;
        y--;
        for (int i=1; i<gameboard.longueur; i++) {
            if (y-i>=0 && gameboard.plateau[x][y-i]==-color) gameboard.plateau[x][y-i]=0;
            else break;
        }
    }
    if (dir==2) {
        gameboard.plateau[x+1][y-1]=color;
        gameboard.plateau[x][y]=0;
        x++;
        y--;
        for (int i=1; i<gameboard.longueur; i++) {
            if (x+i<gameboard.longueur && y-i>=0 && gameboard.plateau[x+i][y-i]==-color) gameboard.plateau[x+i][y-i]=0;
            else break;
        }
    }
    if (dir==3) {
        gameboard.plateau[x+1][y]=color;
        gameboard.plateau[x][y]=0;
        x++;
        for (int i=1; i<gameboard.longueur; i++) {
            if (x+i<gameboard.longueur && gameboard.plateau[x+i][y]==-color) gameboard.plateau[x+i][y]=0;
            else break;
        }
    }
    if (dir==4) {
        gameboard.plateau[x+1][y+1]=color;
        gameboard.plateau[x][y]=0;
        x++;
        y++;
        for (int i=1; i<gameboard.longueur; i++) {
            if (x+i<gameboard.longueur && y+i<gameboard.hauteur && gameboard.plateau[x+i][y+i]==-color) gameboard.plateau[x+i][y+i]=0;
            else break;
        }
    }
    if (dir==5) {
        gameboard.plateau[x][y+1]=color;
        gameboard.plateau[x][y]=0;
        y++;
        for (int i=1; i<gameboard.longueur; i++) {
            if (y+i<gameboard.hauteur && gameboard.plateau[x][y+i]==-color) gameboard.plateau[x][y+i]=0;
            else break;
        }
    }
    if (dir==6) {
        gameboard.plateau[x-1][y+1]=color;
        gameboard.plateau[x][y]=0;
        x--;
        y++;
        for (int i=1; i<gameboard.longueur; i++) {
            if (x-i>=0 && y+i<gameboard.hauteur && gameboard.plateau[x-i][y+i]==-color) gameboard.plateau[x-i][y+i]=0;
            else break;
        }
    }
    if (dir==7) {
        gameboard.plateau[x-1][y]=color;
        gameboard.plateau[x][y]=0;
        x--;
        for (int i=1; i<gameboard.longueur; i++) {
            if (x-i>=0 && gameboard.plateau[x-i][y]==-color) gameboard.plateau[x-i][y]=0;
            else break;
        }
    } 
    
node proofnumbersearch(board gb, int color, int or0and1, int depth, int icolor) {
    board cb = copy_gameboard(gb);
    clean_board(cb);
    bool is_end = is_endgame(cb,color);
    free_gameboard(cb);
    if (is_end || depth>=6) {
        double proofnumber, disproofnumber;
        if (icolor==-1) {
            proofnumber = (double)(*gb.nb_pion_noir)/(*gb.nb_pion_blanc);
            disproofnumber = (double)(*gb.nb_pion_blanc)/(*gb.nb_pion_noir);
            // printf("Pn = %lf, Dn = %lf, icolor = %d\n",proofnumber,disproofnumber, icolor);
            // print_game(gb);
            // printf("\n");
        } else {
            proofnumber = (double)(*gb.nb_pion_blanc)/(*gb.nb_pion_noir);
            disproofnumber = (double)(*gb.nb_pion_noir)/(*gb.nb_pion_blanc);
            // printf("Pn = %lf, Dn = %lf,n icolor = %d\n",proofnumber,disproofnumber, icolor);
            // print_game(gb);
            // printf("\n");
        }
        node res = {0,proofnumber,disproofnumber};
        return res;
    } else {
        int len=0;
        int ind=0;
        board* etats = flatten_possibilites(gb,color, &len, &ind, false);
        for (int i=0; i<len; i++) {
            put_square(etats[i],-color);
        }
        if (or0and1==0) {
            assert(len>0);
            printf(" 0");
            node res = proofnumbersearch(etats[0], -color, 1, depth+1, icolor);
            printf("\b \b");
            double disproofnb = res.disproofnumber;
            double proofnb = res.proofnumber;
            int index = 0;
            for (int i=1; i<len; i++) {
                printf("%d",i);
                node res1 = proofnumbersearch(etats[i], -color, 1, depth+1, icolor);
                printf("\b \b");
                if (i>=10) {
                    printf("\b \b");
                }
                if (i>=100) {
                    printf("\b \b");
                }
                disproofnb = disproofnb+res1.disproofnumber;
                if (res1.proofnumber<proofnb) {
                    proofnb = res1.proofnumber;
                    index=i;
                }
            }
            printf("\b");
            node rs = {index,proofnb,disproofnb};
            return rs;
        } else {
            assert(len>0);
            printf(" 0");
            node res = proofnumbersearch(etats[0], -color, 0, depth+1, icolor);
            printf("\b \b");
            double proofnb = res.proofnumber;
            double disproofnb = res.disproofnumber;
            int index = 0;
            for (int i=1; i<len; i++) {
                printf("%d",i);
                node res1 = proofnumbersearch(etats[i], -color, 0, depth+1, icolor);
                printf("\b \b");
                if (i>=10) {
                    printf("\b \b");
                }
                if (i>=100) {
                    printf("\b \b");
                }
                proofnb = proofnb+res1.proofnumber;
                if (res1.disproofnumber>res.disproofnumber) {
                    disproofnb = res1.disproofnumber;
                    index = i;
                }
                free_gameboard(etats[i]);
            }
            free(etats);
            printf("\b");
            node rs = {index,proofnb,disproofnb};
            return rs;
        }
    
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
            return alp;
        } else { //max node when the list is not empty
            if (depth==0) {
                // Allocate memory for thread IDs, arguments, and results
                pthread_t* threads = malloc(sizeof(pthread_t) * len);
                ThreadedAlphaBetaArgs* args = malloc(sizeof(int) * len);
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
                    printf("%d / %d\n",i,len-1);
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
                //free_etats(etats,len);

                double* max_value_ptr = malloc(sizeof(double));
                *max_value_ptr = max_value;
                return max_value_ptr;
            }
        }
    } else { //min node
        if (iol>=len) { //min node when the list is empty
            double* bet = malloc(sizeof(double));
            *bet = beta;
            return bet;
        } else { //min node when the list is not empty
            put_square(etats[iol],-color);
            double valeur = alphabeta(etats[iol],-color,depth+1,alpha,beta,icolor,0,0);
            double beta1 = (beta<valeur)?beta:valeur;
            if (alpha>=beta1) {
                //free_etats(etats,len);
                double* beta2 = malloc(sizeof(double));
                *beta2 = beta1;
                return beta2;
            } else {
                //free_etats(etats,len);
                double* ret = malloc(sizeof(double));
                *ret = alphabeta(gb,color,depth,alpha,beta1,icolor,iol+1,0);
                return ret;
            }
        }
    
    }
    free_etats(etats,len);
    assert(false); //Should never reach this point
    return NULL;
}

// ThreadedAlphaBetaArgs* args = malloc(sizeof(ThreadedAlphaBetaArgs));
        // args->gb = gameboard;
        // args->color = color;
        // args->depth = 0;
        // args->alpha = -1;
        // args->beta = 9*5;
        // args->icolor = color;
        // args->iol = 0;
        // args->iom = &indexofmax;
        // void* res = ThreadedAlphaBeta(args);
        // double res1 = *((double*)res);
//node res = proofnumbersearch(gameboard, color, 0, 0, color);
        //printf("Pn = %f, Dn = %f\n",res.proofnumber,res.disproofnumber);


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