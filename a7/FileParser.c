/*
 *  Modifications 
 *  Copyright 2018 Adrienne Slaughter, Northeastern University. 
 *  Based on code created by Steven Gribble (under GNU GPL). 
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  See <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include "FileParser.h"
#include "Movie.h"
#include "MovieIndex.h"

#define ASCII_UPPER_BOUND 0x7F


Index BuildMovieIndex(LinkedList movies, enum IndexField field_to_index){
  // TODO: How big to make this hashtable? How to decide? What to think about?
  // Make this "appropriate".  
  Index movie_index = CreateHashtable(100);

  LLIter iter = CreateLLIter(movies);
  Movie* cur_movie;
  LLIterGetPayload(iter, (void**)&cur_movie);

  // TODO: Check that there is at least one movie
  // What happens if there is not at least one movie? 
  // How can we modify a piece(s) of our system to not have to do this check? 
  int result = AddMovieToIndex(movie_index, cur_movie, field_to_index); 

  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&cur_movie);
    result = AddMovieToIndex(movie_index, cur_movie, field_to_index); 
    if (result == 2) {
      // TODO: What to do if there is a collision? 
      // How might we change the system to help us deal with collisions? 
    }
  }
  return movie_index; 
}
      
// Returns a LinkedList of Movie structs from the specified file
LinkedList ReadFile(char* filename){
  FILE *cfPtr;

  LinkedList movie_list = CreateLinkedList(); 

  if ((cfPtr = fopen(filename, "r")) == NULL) {
    printf("File could not be opened\n");
    DestroyLinkedList(movie_list, NULL);
    return NULL; 
  } else {
    int max_row_length = 1000; 
    char row[max_row_length]; 
    
    while (!feof(cfPtr)) {
      fgets(row, max_row_length, cfPtr);
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

