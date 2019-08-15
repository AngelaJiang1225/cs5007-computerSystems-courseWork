// Edited by Aiqi Jiang

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

#include "MovieIndex.h"
#include "FileParser.h"
#include "Movie.h"
#include "DocIdMap.h"
#include "MovieSet.h"

#define NTHREADS 5
#define BUFFER_SIZE 1000
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
//=======================
 
void IndexTheFile(char *file, uint64_t docId, Index index);
void IndexTheFile_MT_Helper(char *file, uint64_t docId, Index index);
void* IndexTheFile_MT(void *argp);
//=======================

struct index_args {
  char *file;
  uint64_t docId;
  Index index;
};

int ParseTheFiles(DocIdMap docs, Index index) { 
  clock_t start, end;
  double cpu_time_used;
  start = clock();
  // For all the files in the docIdMap
  // Index the file. 
  // YOUR CODE GOES HERE
  ParseTheFiles_MT(docs, index); 
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("\n\nTook %f microseconds to parse %d files. \n\n", cpu_time_used, NumElemsInHashtable(docs));
  return 0; 
}


void IndexTheFile(char *file, uint64_t docId, Index index) {
  FILE *cfPtr;
  // Open a file
  // Use CreateMovieFromRow to create a movie
  // Put it in the index

  // YOUR CODE GOES HERE
  if ((cfPtr = fopen(file, "r")) == NULL) {
    printf("File could not be opened\n");
    return;
  } else {
    char buffer[BUFFER_SIZE];
    int row = 0;
    
    while(fgets(buffer, BUFFER_SIZE, cfPtr) !=NULL) {
    Movie *movie = CreateMovieFromRow(buffer);
      if (movie == NULL) {
        continue;
      }
      int result = AddMovieTitleToIndex(index, movie, docId, row);
      if (result < 0) {
        fprintf(stderr, "Didn't add MovieToIndex.\n");
      }
      row++; 
      DestroyMovie(movie);
  }
  fclose(cfPtr);
}
}


int ParseTheFiles_MT(DocIdMap docs, Index index) {
  // Create the threads for parsing the file
  pthread_t tid[5];
  HTIter iter = CreateHashtableIterator(docs);
  HTKeyValue kv;
  HTIteratorGet(iter, &kv);
  int num = 0;
  //IndexTheFile_MT(kv.value, kv.key, index);
  
  while (HTIteratorHasMore(iter) != 0) {
    HTIteratorGet(iter, &kv);
    char *file = GetFileFromId(docs, kv.key);
    struct index_args *args = malloc(sizeof *args);
    args->file = file;
    args->docId = kv.key;
    args->index = index;
    // start threading
    pthread_create(&tid[num], NULL, IndexTheFile_MT, args);
    if (num < 4) {
      num++;
    } else {
      num = 0;
    }
      for (int i = 0; i < num; i++) {
        pthread_join(tid[i], NULL);
      }
     HTIteratorNext(iter);
  }
  DestroyDocIdIterator(iter);
  return 0; 
}

void* IndexTheFile_MT(void *argp) {
  struct index_args *a = argp;
  char *file = a->file;
  uint64_t docId = a->docId;
  Index index = a->index;
  IndexTheFile_MT_Helper(file, docId, index);
  free(a);
  return argp;
}

void IndexTheFile_MT_Helper(char *file, uint64_t docId, Index index) {
  // index the file with lock and unlock
  FILE *cfPtr;
  pthread_mutex_lock(&m);
  cfPtr = fopen(file, "r");
  pthread_mutex_unlock(&m);

  char buffer[1024];
  int rowId = 0;
  while(fgets(buffer, sizeof(buffer), cfPtr)) {
    Movie* movie = CreateMovieFromRow(buffer);
    pthread_mutex_lock(&m);
    //printf("adding %s to index\n", movie->title);
    AddMovieTitleToIndex(index, movie, docId, rowId); 
    pthread_mutex_unlock(&m);
    DestroyMovie(movie);
    rowId++;
  }
  fclose(cfPtr);
}

// Takes a linkedlist of movies, and builds a hashtable based on the given field
// Builds a TypeIndex
Index BuildMovieIndex(LinkedList movies, enum IndexField field_to_index) {
  Index movie_index = CreateIndex();
  movie_index->movies = movies;

  LLIter iter = CreateLLIter(movies);
  Movie* cur_movie;
  LLIterGetPayload(iter, (void**)&cur_movie);

  AddMovieToIndex(movie_index, cur_movie, field_to_index);

  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&cur_movie);
    AddMovieToIndex(movie_index, cur_movie, field_to_index);
  }

  DestroyLLIter(iter);

  return movie_index;
}

Index AddToMovieIndex(Index movie_index,
                      LinkedList movies,
                      enum IndexField field_to_index) {
  LLIter iter = CreateLLIter(movies);
  Movie* cur_movie;
  LLIterGetPayload(iter, (void**)&cur_movie);

  AddMovieToIndex(movie_index, cur_movie, field_to_index);
  InsertLinkedList(movie_index->movies, cur_movie);

  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&cur_movie);
    AddMovieToIndex(movie_index, cur_movie, field_to_index);
    InsertLinkedList(movie_index->movies, cur_movie);
  }

  DestroyLLIter(iter);

  //  NullFree because the payloads now live in the HT
  DestroyLinkedList(movies, NullFree);

  return movie_index;
}

// Returns a LinkedList of Movie structs from the specified file
LinkedList ReadFile(const char* filename) {
  FILE *cfPtr;

  LinkedList movie_list = CreateLinkedList();

  if ((cfPtr = fopen(filename, "r")) == NULL) {
    printf("File could not be opened\n");
    DestroyLinkedList(movie_list, NULL);
    return NULL;
  } else {
    char row[BUFFER_SIZE];

    int i = 0;

    while (!feof(cfPtr)) {
      fgets(row, BUFFER_SIZE, cfPtr);
      i++;
      // Got the line; create a movie from it
      MoviePtr movie = CreateMovieFromRow(row);
      if (movie != NULL) {
        InsertLinkedList(movie_list, movie);
      }
    }
    fclose(cfPtr);
  }
  return movie_list;
}

