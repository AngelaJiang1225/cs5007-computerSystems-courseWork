#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define L 102400
#define M 12800
#define S 800
#define TRIAL 30
void insertSort(int A[],int len);
double SlowSort(int num);
int main(void)
{
    srand(time(0));
    double large= SlowSort(L);
    double median=SlowSort(M);
    double small=SlowSort(S);

    printf("---------------------\nSLOW SORT: INSERTION\n---------------------\n");
    printf("%d trials\n",TRIAL);
    printf("Large:  %d elements  %f msecs\n", L, large);
    printf("Med:  %d elements  %f msecs\n", M, median);
    printf("Small:  %d elements  %f msecs\n", S, small);

    return 0;
  }

  double SlowSort(int num){
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
        time_t start,end;
        for(int s=0;s<TRIAL;s++){
            start = clock();
            insertSort(A,num);
            end = clock();
            total += end - start;
            for(i=0;i<num;i++){
                A[i]=B[i];
            }
        }
        return total/TRIAL;
  }

void insertSort(int A[],int len){
  int j;
  int i;
  int key;
  for(j=1;j<len;j++){
    key=A[j];
    i=j-1;
    while(i>=0 && A[i]>key){
      A[i+1] = A[i];
      i--;
    }
    A[i+1]=key;
  }
}
