#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../../utils/utils.h"

#define MAP_SIZE 200
#define TAIL_LENGTH 10

typedef struct s_pos
{
  int x;
  int y;
} t_pos;

t_pos rope[TAIL_LENGTH];

t_pos visitedPositions[MAP_SIZE * MAP_SIZE];
unsigned int visitedPositionsCount = 0;

static void initRope()
{
  for (int i = 0; i < TAIL_LENGTH; i++)
  {
    rope[i].x = MAP_SIZE / 2;
    rope[i].y = MAP_SIZE / 2;
  }
}

static void calculateTailPos(int tailIndex)
{
  t_pos elementAhead = rope[tailIndex - 1];
  t_pos currentElement = rope[tailIndex];
  // calculate distance between points
  int xDistance = pow(elementAhead.x - currentElement.x, 2);
  int yDistance = pow(elementAhead.y - currentElement.y, 2);
  const int distance = sqrt(xDistance + yDistance);

  // no need to move the tail
  if (distance <= 1)
  {
    return;
  }
  // if they are on the same line or column
  if (elementAhead.x == currentElement.x || elementAhead.y == currentElement.y)
  {
    // if the head is on the right of the tail
    if (elementAhead.x > currentElement.x)
    {
      currentElement.x++;
    }
    // if the head is on the left of the tail
    else if (elementAhead.x < currentElement.x)
    {
      currentElement.x--;
    }
    // if the head is above the tail
    else if (elementAhead.y > currentElement.y)
    {
      currentElement.y++;
    }
    // if the head is below the tail
    else if (elementAhead.y < currentElement.y)
    {
      currentElement.y--;
    }
  }
  else
  {
    // move diagonally
    if (elementAhead.x > currentElement.x)
    {
      currentElement.x++;
    }
    else if (elementAhead.x < currentElement.x)
    {
      currentElement.x--;
    }

    if (elementAhead.y > currentElement.y)
    {
      currentElement.y++;
    }
    else if (elementAhead.y < currentElement.y)
    {
      currentElement.y--;
    }
  }
  rope[tailIndex] = currentElement;
}

static void saveDiscoveredPosition()
{
  t_pos tailPosition = rope[TAIL_LENGTH - 1];
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

static void moveTail()
{
  for (int i = 1; i < TAIL_LENGTH; i++)
  {
    calculateTailPos(i);
  }
}

static void moveHead(char direction, unsigned int distance)
{
  t_pos *headPosition = &rope[0];
  switch (direction)
  {
  // up
  case 'U':
    for (int i = 0; i < distance; i++)
    {
      headPosition->y--;
      moveTail();
      saveDiscoveredPosition();
    }
    break;
  // down
  case 'D':
    for (int i = 0; i < distance; i++)
    {
      headPosition->y++;
      moveTail();
      saveDiscoveredPosition();
    }
    break;
  // left
  case 'L':
    for (int i = 0; i < distance; i++)
    {
      headPosition->x--;
      moveTail();
      saveDiscoveredPosition();
    }
    break;
  // right
  case 'R':
    for (int i = 0; i < distance; i++)
    {
      headPosition->x++;
      moveTail();
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
  initRope();
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