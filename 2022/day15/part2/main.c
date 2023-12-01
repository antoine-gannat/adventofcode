#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../utils/utils.h"

#define OCCUPIED -2

#define MAX_SENSORS 100
#define MAX_BEACONS MAX_SENSORS

#define BEACON_MIN_POS 0
#define BEACON_MAX_POS 4000000

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
      return OCCUPIED;
    if (distance(pos, sensors[i].pos) <= sensors[i].closestBeacon)
      return i;
  }
  return -1;
}

static int getNextColumn(int sensorIndex, t_position current)
{

  t_sensor *sensor = &sensors[sensorIndex];
  int verticalDistanceToBeacon = abs(sensor->pos.y - current.y);

  int rangeEndX = (sensor->pos.x + sensor->closestBeacon) - verticalDistanceToBeacon;

  return rangeEndX;
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
  int found = FALSE;
  // check line 10
  for (int y = BEACON_MIN_POS; y <= BEACON_MAX_POS && found == FALSE; y++)
  {
    if (y % (BEACON_MAX_POS / 100) == 0)
    {
      printf("Progress: %d%%\n", (y * 100) / BEACON_MAX_POS);
    }
    for (int x = BEACON_MIN_POS; x <= BEACON_MAX_POS; x++)
    {
      int sensorIndex;
      if ((sensorIndex = canContainBeacon((t_position){x, y})) == -1)
      {
        long long r = (long long)x * 4000000 + y;
        printf("Solution x=%d,y=%d. Freq=%llu\n", x, y, r);
        found = TRUE;
        break;
      }
      else if (sensorIndex >= 0)
      {
        x = getNextColumn(sensorIndex, (t_position){x, y});
      }
    }
  }
  if (!found)
    printf("No solution found.\n");
  free(fileContent);
  return 0;
}