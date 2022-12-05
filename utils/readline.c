#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define LINE_BUFFER_SIZE 200 // max length of a line

char *readNextLine(const char *fileContent)
{
  static int fileIndex = 0;
  // if reading is done, return null
  if (fileIndex >= strlen(fileContent) || fileContent[fileIndex] == END_OF_FILE)
  {
    return NULL;
  }

  char *line = malloc(LINE_BUFFER_SIZE * sizeof(char));

  memset(line, 0, LINE_BUFFER_SIZE);

  for (; fileContent[fileIndex] != END_OF_FILE; fileIndex++)
  {
    if (fileContent[fileIndex] == EOL)
    {
      fileIndex++;
      return line;
    }
    else
    {
      // add char to line
      strncat(line, &fileContent[fileIndex], 1);
    }
  }
  return line;
}