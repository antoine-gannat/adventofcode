#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../utils/utils.h"

#define MAX_SENSORS 100
#define MAX_BEACONS MAX_SENSORS

typedef struct s_position
{
  int x;
  int y;
} t_position;

typedef struct s_sensor
{
  t_position pos;
  int closestBeacon;
} t_sensor;

typedef struct s_range
{
  int min;
  int max;
} t_range;

t_sensor sensors[MAX_SENSORS];
t_position beacons[MAX_BEACONS];

t_range mapRangeX = {-1, -1};
t_range mapRangeY = {-1, -1};

int sensorCount = 0;

static int distance(t_position a, t_position b)
{
  return abs(a.x - b.x) + abs(a.y - b.y);
}

static int canContainBeacon(t_position pos)
{
  for (int i = 0; i < sensorCount; i++)
  {
    if ((sensors[i].pos.x == pos.x && sensors[i].pos.y == pos.y) || (beacons[i].x == pos.x && beacons[i].y == pos.y))
      return TRUE;
    if (distance(pos, sensors[i].pos) <= sensors[i].closestBeacon)
    {
      return FALSE;
    }
  }
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

  char *line;
  while ((line = readNextLine(fileContent)) != NULL)
  {
    t_position *beacon = &beacons[sensorCount];
    t_sensor *sensor = &sensors[sensorCount];
    sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sensor->pos.x, &sensor->pos.y, &beacon->x, &beacon->y);
    sensor->closestBeacon = distance(sensor->pos, *beacon);

    if (sensor->pos.x - sensor->closestBeacon < mapRangeX.min || mapRangeX.min == -1)
      mapRangeX.min = sensor->pos.x - sensor->closestBeacon;
    if (sensor->pos.x + sensor->closestBeacon > mapRangeX.max || mapRangeX.max == -1)
      mapRangeX.max = sensor->pos.x + sensor->closestBeacon;
    if (sensor->pos.y - sensor->closestBeacon < mapRangeY.min || mapRangeY.min == -1)
      mapRangeY.min = sensor->pos.y - sensor->closestBeacon;
    if (sensor->pos.y + sensor->closestBeacon > mapRangeY.max || mapRangeY.max == -1)
      mapRangeY.max = sensor->pos.y + sensor->closestBeacon;

    if (beacon->x < mapRangeX.min)
      mapRangeX.min = beacon->x;
    if (beacon->x > mapRangeX.max)
      mapRangeX.max = beacon->x;
    if (beacon->y < mapRangeY.min)
      mapRangeY.min = beacon->y;
    if (beacon->y > mapRangeY.max)
      mapRangeY.max = beacon->y;

    free(line);
    sensorCount++;
  }
  // print map range
  printf("Map range: x(%d, %d), y(%d, %d)\n", mapRangeX.min, mapRangeX.max, mapRangeY.min, mapRangeY.max);
  int solution = 0;
  // check line 10
  int y = 2000000;
  for (int x = mapRangeX.min; x < mapRangeX.max; x++)
  {
    if (!canContainBeacon((t_position){x, y}))
      solution++;
  }
  printf("Solution %d\n", solution);
  free(fileContent);
  return 0;
}