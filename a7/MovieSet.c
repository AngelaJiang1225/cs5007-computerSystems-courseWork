#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MovieSet.h"
#include "Movie.h"
#include "htll/LinkedList.h"


void DestroyMovieWrapper(void *a_movie) {
  DestroyMovie((Movie*)a_movie); 
}

int AddMovieToSet(MovieSet set, Movie *movie) {
  int result = InsertLinkedList(set->movies, movie);
  return result; 
}


MovieSet CreateMovieSet(char *desc) {
  MovieSet set = (MovieSet)malloc(sizeof(struct movieSet));
  if (set == NULL) {
    // Out of memory
    printf("Couldn't malloc for movieSet %s\n", desc);
    return NULL; 
  }
  set->desc = (char*)malloc(strlen(desc) *  sizeof(char) + 1);
  if (set->desc == NULL) {
    printf("Couldn't malloc for movieSet->desc");
    return NULL; 
  }
  strcpy(set->desc, desc); 
  set->movies = CreateLinkedList(); 
  return set; 
}

void DestroyMovieSet(MovieSet set) {
  // Free desc
  free(set->desc);
  // Free the list and the movies
  DestroyLinkedList(set->movies, &DestroyMovieWrapper); 
  // Free set
  free(set); 
}
