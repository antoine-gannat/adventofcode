#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

int main()
{
  char *fileContent = readFile("input.txt");
  if (fileContent == NULL)
  {
    printf("Error reading file");
    return 1;
  }

  char *line;
  while ((line = readNextLine(fileContent)) != NULL)
  {
    free(line);
  }
  printf("Solution: \n");
  free(fileContent);
  return 0;
}