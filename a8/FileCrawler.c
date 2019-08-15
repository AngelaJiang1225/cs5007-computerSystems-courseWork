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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include "FileCrawler.h"
#include "DocIdMap.h"
#include "LinkedList.h"

void CrawlFilesToMap(const char *dir, DocIdMap map) {
  struct stat s;

  struct dirent **namelist;
  int n;
  n = scandir(dir, &namelist, 0, alphasort);
  // TODO: use namelist to find all the files and put them in map.
  // NOTE: There may be nested folders.
  // Be sure to lookup how scandir works. Don't forget about memory use.
for (int i = 0; i < n;i++) {
    if (strcmp(namelist[i]->d_name, ".") != 0
&& strcmp(namelist[i]->d_name, "..") != 0) {
      char newdir[strlen(dir) + strlen(namelist[i]->d_name) + 2];
      strcpy(newdir, dir);
      if (newdir[strlen(newdir)-1] != '/') {
       strcat(newdir, "/");
    }
      strcat(newdir, namelist[i]->d_name);
      // if it is a directory
      if (0 == stat(newdir, &s) && S_ISDIR(s.st_mode)) {
        CrawlFilesToMap(newdir, map);
      } else {
        char *np = strdup(newdir);
  PutFileInMap(np, map);
      }
      memset(newdir, 0, sizeof(newdir));
    }
  }
// free the namelist
for (int i = 0; i < n; i++) {
  free(namelist[i]);
}
free(namelist);
}
