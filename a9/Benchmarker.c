// Edited by Aiqi Jiang
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/time.h>
#include <time.h>

#include <unistd.h>
#include <errno.h>

#include "htll/Hashtable.h"
#include "htll/LinkedList.h"

#include "MovieSet.h"
#include "DocIdMap.h"
#include "FileParser.h"
#include "FileCrawler.h"
#include "MovieIndex.h"
#include "Movie.h"
#include "QueryProcessor.h"
#include "MovieReport.h"


DocIdMap docs;
Index docIndex;
Index movie_index;

SetOfMovies GetSetOfMovies(Index index, char* desc);
void PrintTitleWordReport(SetOfMovies setOfMovies, FILE* file);
void printOffsetQuery2Helper(SetOfMovies setOfMovies, FILE* file);
/**
 * Open the specified file, read the specified row into the
 * provided pointer to the movie.
 */
int CreateMovieFromFileRow(char *file, long rowId, Movie** movie) {
  FILE *fp;

  char buffer[1000];

  fp = fopen(file, "r");

  int i=0;
  while (i <= rowId) {
    fgets(buffer, 1000, fp);
    i++;
  }
  buffer[strlen(buffer)-1] = ' ';
  // Creat movie from row
  *movie = CreateMovieFromRow(buffer);
  fclose(fp);
  return 0;
}

void doPrep(char *dir) {
  printf("Crawling directory tree starting at: %s\n", dir);
  // Create a DocIdMap
  docs = CreateDocIdMap();
  CrawlFilesToMap(dir, docs);

  printf("Crawled %d files.\n", NumElemsInHashtable(docs));

  // Create the index
  docIndex = CreateIndex();

  // Index the files
  printf("Parsing and indexing files...\n");
  ParseTheFiles(docs, docIndex);
  printf("%d entries in the index.\n", NumElemsInHashtable(docIndex->ht));
}

void BenchmarkSetOfMovies(DocIdMap docs) {
  HTIter iter = CreateHashtableIterator(docs);
  // Now go through all the files, and insert them into the index.
  HTKeyValue kv;
  HTIteratorGet(iter, &kv);
  LinkedList movie_list  = ReadFile((char*)kv.value);
  movie_index = BuildMovieIndex(movie_list, Type);

  while (HTIteratorHasMore(iter) != 0) {
    HTIteratorGet(iter, &kv);
    movie_list = ReadFile((char*)kv.value);
    AddToMovieIndex(movie_index, movie_list, Type);
    HTIteratorNext(iter);
  }
  
  printf("%d entries in the index.\n", NumElemsInHashtable(movie_index->ht));
  DestroyHashtableIterator(iter);
}

void BenchmarkMovieSet(DocIdMap docs) {
  // Create the index
  docIndex = CreateIndex();

  // Index the files
  printf("Parsing and indexing files...\n");
  ParseTheFiles(docs, docIndex);
  printf("%d entries in the index.\n", NumElemsInHashtable(docIndex->ht));
  //return docIndex;
}

void WriteFile(FILE *file) {
  int buffer_size = 1000;
  char buffer[buffer_size];

  while (fgets(buffer, buffer_size, file) != NULL) {
    printf("%s\n", buffer);
  }
}

/*
 * Measures the current (and peak) resident and virtual memories
 * usage of your linux C process, in kB
 */
void getMemory() {

  int currRealMem, peakRealMem, currVirtMem, peakVirtMem;

  // stores each word in status file
  char buffer[1024] = "";

  // linux file contains this-process info
  FILE* file = fopen("/proc/self/status", "r");

  // read the entire file
  while (fscanf(file, " %1023s", buffer) == 1) {
    if (strcmp(buffer, "VmRSS:") == 0) {
      fscanf(file, " %d", &currRealMem);
    }
    if (strcmp(buffer, "VmHWM:") == 0) {
      fscanf(file, " %d", &peakRealMem);
    }
    if (strcmp(buffer, "VmSize:") == 0) {
      fscanf(file, " %d", &currVirtMem);
    }
    if (strcmp(buffer, "VmPeak:") == 0) {
      fscanf(file, " %d", &peakVirtMem);
    }
  }

  fclose(file);

  printf("Cur Real Mem: %d\tPeak Real Mem: %d "
         "\t Cur VirtMem: %d\tPeakVirtMem: %d\n",
         currRealMem, peakRealMem,
         currVirtMem, peakVirtMem);
}

// function for querying movies that have seattle in title with offsetindex
void runQueryTitleWord(char *term, DocIdMap docs, char* desc) {
  SearchResultIter results = FindMovies(docIndex, term);
  LinkedList movies = CreateLinkedList();

  if (results == NULL) {
    printf("No results for this term. Please try another.\n");
    return;
  } else {
    SearchResult sr = (SearchResult)malloc(sizeof(*sr));
    if (sr == NULL) {
      printf("Couldn't malloc SearchResult in main.c\n");
      return;
    }
    int result;
    SearchResultGet(results, sr);
    char *filename;
    filename = GetFileFromId(docs, sr->doc_id);

    Movie *movie;
    CreateMovieFromFileRow(filename, sr->row_id, &movie);
    InsertLinkedList(movies, movie);

    // Check if there are more
    while (SearchResultIterHasMore(results) != 0) {
      result =  SearchResultNext(results);
      if (result < 0) {
        printf("error retrieving result\n");
        break;
      }
      SearchResultGet(results, sr);
      char *filename = GetFileFromId(docs, sr->doc_id);

      Movie *movie;
      CreateMovieFromFileRow(filename, sr->row_id, &movie);
      InsertLinkedList(movies, movie);
    }
    free(sr);
    DestroySearchResultIter(results);
  }
  // Now that you have all the search results, print them out nicely.
  Index index = BuildMovieIndex(movies, Type);
  PrintTitleWordReport((SetOfMovies)GetSetOfMovies(index, desc), stdout);
  DestroyTypeIndex(index);
}
// helper for printing out the movie titles that has required
// word in title and the required type.
void PrintTitleWordReport(SetOfMovies setOfMovies, FILE* file) {
  int cnt = 0;
  if (!setOfMovies) {
    fprintf(file, "\t%s%d\n", "There are", cnt);
    return;
  }
  LLIter iter = CreateLLIter(setOfMovies->movies);
  if (iter == NULL) {
    fprintf(file, "iter null for some reason.. \n");
    fprintf(file, "\t%s%d\n", "There are", cnt);
    return;
  }
  Movie* movie;
  LLIterGetPayload(iter,(void**)&movie);
  fprintf(file, "\t%s\n", movie->title);
  cnt ++;
  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter,(void**)&movie);
    fprintf(file, "\t%s\n", movie->title);
    cnt++;
  }
  free(iter);
   fprintf(file, "There are %d files!\n", cnt);
}
// function for printing out How many Action movies are there in the database
void runQueryTypeGenre(char*desc, FILE* file) {
  SetOfMovies setOfMovies = GetSetOfMovies(movie_index, desc);
  LLIter iter = CreateLLIter(setOfMovies->movies);
  if (iter == NULL) {
    fprintf(file, "iter null for some reason.. \n");
    return;
  }
  Movie* movie;
  LLIterGetPayload(iter,(void**)&movie);
  int cnt = 0;
  for (int i = 0; i < NUM_GENRES; i++) {
    if (movie->genres[i] != NULL && strncmp(movie->genres[i], "Action", strlen("Action")) == 0) {
      fprintf(file, "\t%s\n", movie->title);
      cnt++;
      break;
    }
  }
  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter,(void**)&movie);
    for (int i = 0; i < NUM_GENRES; i++) {
      if (movie->genres[i] != NULL && strncmp(movie->genres[i], "Action", strlen("Action")) == 0) {
          fprintf(file, "\t%s\n", movie->title);
	  cnt++;
          break;
        }
      }
  }
  free(iter);
  printf("There are %d action movies!\n", cnt);
}

SetOfMovies GetSetOfMovies(Index index, char* desc) {
  HTKeyValue kvp;
  printf("set begin\n");
  // search for the setofmovies with required desc.
  if (! index->ht) return NULL;
  int result = LookupInHashtable(index->ht,
                                 FNVHash64((unsigned char*)desc, 
                                 strlen(desc)),
                                 &kvp);
  printf("lookup end\n");
  if (result < 0) {
    printf("there is no such term\n");
    return NULL;
  }
  printf("returning movieset\n");
  return (SetOfMovies)kvp.value;
}

// used for quering What movies have Seattle in the title and are Rom/Coms
// based on offsetindex
void runOffsetQuery2(char *term, DocIdMap docs, char* desc) {
  SearchResultIter results = FindMovies(docIndex, term);
  LinkedList movies = CreateLinkedList();

  if (results == NULL) {
    printf("No results for this term. Please try another.\n");
    return;
  } else {
    SearchResult sr = (SearchResult)malloc(sizeof(*sr));
    if (sr == NULL) {
      printf("Couldn't malloc SearchResult in main.c\n");
      return;
    }
    int result;
    SearchResultGet(results, sr);
    char *filename;
    filename = GetFileFromId(docs, sr->doc_id);

    Movie *movie;
    CreateMovieFromFileRow(filename, sr->row_id, &movie);
    InsertLinkedList(movies, movie);

    // Check if there are more
    while (SearchResultIterHasMore(results) != 0) {
      result =  SearchResultNext(results);
      if (result < 0) {
        printf("error retrieving result\n");
        break;
      }
      SearchResultGet(results, sr);
      char *filename = GetFileFromId(docs, sr->doc_id);

      Movie *movie;
      CreateMovieFromFileRow(filename, sr->row_id, &movie);
      InsertLinkedList(movies, movie);
    }
    free(sr);
    DestroySearchResultIter(results);
  }
  // Now that you have all the search results, print them out nicely.
  Index index = BuildMovieIndex(movies, Type);
  printOffsetQuery2Helper((SetOfMovies)GetSetOfMovies(index, desc), stdout);
  DestroyTypeIndex(index);
}
// help offsetindex to output the filnames
void printOffsetQuery2Helper(SetOfMovies setOfMovies, FILE* file) {
  int cnt = 0;
  if (!setOfMovies) {
     fprintf(file, "There are %d files!\n", cnt);
    return;
  }
  LLIter iter = CreateLLIter(setOfMovies->movies);
  if (iter == NULL) {
    fprintf(file, "iter null for some reason.. \n");
    fprintf(file, "There are %d files!\n", cnt);
    return;
  }
  Movie* movie;
  LLIterGetPayload(iter,(void**)&movie);
  for (int i = 0; i < NUM_GENRES; i++) {
    if (movie->genres[i] != NULL && ((strncmp(movie->genres[i], "Comedy", strlen("Comedy")) == 0) || (strncmp(movie->genres[i], "Romance", strlen("Romance")) == 0))) {
      fprintf(file, "\t%s\n", movie->title);
      cnt++;
      break;
    }
  }
  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter,(void**)&movie);
    for (int i = 0; i < NUM_GENRES; i++) {
      if (movie->genres[i] != NULL && ((strncmp(movie->genres[i], "Comedy", strlen("Comedy")) == 0)
				       || (strncmp(movie->genres[i], "Romance", strlen("Romance")) == 0))) {
      fprintf(file, "\t%s\n", movie->title);
      cnt++;
      break;
    }
  }
  }
  free(iter);
  fprintf(file, "There are %d files!\n", cnt);
}
// function for querying movies have Seattle in the title and are Rom/Coms
// based on typeindex
 void runQueryType2(char*desc, FILE* file) {
  int cnt = 0;
  printf("run query type genre\n");
  SetOfMovies setOfMovies = GetSetOfMovies(movie_index, desc);
  printf("get set of movies\n");
  LLIter iter = CreateLLIter(setOfMovies->movies);
  if (iter == NULL) {
    fprintf(file, "iter null for some reason.. \n");
    fprintf(file, "There are %d files!\n", cnt);
    return;
  }
  Movie* movie;
  LLIterGetPayload(iter,(void**)&movie);
  for (int i = 0; i < NUM_GENRES; i++) {
    if (movie->genres[i] != NULL && ((strncmp(movie->genres[i], "Comedy", strlen("Comedy")) == 0) || (strncmp(movie->genres[i], "Romance", strlen("Romance")) == 0))
	&& strstr(movie->title, "Seattle") != NULL) {//
      fprintf(file, "\t%s\n", movie->title);
      cnt++;
      break;
    }
  }
  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter,(void**)&movie);
    for (int i = 0; i < NUM_GENRES; i++) {
      if (movie->genres[i] != NULL && ((strncmp(movie->genres[i], "Comedy", strlen("Comedy")) == 0) || (strncmp(movie->genres[i], "Romance", strlen("Romance")) == 0))
	  && strstr(movie->title, "Seattle") != NULL){//
          fprintf(file, "\t%s\n", movie->title);
          cnt++;
          break;
        }
      }
  }
   fprintf(file, "There are %d files!\n", cnt);
 }
int main(int argc, char *argv[]) {
  // Check arguments
  if (argc != 2) {
    printf("Wrong number of arguments.\n");
    printf("usage: main <directory_to_crawl>\n");
    return 0;
  }
  pid_t pid = getpid();
  printf("Process ID: %d\n", pid);
  getMemory();

  // Create a DocIdMap
  docs = CreateDocIdMap();
  CrawlFilesToMap(argv[1], docs);
  printf("Crawled %d files.\n", NumElemsInHashtable(docs));
  printf("Created DocIdMap\n");
  getMemory();
  clock_t start2, end2, start3, end3, start4, end4, start5, end5, start6, end6;
  double cpu_time_used;

  // =======================
    // Benchmark MovieSet
  printf("\n\nBuilding the OffsetIndex\n");
  start2 = clock();
  BenchmarkMovieSet(docs);
  end2 = clock();
  cpu_time_used = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
  printf("Took %f seconds to execute. \n", cpu_time_used);
  printf("Memory usage: \n");
  getMemory();
 
  printf("\n\nQuerying seattle movies based on OffsetIndex\n");
  start3 = clock();
  runQueryTitleWord("Seattle", docs, "movie");
  end3 = clock();
  cpu_time_used = ((double) (end3 - start3)) / CLOCKS_PER_SEC;
  printf("Took %f seconds to execute. \n", cpu_time_used);
  printf("Memory usage: \n");
  getMemory();
  
  printf("\n\nOffsetIndex querying movies have Seattle in the title and are Rom/Coms\n");
  start5 = clock();
  runOffsetQuery2("Seattle", docs, "movie");
  end5 = clock();
  cpu_time_used = ((double) (end5 - start5)) / CLOCKS_PER_SEC;
  printf("Took %f seconds to execute. \n", cpu_time_used);
  printf("Memory usage: \n");
  getMemory();
  DestroyOffsetIndex(docIndex);
  printf("Destroyed OffsetIndex\n");
  getMemory();
  
  // =======================

  // ======================
  // Benchmark SetOfMovies
  
  printf("\n\nBuilding the TypeIndex\n");
  start2 = clock();
  BenchmarkSetOfMovies(docs);
  end2 = clock();
  cpu_time_used = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
  printf("Took %f seconds to execute. \n", cpu_time_used);
  printf("Memory usage: \n");
  getMemory();
  getMemory();
  
  printf("\n\nQuerying Action movies based on typeIndex\n");
  start4 = clock();
  
  runQueryTypeGenre("movie", stdout);
  
  end4 = clock();
  cpu_time_used = ((double) (end4 - start4)) / CLOCKS_PER_SEC;
  printf("Took %f seconds to execute. \n", cpu_time_used);
  printf("Memory usage: \n");
  
  getMemory();
  printf("Destroyed typeIndex\n");
  getMemory();
  
  printf("\n\nQuerying movies have Seattle in the title and are Rom/Coms based on typeindex\n");
  start6 = clock();
  runQueryType2("movie", stdout);
  end6 = clock();
  cpu_time_used = ((double) (end6 - start6)) / CLOCKS_PER_SEC;
  printf("Took %f seconds to execute. \n", cpu_time_used);
  printf("Memory usage: \n");

  getMemory();
  DestroyTypeIndex(movie_index);
  printf("Destroyed typeIndex\n");
  getMemory();
  
  // ======================
  
  DestroyDocIdMap(docs);
  printf("\n\nDestroyed DocIdMap\n");
  getMemory();

}
