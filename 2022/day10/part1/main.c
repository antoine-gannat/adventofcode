#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

int X = 1;
unsigned int cycleCount = 0;
unsigned int lastDisplay = 0;

unsigned int totalStrength = 0;

static void displayXForCycle()
{
  if (cycleCount == 20 || lastDisplay == 40)
  {
    lastDisplay = 0;
    // print
    printf("X=%d (cycle=%d)\n", X, cycleCount);
    // calc signal strength
    totalStrength += cycleCount * X;
  }
  lastDisplay++;
}

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
    if (strncmp(line, "noop", 4) == 0)
    {
      // one cycle
      cycleCount++;
      displayXForCycle();
    }
    else if (strncmp(line, "addx", 4) == 0)
    {
      // 2 cycles
      int v;
      sscanf(line, "addx %d", &v);
      cycleCount++;
      displayXForCycle();
      cycleCount++;
      displayXForCycle();
      X += v;
    }
    free(line);
  }
  printf("Total strength: %d\n", totalStrength);
  free(fileContent);
  return 0;
}