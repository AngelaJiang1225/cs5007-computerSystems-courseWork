#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#define N 1000

void readFile(char *city[N], double matrix[N][N]) {
  int lines = 0;  // number of lines
  int num = 0;
  int cityIndex = -1;
  FILE *cfPtr;
  char *line;
  char buffer[2^32 * 1001];
  if ((cfPtr = fopen("miles_graph_FINAL.csv", "r")) == NULL) {
    printf("File could not be opened\n");
  } else {
    fseek(cfPtr, 0, SEEK_SET);
    while ((line = fgets(buffer, sizeof(buffer), cfPtr))
  != NULL && !feof(cfPtr)) {
      lines += 1;
      char *tempRes;
      char *s = strdup(buffer);
      int len = 0;  // number of data in each line
      for (tempRes = strsep(&s, ","); tempRes != NULL;
  tempRes = strsep(&s, ",")) {
        if (lines == 1 && len > 0) {
          cityIndex += 1;
          city[cityIndex] = tempRes;
        } else if (lines > 1 && len > 0) {
          num += 1;
  matrix[lines - 2][len - 1] = atof(tempRes);
  }
        len += 1;
      }
    free(s);
    }
  fclose(cfPtr);
}
}
int changeToNum(char *city[], char *s) {
  for (int i = 0; i < N; i++) {
    if (strcmp(city[i], s) == 0) {
      return i;
    }
  }
}
// methods for dijkstra: s for start place,
// e for end place, dist[N] to store the
// minimum distance from each place to the s
void dijkstra(double matrix[N][N], int s, int e, char* city[], double dist[N]) {
  double mindist;
  // used is the array of elements that already have found their minimun value
  int used[N], count, prev[N], minindex;  // the index that store the min value
  for (int i = 0;i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] < 0) {
        matrix[i][j] = INT_MAX;
      }
    }
  }
// initialize pred[], dist[] and used[]
  for (int i = 0; i < N; i++) {
    dist[i] = matrix[s][i];
    prev[i] = s;
    used[i] = 0;
  }
  used[s] = 1;
  // count iteration times
  count = 1;
  while (count < N) {
    mindist = INT_MAX;
    for (int i = 0; i < N; i++) {
  if (dist[i] < mindist && dist[i] > 0 && used[i] == 0) {
  mindist = dist[i];
  minindex = i;
  used[i] = 1;
  }
    }
    // find if there are other ways that give other nodes smaller values
    for (int j = 0; j < N; j++) {
       if (mindist + matrix[minindex][j]
< dist[j] && used[j] == 0 && dist[j] > 0) {
  dist[j] = mindist + matrix[minindex][j];
  prev[j] = minindex;
  }
    }
    count++;
  }
  // print out the path result
  printf("\n%s to %s: %f miles", city[s], city[e], dist[e]);
  printf("\n");
  printf("<- %s", city[e]);
      int j = e;
      do {
        j = prev[j];
        printf("<- %s", city[j]);
      } while (j != s);
}
// main function
int main() {
  double matrix[N][N];
  char *city[N];
  double dist[N];
  int s, e;
  readFile(city, matrix);
  s = changeToNum(city, "Seattle_WA");
  e = changeToNum(city, "Boston_MA");
  dijkstra(matrix, s, e, city, dist);
  s = changeToNum(city, "Minneapolis_MN");
  e = changeToNum(city, "Ann Arbor_MI");
  dijkstra(matrix, s, e, city, dist);
  return 0;
}
