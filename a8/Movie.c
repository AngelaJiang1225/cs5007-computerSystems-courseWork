/*
 *  Created by Adrienne Slaughter
 *  CS 5007 Spring 2019
 *  Northeastern University, Seattle
 *  Edit by Aiqi Jiang
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
#include <string.h>


#include "Movie.h"
int countchar(char *str, char c);

Movie* CreateMovie() {
  Movie *mov = (Movie*)malloc(sizeof(Movie));
  if (mov == NULL) {
    printf("Couldn't allocate more memory to create a Movie\n");
    return NULL;
  }
  // Populate/Initialize movie.
  mov -> id = NULL;
  mov -> type = NULL;
  mov -> title = NULL;
  mov -> isAdult = -1;
  mov -> year = -1;
  mov -> runtime = -1;
  for (int i = 0; i < NUM_GENRES; i++) {
    mov->genres[i] = (char *)malloc(sizeof(char) * 1000);
  }
  return mov;
}

void DestroyMovie(Movie* movie) {
  if (movie->id != NULL) free(movie->id);
  if (movie->type != NULL) free(movie->type);
  if (movie->title != NULL) free(movie->title);
  // Destroy properly
for (int i = 0; i < NUM_GENRES; i++) {
    if (movie->genres[i] != NULL) {
      free(movie->genres[i]);
  }
  }
  free(movie);
}

void DestroyMovieWrapper(void *movie) {
  DestroyMovie((Movie*)movie);
}

char* CheckAndAllocateString(char* token) {
  if (strcmp("-", token) == 0) {
    return NULL;
  } else {
    // TODO(adrienne): get rid of whitespace.
    char *out = (char *) malloc((strlen(token) + 1) * sizeof(char));
    strcpy(out, token);
    return out;
  }
}

int CheckInt(char* token) {
  if (strcmp("-", token) == 0) {
    return -1;
  } else {
    return atoi(token);
  }
}

Movie* CreateMovieFromRow(char *data_row) {
  Movie* mov = CreateMovie();
  if (mov == NULL) {
    printf("Couldn't create a Movie.\n");
    return NULL;
  }
  int num_fields = 9;

  char *token[num_fields];
  char *rest = data_row;

  for (int i = 0; i < num_fields; i++) {
    token[i] = strtok_r(rest, "|", &rest);
    if (token[i] == NULL) {
      fprintf(stderr, "Error reading row\n");
      return NULL;
    }
  }
  // assign movie fields
  mov->id = CheckAndAllocateString(token[0]);
  mov->type = CheckAndAllocateString(token[1]);
  mov->title = CheckAndAllocateString(token[2]);
  mov->isAdult = CheckInt(token[4]);
  mov->year = CheckInt(token[5]);
  mov->runtime = CheckInt(token[7]);
  // Change such that genres is an array (or linkedlist), not just a string.
  char *gen = CheckAndAllocateString(token[8]);
  if (gen != NULL && strstr(gen, ",") == NULL) {
    strcpy(mov->genres[0], gen);
    free(gen);
  } else if (gen) {
    int cnt = countchar(gen, ',');  // count the number of ',' in gen
    for (int i = 0; i < cnt + 1; i++) {
      if (!gen) {
free(gen);
break;
      }
      strcpy(mov->genres[i], strtok_r(gen, ",", &gen));
  }
    if (!gen) free(gen);
  }
  if (!gen) free(gen);
  return mov;
}

// helper function to count the number of character a in the string
int countchar(char *str, char c) {
    int cnt = 0;
    int i = 0;
    while (*(str+i) != '\0') {
        if (*(str+i) == c)
            cnt++;
        i++;
    }
    return cnt;
}
