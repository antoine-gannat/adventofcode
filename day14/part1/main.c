#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define LINE_BUFF_SIZE 20

#define EMPTY 0
#define ROCK 1
#define SAND 2

#define END_OF_MAP 3

#define MAP_WIDTH 200
#define MAP_HEIGHT 200
#define SAND_SOURCE_X 500

#define OFFSET 490

typedef struct s_position
{
  int x;
  int y;
} t_position;

int map[MAP_HEIGHT][MAP_WIDTH];

static void initMap()
{
  for (int i = 0; i < MAP_HEIGHT; i++)
  {
    for (int j = 0; j < MAP_WIDTH; j++)
    {
      map[i][j] = EMPTY;
    }
  }
}

static void displayMap()
{
  for (int i = 0; i < MAP_HEIGHT; i++)
  {
    for (int j = 0; j < MAP_WIDTH; j++)
    {
      if (map[i][j] == EMPTY)
        printf(".");
      else if (map[i][j] == ROCK)
        printf("#");
      else if (map[i][j] == SAND)
        printf("o");
    }
    printf("\n");
  }
}

static int moveSand(int x, int y)
{
  int cellContent = map[y][x];
  if (y > MAP_HEIGHT - 1)
    return END_OF_MAP;
  switch (cellContent)
  {
  case EMPTY:
    // if empty, continue to fall
    return moveSand(x, y + 1);
  case ROCK:
  case SAND:
    // if rock or sand, try to move diagonally, first left, then right
    if (map[y][x - 1] == EMPTY)
      return moveSand(x - 1, y + 1);
    else if (map[y][x + 1] == EMPTY)
      return moveSand(x + 1, y + 1);
    // if that didn't work, stay
    else
      map[y - 1][x] = SAND;
  }
  return TRUE;
}

// funny name
static int sendSand()
{
  int x = SAND_SOURCE_X - OFFSET;

  for (int y = 0; y < MAP_HEIGHT; y++)
  {
    int res = moveSand(x, y);
    if (res == TRUE)
      return FALSE;
    else if (res == END_OF_MAP)
      break;
  }
  printf("Falling too long\n");
  return TRUE;
}

int main()
{
  char *fileContent = readFile("input.txt");
  if (fileContent == NULL)
  {
    printf("Error reading file");
    return 1;
  }

  initMap();
  char *line;
  while ((line = readNextLine(fileContent)) != NULL)
  {
    char lineBuff[LINE_BUFF_SIZE];
    memset(lineBuff, 0, LINE_BUFF_SIZE * sizeof(char));
    t_position current = {-1, -1};
    for (int i = 0; i <= strlen(line); i++)
    {
      if (line[i] == ' ' || line[i] == '\0')
      {
        int x, y;
        sscanf(lineBuff, "%d,%d", &x, &y);
        x -= OFFSET;
        if (current.x != -1 && current.y != -1)
        {
          int yStart = current.y > y ? y : current.y;
          int yEnd = current.y > y ? current.y : y;
          for (int j = yStart; j <= yEnd; j++)
          {
            map[j][x] = ROCK;
          }
          int xStart = current.x > x ? x : current.x;
          int xEnd = current.x > x ? current.x : x;
          for (int j = xStart; j <= xEnd; j++)
          {
            map[y][j] = ROCK;
          }
        }
        current.x = x;
        current.y = y;

        if (line[i] == '\0')
          break;
        // reset lineBuff
        memset(lineBuff, 0, LINE_BUFF_SIZE * sizeof(char));
        i += 3; // length of ' -> '
      }
      else
        strncat(lineBuff, &line[i], 1);
    }
    free(line);
  }
  int i = 0;
  while (!sendSand())
  {
    printf("Iteration %d\n", i++);
  }
  printf("Stopped at iteration %d\n", i);
  free(fileContent);
  return 0;
}