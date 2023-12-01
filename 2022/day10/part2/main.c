#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 6

#define LIT_PIXEL '#'
#define DARK_PIXEL '.'

int X = 1;
unsigned int cycleCount = 0;

static void printCurrentPixel()
{
  const int pixelPos = (cycleCount - 1) % SCREEN_WIDTH;
  const int isLit = (pixelPos >= X - 1 && pixelPos <= X + 1);
  // print new lines
  if ((pixelPos % SCREEN_WIDTH) == 0)
  {
    printf("\n");
  }
  // if should be lit pixel
  if (isLit)
  {
    printf("%c", LIT_PIXEL);
  }
  else
  {
    printf("%c", DARK_PIXEL);
  }
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
      printCurrentPixel();
    }
    else if (strncmp(line, "addx", 4) == 0)
    {
      // 2 cycles
      int v;
      sscanf(line, "addx %d", &v);
      cycleCount++;
      printCurrentPixel();
      cycleCount++;
      printCurrentPixel();
      X += v;
    }
    free(line);
  }
  free(fileContent);
  return 0;
}