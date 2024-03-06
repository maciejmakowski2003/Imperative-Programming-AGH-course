#include <stdio.h>
#include <stdlib.h>

int out_A=0;
int out_B=0;
int len_A=26;
int len_B=26;

int rand_interval(int a, int b) {
    return a + rand()%(b-a+1);
}
void rand_perm(int n, int tab[]) {
    int k;
    int tmp;
    if(n>0 && n<=100){
        for(int i=0;i<n;i++){
            tab[i]=i;
        }

        for(int i=0;i<n-1;i++){
            k=rand_interval(i,n-1);
            tmp=tab[i];
            tab[i]=tab[k];
            tab[k]=tmp;
        }
    }
}

void shuffle(int tab[], int A[], int B[], int n){
    for(int i=0;i<n;i++){
        A[i]=tab[i];
        B[i]=tab[n/2+i];
    }
}

void pushA(int T[], int card) {
    if(out_A + len_A>51){
        T[(out_A + len_A)%52]=card;
        len_A++;
    }
    else{
        T[out_A+len_A]=card;
        len_A++;
    }
}

void pushB(int T[], int card) {
    if(out_B + len_B>51){
        T[(out_B + len_B)%52]=card;
        len_B++;
    }
    else{
        T[out_B+len_B]=card;
        len_B++;
    }
}

int popB(int T[]) {
    int card = T[out_B];
    out_B++;
    if (out_B > 51) {
        out_B = 0;
    }
    len_B--;
    return card;
}

int popA(int T[]) {
    int card=T[out_A];
    out_A++;
    if(out_A>51){
        out_A=0;
    }
    len_A--;
    return card;
}

int main23(void){
    int seed;
    int version;
    int max;
    scanf("%d",&seed);
    scanf("%d",&version);
    scanf("%d",&max);
    srand(seed);
    int tab[52];
    int A[52];
    int B[52];
    rand_perm(52,tab);
    shuffle(tab,A,B,52);

    int conflicts = 0;
    int tmp_A[52];
    int tmp_B[52];
    int a = 0;
    int b = 0;
    int card_A;
    int card_B;

    if(version==0) {
        while (conflicts < max) {
            if (len_A == 52) {
                printf("%d %d", 2, conflicts);
                return 0;
            }

            if (len_B == 52) {
                printf("%d ", 3);
                for(int i=out_B;i<52;i++){
                    printf("%d ",B[i]);
                }

                for(int i=0;i<out_B;i++){
                    printf("%d ",B[i]);
                }
                return 0;
            }
            card_A = popA(A);
            card_B = popB(B);
            a = 0;
            b = 0;
            tmp_A[a] = card_A;
            tmp_B[b] = card_B;
            a++;
            b++;

            while (card_A / 4 == card_B / 4) {
                conflicts++;
                if (len_A <= 1 || len_B <= 1) {
                    printf("%d %d %d", 1, len_A + a, len_B + b);
                    return 0;
                }
                for (int i = 0; i < 2; i++) {
                    card_A = popA(A);
                    card_B = popB(B);
                    tmp_A[a] = card_A;
                    tmp_B[b] = card_B;
                    a++;
                    b++;
                }
            }
            if (card_A / 4 > card_B / 4) {
                for (int i = 0; i < a; i++) {
                    pushA(A, tmp_A[i]);
                }

                for (int i = 0; i < b; i++) {
                    pushA(A, tmp_B[i]);
                }
            }

            else {
                for (int i = 0; i < b; i++) {
                    pushB(B, tmp_B[i]);
                }

                for (int i = 0; i < a; i++) {
                    pushB(B, tmp_A[i]);
                }

            }
            conflicts++;
        }
    }

    else if(version==1){
        while(conflicts<max){
            if (len_A == 52) {
                printf("%d %d", 2, conflicts);
                return 0;
            }

            if (len_B == 52) {
                printf("%d ", 3);
                for(int i=out_B;i<52;i++){
                    printf("%d ",B[i]);
                }

                for(int i=0;i<out_B;i++){
                    printf("%d ",B[i]);
                }
                return 0;
            }

            card_A = popA(A);
            card_B = popB(B);
            conflicts++;

            if(card_A/4>card_B/4){
                pushA(A,card_A);
                pushA(A,card_B);
            }

            else if(card_A/4<card_B/4){
                pushB(B,card_B);
                pushB(B,card_A);
            }

            else{
                pushA(A,card_A);
                pushB(B,card_B);
            }
        }
    }
    printf("%d %d %d", 0, len_A, len_B);
    return 0;
}