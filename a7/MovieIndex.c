#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "MovieIndex.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"
#include "Movie.h"
#include "MovieSet.h"

void DestroyMovieSetWrapper(void *movie_set) {
  DestroyMovieSet((MovieSet)movie_set); 
}

void toLower(char *str, int len) {
  for (int i=0; i<len; i++) {
    str[i] = tolower(str[i]); 
  }
}

int AddMovieToIndex(Index index, Movie *movie, enum IndexField field) {

  // Put in the index
  HTKeyValue kvp;
  HTKeyValue old_kvp; 
  
  // If this key is already in the hashtable, get the MovieSet. 
  // Otherwise, create a MovieSet and put it in. 
  int result = LookupInHashtable(index,
                                 ComputeKey(movie, field),
                                 &kvp);
  
  if(result < 0) {
    char* doc_set_name;
    char year_str[10]; 
    switch(field){
      case Type:
        doc_set_name = movie->type;
        break;
      case Year:
        snprintf(year_str, 10, "%d", movie->year); 
        doc_set_name = year_str; 
        break;
      case Genre:
        // TODO: What to do if there are multiple genres? 
        doc_set_name = movie->genres; 
    }
    kvp.value = CreateMovieSet(doc_set_name); // Should be something like "1974", or "Documentary"
    kvp.key = ComputeKey(movie, field); 
    PutInHashtable(index, kvp, &old_kvp);
  } 
  
  AddMovieToSet((MovieSet)kvp.value, movie);
    
  return 0; 
}



uint64_t ComputeKey(Movie* movie, enum IndexField which_field) {
  switch(which_field) {
    case Year:
      return FNVHashInt64(movie->year); 
      break; 
    case Type:
      return FNVHash64((unsigned char*)movie->type, strlen(movie->type));
      break; 
    case Genre:
      // TODO: how to deal with multiple genres?? 
      return FNVHash64((unsigned char*)movie->genres, strlen(movie->genres));
  }
  return -1u; 
}


MovieSet GetMovieSet(Index index, const char *term) {
  HTKeyValue kvp;
  char lower[strlen(term)+1];
  strcpy(lower, term); 
  toLower(lower, strlen(lower)); 
  int result = LookupInHashtable(index, FNVHash64((unsigned char*)lower,
						  (unsigned int)strlen(lower)), &kvp);
  if (result < 0) {
    printf("term couldn't be found: %s \n", term); 
    return NULL; 
  }
  return (MovieSet)kvp.value; 
}


int DestroyIndex(Index index) {
  DestroyHashtable(index, &DestroyMovieSetWrapper); 
  return 0;
}

Index CreateIndex() {
  return CreateHashtable(128);
}
