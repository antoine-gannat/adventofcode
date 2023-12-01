#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define TREE_COUNT 99

int trees[TREE_COUNT][TREE_COUNT];

static int countVisibleFromOutside()
{
  return TREE_COUNT * 2 + (TREE_COUNT - 2) * 2;
}

static int isTreeVisible(const int x, const int y)
{
  const int currentTreeHeight = trees[y][x];
  int found = FALSE;
  // check north
  for (int i = y - 1; i >= 0; i--)
  {
    // if at least one is higher, then the tree is hidden
    if (trees[i][x] >= currentTreeHeight)
    {
      found = TRUE;
      break;
    }
  }
  if (!found)
    return TRUE;
  found = FALSE;
  // check south
  for (int i = y + 1; i < TREE_COUNT; i++)
  {
    if (trees[i][x] >= currentTreeHeight)
    {
      found = TRUE;
      break;
    }
  }
  if (!found)
    return TRUE;
  found = FALSE;
  // check west
  for (int i = x - 1; i >= 0; i--)
  {
    if (trees[y][i] >= currentTreeHeight)
    {
      found = TRUE;
      break;
    }
  }
  if (!found)
    return TRUE;
  found = FALSE;
  // check east
  for (int i = x + 1; i < TREE_COUNT; i++)
  {
    if (trees[y][i] >= currentTreeHeight)
    {
      found = TRUE;
      break;
    }
  }
  if (!found)
    return TRUE;
  return FALSE;
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
  // now check each tree
  int visibleTreeCount = 0;
  for (int y = 1; y < TREE_COUNT - 1; y++) // don't check sides
  {
    for (int x = 1; x < TREE_COUNT - 1; x++) // don't check sides
    {
      if (isTreeVisible(x, y))
      {
        visibleTreeCount++;
      }
    }
  }
  printf("Visible trees: %d\n", visibleTreeCount + countVisibleFromOutside());
  free(fileContent);
  return 0;
}