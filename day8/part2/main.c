#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define TREE_COUNT 99

int trees[TREE_COUNT][TREE_COUNT];

static int calcScenicScore(const int n, const int s, const int e, const int w)
{
  return n * s * e * w;
}

static int getTreeScore(const int x, const int y)
{
  const int currentTreeHeight = trees[y][x];
  int n = 0, s = 0, e = 0, w = 0;
  // check north
  for (int i = y - 1; i >= 0; i--)
  {
    n++;
    // if at least one is higher, then the tree is hidden
    if (trees[i][x] >= currentTreeHeight)
      break;
  }
  // check south
  for (int i = y + 1; i < TREE_COUNT; i++)
  {
    s++;
    if (trees[i][x] >= currentTreeHeight)
      break;
  }
  // check west
  for (int i = x - 1; i >= 0; i--)
  {
    w++;
    if (trees[y][i] >= currentTreeHeight)
      break;
  }
  // check east
  for (int i = x + 1; i < TREE_COUNT; i++)
  {
    e++;
    if (trees[y][i] >= currentTreeHeight)
      break;
  }
  return calcScenicScore(n, s, e, w);
}

int main()
{
  char *fileContent = readFile("input.txt");
  if (fileContent == NULL)
  {
    printf("Error while reading file");
    return 1;
  }

  char *line;
  int lineCount = 0;
  while ((line = readNextLine((const char *)fileContent)) != NULL)
  {
    // add the new line of trees
    for (int i = 0; line[i] != '\0'; i++)
    {
      trees[lineCount][i] = (int)(line[i] - '0'); // convert char to int and store it
    }
    lineCount++;
    free(line);
  }
  int highestScore = -1;
  // now check each tree
  for (int y = 1; y < TREE_COUNT - 1; y++) // don't check sides
  {
    for (int x = 1; x < TREE_COUNT - 1; x++) // don't check sides
    {
      const int newScore = getTreeScore(x, y);
      if (newScore > highestScore)
      {
        highestScore = newScore;
      }
    }
  }
  printf("Highest score: %d\n", highestScore);
  free(fileContent);
  return 0;
}