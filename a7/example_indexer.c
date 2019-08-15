#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "htll/LinkedList.h"
#include "MovieIndex.h"
#include "Movie.h"
#include "MovieReport.h"
#include "./FileParser.h"



int main(int argc, char*[] args){
  LinkedList movie_list  = ReadFile("data/test");

  // TODO: Parse command-line args to index based on the correct field
  Index index = BuildMovieIndex(movie_list, Year); 

  // TODO: Output report to file, rather than terminal (see MovieReport.h)
  PrintReport(index); 
  
  DestroyIndex(index);
  
  return 0; 
}

