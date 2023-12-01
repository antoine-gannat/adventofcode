#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define MAX_VALVES 60
#define MAX_NEIGHBORS 10
#define MAX_PATH_LENGTH 200
#define MAX_TIME 30

typedef struct s_valve
{
  char name[3];
  int flowRate;
  int neighborsCount;
  void *neighbors[MAX_NEIGHBORS];
  char serializedNeighbors[MAX_NEIGHBORS][3];
} t_valve;

int valveCount = 0;
t_valve valves[MAX_VALVES];

static void displayValve(t_valve v)
{
  printf("Valve %s has flow rate=%d and neighbors: ", v.name, v.flowRate);
  for (int i = 0; i < v.neighborsCount; i++)
  {
    printf("%s ", ((t_valve *)v.neighbors[i])->name);
  }
  printf("\n");
}

static void displayPadding(int padding)
{
  for (int i = 0; i < padding; i++)
    printf("  ");
}

static t_valve valveFactory(const char *line)
{
  t_valve valve;

  sscanf(line, "Valve %c%c has flow rate=%d;", &valve.name[0], &valve.name[1], &valve.flowRate);
  valve.name[2] = '\0';
  valve.neighborsCount = 0;

  // now get the neighbors name
  for (int i = strlen(line) - 1; i >= 0 && !((line[i] == 's' || line[i] == 'e') && line[i + 1] == ' '); i--)
  {
    if (line[i] == ' ')
    {
      valve.serializedNeighbors[valve.neighborsCount][0] = line[i + 1];
      valve.serializedNeighbors[valve.neighborsCount][1] = line[i + 2];
      valve.serializedNeighbors[valve.neighborsCount][2] = '\0';
      valve.neighborsCount++;
    }
  }
  return valve;
}

static int isInPath(t_valve *v, t_valve **path)
{
  for (int i = 0; path[i] != NULL; i++)
  {
    if (path[i] == v)
      return TRUE;
  }
  return FALSE;
}

static int getCurrentFlowRate(t_valve **path, const int pathLength)
{
  int flowRate = 0;
  for (int i = 0; i < pathLength; i++)
  {
    flowRate += path[i]->flowRate;
  }
  return flowRate;
}

// find the path which is going to release the most pressure under 30min
static int getMaxFlowRateFromValve(t_valve **path, const int pathLength, int *time, int pressureReleased)
{
  // take current valve from last in path
  t_valve *currentValve = path[pathLength - 1];
  // displayPadding(pathLength);
  // printf("[In %s] [pressure=%d, neighbors=%d, pathLength=%d, time=%d]\n", currentValve->name, pressureReleased, currentValve->neighborsCount, pathLength, *time);
  // move to new valve (+1 minute)
  (*time)++;
  // since a minute has passed, increase total released pressure based on previously opened valves (without current one)
  pressureReleased += getCurrentFlowRate(path, pathLength - 1);
  // if time is over 30min, leave
  if (*time >= MAX_TIME)
    return pressureReleased;
  // open current valve (+1 minute)
  (*time)++;
  // since another minute has passed, increase total released pressure based on previously opened valves
  pressureReleased += getCurrentFlowRate(path, pathLength);
  // if time is over 30min, leave
  if (*time >= MAX_TIME)
    return pressureReleased;

  // for each neighbor, if not in path, go to it
  int maxFlowRate = 0;
  int maxFlowRateTime = *time;
  for (int i = 0; i < currentValve->neighborsCount; i++)
  {
    t_valve *neighbor = currentValve->neighbors[i];
    // add neighbor to path
    path[pathLength] = neighbor;
    // get max flow rate from neighbor
    int tmpTime = *time;
    int flowRate = getMaxFlowRateFromValve(path, pathLength + 1, &tmpTime, pressureReleased);
    // if flow rate is greater than max flow rate, update max flow rate
    if (flowRate > maxFlowRate)
    {
      maxFlowRateTime = tmpTime;
      maxFlowRate = flowRate;
    }
    // remove neighbor from path
    path[pathLength] = NULL;
  }
  *time = maxFlowRateTime;
  // displayPadding(pathLength);
  // display full path
  printf("[In %s] [path=", currentValve->name);
  for (int i = 0; i < pathLength; i++)
  {
    printf("%s ", path[i]->name);
  }
  printf("] [pressure=%d,time=%d]\n", maxFlowRate + pressureReleased, *time);
  // printf("[In %s] [pressure=%d,time=%d]\n", currentValve->name, maxFlowRate + pressureReleased, *time);
  // then return neighbor with max flow rate
  return maxFlowRate + pressureReleased;
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
    valves[valveCount++] = valveFactory(line);
    free(line);
  }
  // deserialize the valves
  for (int i = 0; i < valveCount; i++)
  {
    for (int j = 0; j < valves[i].neighborsCount; j++)
    {
      for (int k = 0; k < valveCount; k++)
      {
        if (strcmp(valves[i].serializedNeighbors[j], valves[k].name) == 0)
        {
          valves[i].neighbors[j] = &valves[k];
          break;
        }
      }
    }
  }
  t_valve *initialPath[MAX_PATH_LENGTH] = {&valves[0], NULL};
  int time = 0;
  printf("Solution: %d\n", getMaxFlowRateFromValve(initialPath, 1, &time, 0));
  free(fileContent);
  return 0;
}