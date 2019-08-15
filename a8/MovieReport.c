#include <stdio.h>
#include <stdlib.h>
#include "MovieIndex.h"
#include "MovieReport.h"
#include "Movie.h"
#include "MovieSet.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"
//  Edited by Aiqi Jiang
// Prints a report of an index, assuming the key
// is a field/string and the value is a SetOfMovies.
void PrintReport(Index index) {
  OutputReport(index, stdout);
}
void OutputListOfMovies(LinkedList movie_list, char *desc, FILE *file) {
  fprintf(file, "%s: %s\n", "indexType", desc);
  fprintf(file, "%d items\n", NumElementsInLinkedList(movie_list));
  LLIter iter = CreateLLIter(movie_list);
  if (iter == NULL) {
    fprintf(file, "iter null for some reason.. \n");
    return;
  }
  Movie *movie;
  LLIterGetPayload(iter, (void**)&movie);
  if (movie->title != NULL) {
    fprintf(file, "\t%s\n", movie->title);
  } else {
    fprintf(file, "title is null\n");
  }
  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&movie);
    if (movie->title != NULL) {
      fprintf(file, "\t%s\n", movie->title);
    } else {
      fprintf(file, "title is null\n");
    }
  }
  DestroyLLIter(iter);
}

// Assumes the value of the index is a SetOfMovies
void OutputReport(Index index, FILE* output) {
  // TODO: Implement this function.
  // After you've implemented it, consider modifying PrintReport()
  // to utilize this function.
  HTIter it = CreateHashtableIterator(index->ht);
  if (it == NULL) {
    DestroyHashtableIterator(it);
    return;
  }
    HTKeyValue ptr;
    HTIteratorGet(it, &ptr);
    OutputListOfMovies(((SetOfMovies)ptr.value)->movies,
((SetOfMovies)ptr.value)->desc, output);
    while (HTIteratorHasMore(it) != 0) {
      HTIteratorNext(it);
      HTIteratorGet(it, &ptr);
      OutputListOfMovies(((SetOfMovies)ptr.value)->movies,
((SetOfMovies)ptr.value)->desc, output);
    }
    DestroyHashtableIterator(it);
}

void SaveReport(Index index, char* filename) {
  // TODO: Implement this. You might utilize OutputReport.
}

