#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#define N 1000
// Node structure
struct Node {
  int end;  // the destination
  int time;
  struct Node* next;
};
// adjacency list
struct List {
  struct Node *head;  // pointer to head node of list
};
// array of lists
struct Graph {
  int n;  // size
  struct List* array;
};
int changeToId(char *name[N], char *city);
void addEdge(struct Graph* graph, int start, int end, int time);
char ** readNodes(char * city[N]) {
  int lines = 0;  // number of lines
  FILE *cfPtr;
  char buffer[2^32 * 1001];
  int id = 0;
  int len;  // number of each line
  char *line;
  if ((cfPtr = fopen("nodes.csv", "r")) == NULL) {
    printf("File could not be opened\n");
  } else {
    fseek(cfPtr, 0, SEEK_SET);
    while ((line = fgets(buffer, sizeof(buffer), cfPtr))
!= NULL && !feof(cfPtr)) {
      char *tempRes;
      char *s = strdup(buffer);
      len = 0;
      for (tempRes = strsep(&s, ","); tempRes != NULL;
tempRes = strsep(&s, ",")) {
        if (id != 0 && len == 0) {
          scanf(tempRes, "%1d", &id);
          len += 1;
        } else if (id != 0 && len == 1) {
          city[id - 1] = tempRes;
        }
      }

    free(s);
    id += 1;
    }
    fclose(cfPtr);
    return city;
  }
}
int changeToId(char *name[N], char *city) {
  for (int i = 0; i< N;i++) {
    if (strcmp(city, name[i]) == 0) {
      return i;
    }
  }
  return 0;
}
int readEdges(char *city[N], int matrix[N][N], struct Graph *graph) {
  int lines = 0;  // number of lines
  int num = 0;
  FILE *cfPtr;
  char *line;
  char buffer[2^32 * 1001];
  if ((cfPtr = fopen("edges_FINAL.csv", "r")) == NULL) {
    printf("File could not be opened\n");
  } else {
    fseek(cfPtr, 0, SEEK_SET);
    while ((line = fgets(buffer, sizeof(buffer), cfPtr))
  != NULL && !feof(cfPtr)) {
      lines += 1;
      char *tempRes;
      char *s = strdup(buffer);
      int len = 0;  // number of data in each line
      int start;
      int end;
      int index = 0;
      int time = 0;
      int hour = 0;
      int minute = 0;
      int seconds = 0;
      char *update;
      for (tempRes = strsep(&s, " "); tempRes != NULL;
tempRes = strsep(&s, " ")) {
        if (index == 0) {
          start = changeToId(city, tempRes);
        }
        if (index == 1) {
          end = changeToId(city, tempRes);
        } else if (index == 2) {
  hour = atoi(tempRes);
          time += hour * 3600;
        } else if (index == 3) {
          minute = atoi(tempRes);
          time += minute * 60;
        } else if (index == 4) {
          seconds = atoi(tempRes);
          time += seconds;
          matrix[start][end] = time;
          addEdge(graph, start, end, time);
        }
        index += 1;
      }
      free(s);
  }
    fclose(cfPtr);
  }
}
struct Node* createNode(int end, int time) {
  struct Node* newNode =
  (struct Node*) malloc(sizeof(struct Node));
  newNode->end = end;
  newNode->time = time;
  newNode->next = NULL;
  return newNode;
}
// creates a graph of N Nertices
struct Graph* createGraph(int n) {
  struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
  graph->n = n;
  // Create an array of adjacency lists
  graph->array = (struct List*) malloc(n * sizeof(struct List));
  // Initialize each adjacency list as empty by making head as NULL
  for (int i = 0; i < n; ++i)
    graph->array[i].head = NULL;
  return graph;
}
// Adds an edge
void addEdge(struct Graph* graph, int start, int end, int time) {
  struct Node* newNode = createNode(end, time);
  newNode->next = graph->array[start].head;
  graph->array[start].head = newNode;
  newNode = createNode(start, time);
  newNode->next = graph->array[end].head;
  graph->array[end].head = newNode;
}
void dijkstra(struct Graph *graph , int s, int dist[N]) {
  int minTime;
  int minIndex;
  // used is the array of elements that already have found their minimun value
  int used[N], count, prev[N], minindex;  // the index that store the min value
// initialize pred[] and used[]
  for (int i = 0;i < N;i++) {
    prev[i] = s;
    used[i] = 0;
  }
  used[s] = 1;
  // count iteration times
  count = 1;
  while (count < N) {
    minTime = INT_MAX;
    struct Node *node = graph->array[s].head;
    while (node != NULL) {
      dist[node->end] = node -> time;
      if (dist[node->end] < minTime) {
        minTime = dist[node->end];
  minIndex = node->end;
  node = node->next;
      }
    }
    used[minIndex] = 1;
    // find if there are other ways that give other nodes smaller values
    struct Node* curNode = graph->array[minIndex].head;
    while (curNode != NULL) {
      if (minTime + curNode->time < dist[curNode->end]
  && used[curNode->end] == 0 && dist[curNode->end] > 0) {
  dist[curNode->end] = minTime + curNode->time;
  prev[curNode->end] = minindex;
       }
    }
    count++;
  }
}
// If  I operate the main function,it will have segmentation error,
// I tried everything to modify it but still cant get avoid of it,
// so I comment it.thanks!!!!
void main() {
  char *city [N];
  readNodes(city);
  struct Graph *graph = createGraph(N);
  int matrix[N][N];
  int dist[N];
  /*
  readEdges(city, matrix, graph);
  
  dijkstra(graph, changeToId(city, "Seattle_WA"), dist);
  printf("from %s to %s : %d seconds", "Seattle_WA", "Boston_MA", dist[changeToId(city, "Boston_MA")]);
  dijkstra(graph, changeToId(city, "Ann Arbor_MI"), dist);
  printf("from %s to %s : %d seconds", "Ann Arbor_MI", "Minneapolis_MN", dist[changeToId(city, "Minneapolis_MN")]);
  for (int i = 0; i < N; i++) {
    struct Node *node = graph->array[i].head;
    while (node != NULL) {
      free(node);
      node = node->next;
    }
  }
*/
}
