#include <stdio.h>
#include <string.h>
#include "../../utils/utils.h"

#define CURRENT_LINE_BUFFER_SIZE 100

static const int isOverlapping(const int a1, const int a2, const int b1, const int b2)
{
  if (a1 >= b1 && a1 <= b2)
    return TRUE;
  if (a2 >= b1 && a2 <= b2)
    return TRUE;
  if (b1 >= a1 && b1 <= a2)
    return TRUE;
  if (b2 >= a1 && b2 <= a2)
    return TRUE;
  return FALSE;
}

int main()
{
  char *fileContent = readFile("input.txt");
  if (fileContent == NULL)
  {
    printf("Error reading file");
    return 1;
  }

  int a1, a2, b1, b2;
  int totalOverlap = 0;
  char currentLine[CURRENT_LINE_BUFFER_SIZE];

  memset(currentLine, 0, CURRENT_LINE_BUFFER_SIZE);
  for (int i = 0;; i++)
  {
    if (fileContent[i] == EOL || fileContent[i] == END_OF_FILE)
    {
      // find numbers
      sscanf(currentLine, "%d-%d,%d-%d", &a1, &a2, &b1, &b2);
      if (isOverlapping(a1, a2, b1, b2))
      {
        totalOverlap++;
      }
      // reset line
      memset(currentLine, 0, CURRENT_LINE_BUFFER_SIZE);
    }
    else
    {
      // add char to line
      strncat(currentLine, &fileContent[i], 1);
    }
    // if end, break
    if (fileContent[i] == END_OF_FILE)
    {
      break;
    }
  }

  printf("Total overlap = %d\n", totalOverlap);
  free(fileContent);
  return 0;
}