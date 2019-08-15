
#include <stdio.h>
#include <stdlib.h>

#include "MovieIndex.h"
#include "Movie.h"
#include "MovieSet.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"


void PrintReport(Index index) {
  // Create Iter
  HTIter iter = CreateHashtableIterator(index);

  HTKeyValue movie_set;

  HTIteratorGet(iter, &movie_set);
  OutputMovieSet((MovieSet)movie_set.value);

  while (HTIteratorHasMore(iter)) {
    HTIteratorNext(iter);
    HTIteratorGet(iter, &movie_set);
    OutputMovieSet((MovieSet)movie_set.value); 
  }
  // For every movie set, create a LLIter
  
  DestroyHashtableIterator(iter); 
}

void OutputMovieSet(MovieSet movie_set) {
  // TODO: Output the type, not "indexType" (replace that hardcoded string)
  printf("%s: %s\n", "indexType", movie_set->desc);
  printf("%d items\n", NumElementsInLinkedList(movie_set->movies)); 
  LLIter iter = CreateLLIter(movie_set->movies);
  if (iter == NULL) {
    printf("iter null for some reason.. \n");
    return; 
  }
  Movie *movie;
  
  LLIterGetPayload(iter, (void**)&movie);
  printf("\t%s\n", movie->title); 

  while(LLIterHasNext(iter)) {
    LLIterNext(iter); 
    LLIterGetPayload(iter, (void**)&movie);
    printf("\t%s\n", movie->title); 
  }

  DestroyLLIter(iter); 
  
}


void OutputReport(Index index, FILE output) {
  // TODO: Implement this. 
  // After you've implemented it, consider modifying PrintReport() 
  // to utilize this function. 
}

void SaveReport(Index index, char* filename) {
  // TODO: Implement this. You might utilize OutputReport. 

}

