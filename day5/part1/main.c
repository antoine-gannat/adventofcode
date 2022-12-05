#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../utils/utils.h"

#define MAX_CRATE_PER_PILE 200
#define PILE_COUNT 9

typedef struct s_pile
{
  char crates[MAX_CRATE_PER_PILE];
} t_pile;

static void getCratesFromLine(t_pile *piles, const char *line)
{
  const int lineLength = strlen(line);
  const int pileOffset = 4;
  for (int i = 0; i < lineLength && line[i] != EOF; i += pileOffset /* jump to next pile */)
  {
    // check if we have a crate
    if (line[i] != '[')
    {
      // if not continue;
      continue;
    }
    // get crate name
    const char crateName = line[i + 1];
    const int pileIndex = i / pileOffset;
    // shift all crates up
    memcpy(piles[pileIndex].crates + 1, piles[pileIndex].crates, strlen(piles[pileIndex].crates));
    piles[pileIndex].crates[0] = crateName;
  }
}

static void moveCrates(t_pile *piles, const int from, const int to, const int count)
{
  int destinationCrateCount = strlen(piles[to].crates);
  int sourceCrateCount = strlen(piles[from].crates);
  for (int i = 0; i < count; i++)
  {
    // add crate to destination
    piles[to].crates[destinationCrateCount] = piles[from].crates[sourceCrateCount - 1];
    // remove crate from source
    piles[from].crates[sourceCrateCount - 1] = '\0';
    // update the counts
    destinationCrateCount++;
    sourceCrateCount--;
  }
}
static void displayCrates(const t_pile *piles)
{
  for (int i = 0; i < PILE_COUNT; i++)
  {
    printf("Pile %d: ", i + 1);
    for (int j = 0; j < MAX_CRATE_PER_PILE; j++)
    {
      if (piles[i].crates[j] != 0)
      {
        printf("%c ", piles[i].crates[j]);
      }
    }
    printf("\n");
  }
}

static void printTopCrates(const t_pile *piles)
{
  for (int i = 0; i < PILE_COUNT; i++)
  {
    printf("%c", piles[i].crates[strlen(piles[i].crates) - 1]);
  }
  printf("\n");
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
  int inCommandMode = FALSE;
  int count, from, to;
  t_pile piles[PILE_COUNT];
  // init piles
  for (int i = 0; i < PILE_COUNT; i++)
  {
    memset(piles[i].crates, 0, MAX_CRATE_PER_PILE);
  }
  // read file per line
  while ((line = readNextLine((const char *)fileContent)) != NULL)
  {
    // skip the number line
    if (line[0] == ' ' && line[1] == '1')
    {
      continue;
    }
    if (line[0] == '\0')
    {
      // switch to command lines
      inCommandMode = TRUE;
      continue;
    }
    if (!inCommandMode)
    {
      // get crates
      getCratesFromLine(piles, line);
    }
    else
    {
      sscanf(line, "move %d from %d to %d", &count, &from, &to);
      // move the crates (modify the indexes to start from index 0)
      moveCrates(piles, from - 1, to - 1, count);
    }
    free(line);
  }
  displayCrates(piles);

  printf("Solution: ");
  printTopCrates(piles);
  free(fileContent);
  return 0;
}