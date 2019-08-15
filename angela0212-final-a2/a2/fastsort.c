#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define L 102400
#define M 12800
#define S 800
#define TRIAL 30

void quickSort(int A[], int p, int r);
int Partition(int A[], int p, int r);
double FastSort(int num);
int main(void)
{   time_t start, end;
    double total = 0;
    srand(time(0));
    double large= FastSort(L);
    double median=FastSort(M);
    double small=FastSort(S);
    printf("---------------------\nFAST SORT: QUICKSORT\n---------------------\n");
    printf("%d trials\n", TRIAL);
    printf("Large:  %d elements  %f msecs\n", L, large);
    printf("Med:  %d elements  %f msecs\n", M, median);
    printf("Small:  %d elements  %f msecs\n", S, small);

    return 0;
}

double FastSort(int num){
        int i;
        int A[num];
        double total=0;
        for(i=0;i<num;i++){
            A[i] = rand();
        }
        int B[num];
        for(i=0;i<num;i++){
            B[i]=A[i];
        }
        for(int s=0;s<TRIAL;s++){

            time_t start = clock();
            quickSort(A, 0, num-1);
            time_t end = clock();
            total += end-start;
            for(i=0;i<num;i++){
                A[i]=B[i];
        }
        }
        return total/TRIAL;
    }

void quickSort(int A[], int p, int r){
    int q;
    if(p < r){
        q=Partition(A, p, r);
        quickSort(A, p, q-1);
        quickSort(A, q+1, r);
    }
}

int Partition(int A[], int p, int r){
    int x = A[r];
    int i = p-1;
    for(int j=p;j<= r-1;j++){
        if(A[j]<=x){
            i++;
            int temp = A[i];
           A[i] = A[j];
            A[j] = temp;
        }
    }
    int exc = A[i+1];
    A[i+1] = A[r];
    A[r]=exc;
    return i+1;
    }
