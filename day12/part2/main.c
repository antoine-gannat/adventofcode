#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define MAP_SIZE 200

#define UNVISITED -1 // very high magic number
#define PATH -2

typedef struct s_position
{
  int x;
  int y;
  int prevValue;
} t_position;

typedef struct s_size
{
  int w;
  int h;
} t_size;

t_position end;
t_size mapSize;

int map[MAP_SIZE][MAP_SIZE];

static int getCellValue(char c, t_position pos)
{
  if (c == 'S')
  {
    c = 'a';
  }
  else if (c == 'E')
  {
    end = pos;
    c = 'z';
  }
  return c - 'a';
}

static int isNotTooHigh(t_position current, t_position target)
{
  const int diff = map[target.y][target.x] - map[current.y][current.x];
  if (diff > 1)
  {
    return FALSE;
  }
  return TRUE;
}

static int findPath(t_position start)
{
  int nodesToVisitCount = 0;
  int pathMap[MAP_SIZE][MAP_SIZE];
  t_position nodesToVisit[MAP_SIZE * MAP_SIZE];
  t_position current;

  for (int i = 0; i < MAP_SIZE; i++)
  {
    for (int j = 0; j < MAP_SIZE; j++)
    {
      pathMap[i][j] = UNVISITED;
    }
  }

  start.prevValue = 0;

  nodesToVisit[nodesToVisitCount++] = start;
  int found = FALSE;
  while (nodesToVisitCount > 0)
  {
    current = nodesToVisit[0];
    if (current.x == end.x && current.y == end.y)
    {
      found = TRUE;
    }
    if (pathMap[current.y][current.x] == UNVISITED)
    {
      pathMap[current.y][current.x] = current.prevValue + 1;
      // add more nodes to visit
      // north
      t_position north = (t_position){current.x, current.y - 1, pathMap[current.y][current.x]};
      if (current.y > 0 && pathMap[north.y][north.x] == UNVISITED && isNotTooHigh(current, north))
      {
        nodesToVisit[nodesToVisitCount++] = north;
      }
      // south
      t_position south = (t_position){current.x, current.y + 1, pathMap[current.y][current.x]};
      if (current.y < mapSize.h - 1 && pathMap[south.y][south.x] == UNVISITED && isNotTooHigh(current, south))
      {
        nodesToVisit[nodesToVisitCount++] = south;
      }
      // west
      t_position west = (t_position){current.x - 1, current.y, pathMap[current.y][current.x]};
      if (current.x > 0 && pathMap[west.y][west.x] == UNVISITED && isNotTooHigh(current, west))
      {
        nodesToVisit[nodesToVisitCount++] = west;
      }
      // east
      t_position east = (t_position){current.x + 1, current.y, pathMap[current.y][current.x]};
      if (current.x < mapSize.w - 1 && pathMap[east.y][east.x] == UNVISITED && isNotTooHigh(current, east))
      {
        nodesToVisit[nodesToVisitCount++] = east;
      }
    }
    // shift nodes to visit
    for (int i = 0; i < nodesToVisitCount; i++)
    {
      nodesToVisit[i] = nodesToVisit[i + 1];
    }
    nodesToVisitCount--;
  }
  if (!found)
  {
    return 0;
  }

  // find the path starting from the end
  current = end;

  int distance = 0;
  while (!(current.x == start.x && current.y == start.y))
  {
    t_position prev = current;
    t_position south = (t_position){current.x, current.y + 1};
    t_position north = (t_position){current.x, current.y - 1};
    t_position west = (t_position){current.x - 1, current.y};
    t_position east = (t_position){current.x + 1, current.y};
    // north
    if (current.y > 0 && pathMap[north.y][north.x] == pathMap[current.y][current.x] - 1)
    {
      current = north;
    }
    // south
    else if (current.y < mapSize.h - 1 && pathMap[south.y][south.x] == pathMap[current.y][current.x] - 1)
    {
      current = south;
    }
    // west
    else if (current.x > 0 && pathMap[west.y][west.x] == pathMap[current.y][current.x] - 1)
    {
      current = west;
    }
    // east
    else if (current.x < mapSize.w - 1 && pathMap[east.y][east.x] == pathMap[current.y][current.x] - 1)
    {
      current = east;
    }
    // write path
    pathMap[prev.y][prev.x] = PATH;
    distance++;
  }
  return distance;
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
  int lineCount = 0;
  while ((line = readNextLine(fileContent)) != NULL)
  {
    for (int i = 0; i < strlen(line); i++)
    {
      map[lineCount][i] = getCellValue(line[i], (t_position){i, lineCount});
    }
    mapSize.w = strlen(line);
    lineCount++;
    free(line);
  }
  mapSize.h = lineCount;
  // find all 0 on the map
  int best = 0;
  for (int y = 0; y < mapSize.h; y++)
  {
    for (int x = 0; x < mapSize.w; x++)
    {
      if (map[y][x] == 0)
      {
        const int solution = findPath((t_position){x, y});
        if (solution > 0 && (solution < best || best == 0))
        {
          best = solution;
        }
      }
    }
  }
  printf("Best solution: %d", best);
  free(fileContent);
  return 0;
}