#include <stdio.h>
#include "../utils/utils.h"

#define RAW_NUMBER_BUFFER_SIZE 10
#define ELVES_BUFFER_SIZE 3
#define EOL '\n'
#define END_OF_FILE '\0'

// update the top elves array
static void saveElf(int currentElfSum, int *topElves)
{
  // if higher than first, replace first and move the rest down
  if (currentElfSum > topElves[0])
  {
    topElves[2] = topElves[1];
    topElves[1] = topElves[0];
    topElves[0] = currentElfSum;
  }
  // if higher than second, replace second and move the rest down
  else if (currentElfSum > topElves[1])
  {
    topElves[2] = topElves[1];
    topElves[1] = currentElfSum;
  }
  // if higher than third, replace third
  else if (currentElfSum > topElves[2])
  {
    topElves[2] = currentElfSum;
  }
}

static const int findSolution(char *buffer)
{
  size_t i = 0;
  int elfTotal = 0;
  int topElves[ELVES_BUFFER_SIZE];
  char rawNumberBuffer[RAW_NUMBER_BUFFER_SIZE];

  // init buffers
  memset(rawNumberBuffer, 0, RAW_NUMBER_BUFFER_SIZE);
  memset(topElves, 0, ELVES_BUFFER_SIZE);

  // for each character in the file
  while (buffer[i] != END_OF_FILE)
  {
    // if end of elf (current char is EOL and so is the previous one)
    if (i > 0 && buffer[i - 1] == EOL && buffer[i] == EOL)
    {
      // save the total for this elf
      saveElf(elfTotal, topElves);
      // reset the total
      elfTotal = 0;
    }
    // if end of line, parse the raw number
    else if (buffer[i] == EOL)
    {
      const int number = atoi(rawNumberBuffer);
      // add the number to the total
      elfTotal += number;
      // reset the raw number buffer
      memset(rawNumberBuffer, 0, RAW_NUMBER_BUFFER_SIZE);
    }
    else
    {
      // concat the current char to the raw number buffer
      strncat(rawNumberBuffer, &buffer[i], 1);
    }
    i++;
  }
  // add the last elf
  saveElf(elfTotal, topElves);
  // return the sum of the top elves
  return topElves[0] + topElves[1] + topElves[2];
}

int main()
{
  char *fileContent = readFile("./input.txt");
  if (fileContent == NULL)
  {
    printf("Failed to read input file");
    return 1;
  }
  // find and print the solution
  printf("Solution: %d\n", findSolution(fileContent));
  // free the allocated memory
  free(fileContent);
  return 0;
}