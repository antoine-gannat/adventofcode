#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../../utils/utils.h"

#define MAP_SIZE 200

typedef struct s_pos
{
  int x;
  int y;
} t_pos;

// put head and tail in the middle of the map
t_pos headPosition = {MAP_SIZE / 2, MAP_SIZE / 2};
t_pos tailPosition = {MAP_SIZE / 2, MAP_SIZE / 2};

t_pos visitedPositions[MAP_SIZE * MAP_SIZE];
unsigned int visitedPositionsCount = 0;

static void calculateTailPos()
{
  // calculate distance between points
  int xDistance = pow(headPosition.x - tailPosition.x, 2);
  int yDistance = pow(headPosition.y - tailPosition.y, 2);
  const int distance = sqrt(xDistance + yDistance);

  // no need to move the tail
  if (distance <= 1)
  {
    return;
  }
  // if they are on the same line or column
  if (headPosition.x == tailPosition.x || headPosition.y == tailPosition.y)
  {
    // if the head is on the right of the tail
    if (headPosition.x > tailPosition.x)
    {
      tailPosition.x++;
    }
    // if the head is on the left of the tail
    else if (headPosition.x < tailPosition.x)
    {
      tailPosition.x--;
    }
    // if the head is above the tail
    else if (headPosition.y > tailPosition.y)
    {
      tailPosition.y++;
    }
    // if the head is below the tail
    else if (headPosition.y < tailPosition.y)
    {
      tailPosition.y--;
    }
  }
  else
  {
    // move diagonally
    if (headPosition.x > tailPosition.x)
    {
      tailPosition.x++;
    }
    else if (headPosition.x < tailPosition.x)
    {
      tailPosition.x--;
    }

    if (headPosition.y > tailPosition.y)
    {
      tailPosition.y++;
    }
    else if (headPosition.y < tailPosition.y)
    {
      tailPosition.y--;
    }
  }
}

static void saveDiscoveredPosition()
{
  // check if the position is already in the list
  for (unsigned int i = 0; i < visitedPositionsCount; i++)
  {
    // leave if position has already been visited
    if (visitedPositions[i].x == tailPosition.x && visitedPositions[i].y == tailPosition.y)
    {
      return;
    }
  }
  // if not visited, add it
  visitedPositions[visitedPositionsCount].x = tailPosition.x;
  visitedPositions[visitedPositionsCount].y = tailPosition.y;
  visitedPositionsCount++;
}

static void moveHead(char direction, unsigned int distance)
{
  switch (direction)
  {
  // up
  case 'U':
    for (int i = 0; i < distance; i++)
    {
      headPosition.y--;
      calculateTailPos();
      saveDiscoveredPosition();
      // displayMap();
    }
    break;
  // down
  case 'D':
    for (int i = 0; i < distance; i++)
    {
      headPosition.y++;
      calculateTailPos();
      saveDiscoveredPosition();
    }
    break;
  // left
  case 'L':
    for (int i = 0; i < distance; i++)
    {
      headPosition.x--;
      calculateTailPos();
      saveDiscoveredPosition();
    }
    break;
  // right
  case 'R':
    for (int i = 0; i < distance; i++)
    {
      headPosition.x++;
      calculateTailPos();
      saveDiscoveredPosition();
    }
    break;
  }
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
  while ((line = readNextLine((const char *)fileContent)) != NULL)
  {
    char direction;
    unsigned int distance;

    sscanf(line, "%c %d", &direction, &distance);
    moveHead(direction, distance);
    free(line);
  }

  printf("Solution : %d\n", visitedPositionsCount);
  free(fileContent);
  return 0;
}