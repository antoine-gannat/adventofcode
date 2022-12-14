#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define PACKET_TYPE_ARRAY 1
#define PACKET_TYPE_NUMBER 2
#define MAX_CHILDREN 300

#define UNKNOWN -1

typedef struct s_packet
{
  unsigned short type;
  int value;
  void *children[MAX_CHILDREN];
  void *parent;
} t_packet;

static void displayPacket(t_packet *p)
{
  if (p->type == PACKET_TYPE_NUMBER)
  {
    printf("%d", p->value);
  }
  else if (p->type == PACKET_TYPE_ARRAY)
  {
    printf("[");
    for (int i = 0; i < MAX_CHILDREN; i++)
    {
      if (p->children[i] != NULL)
      {
        if (i > 0)
        {
          printf(",");
        }
        displayPacket(p->children[i]);
      }
    }
    printf("]");
  }
}

static void freePacket(t_packet *p)
{
  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    if (p->children[i] != NULL)
    {
      freePacket(p->children[i]);
    }
  }
  free(p);
}

static t_packet *packetFactory(unsigned short type)
{
  t_packet *p = malloc(sizeof(t_packet));
  p->type = type;
  p->value = 0;
  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    p->children[i] = NULL;
  }
  p->parent = NULL;
  return p;
}

static void addChildren(t_packet *parent, t_packet *child)
{
  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    if (parent->children[i] == NULL)
    {
      parent->children[i] = child;
      child->parent = parent;
      parent->children[i + 1] = NULL;
      break;
    }
  }
}

static int isInRightOrder(t_packet *pair1, t_packet *pair2)
{
  // compare each children of the pairs
  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    t_packet *p1 = pair1->children[i];
    t_packet *p2 = pair2->children[i];

    // if we run out of items in pair 1
    if (p1 == NULL && p2 != NULL)
      return TRUE;
    // if we run out of items in pair 2
    if (p1 != NULL && p2 == NULL)
      return FALSE;
    // if we run out of items in both
    if (p1 == NULL && p2 == NULL)
      break;

    // if we are trying to compare a number and an array, change the type of the number to array
    if (p1->type == PACKET_TYPE_NUMBER && p2->type == PACKET_TYPE_ARRAY)
    {
      t_packet *newPacket = packetFactory(PACKET_TYPE_ARRAY);
      addChildren(newPacket, p1);
      p1 = pair1->children[i] = newPacket;
    }
    else if (p1->type == PACKET_TYPE_ARRAY && p2->type == PACKET_TYPE_NUMBER)
    {
      t_packet *newPacket = packetFactory(PACKET_TYPE_ARRAY);
      addChildren(newPacket, p2);
      p2 = pair2->children[i] = newPacket;
    }

    if (p1->type == PACKET_TYPE_ARRAY || p2->type == PACKET_TYPE_ARRAY)
    {
      const int res = isInRightOrder(p1, p2);
      if (res == FALSE)
        return FALSE;
      else if (res == TRUE)
        return TRUE;
      // continue if UNKNOWN
    }
    else
    {
      if (p1->value > p2->value)
        return FALSE;
      else if (p1->value < p2->value)
        return TRUE;
    }
  }
  return UNKNOWN;
}

int main()
{
  char *fileContent = readFile("input.txt");
  if (fileContent == NULL)
  {
    printf("Error reading file");
    return 1;
  }

  t_packet *pair1 = packetFactory(PACKET_TYPE_ARRAY);
  t_packet *pair2 = packetFactory(PACKET_TYPE_ARRAY);

  char *line;
  int pairToggle = 0;
  int pairCount = 0;
  int solution = 0;
  while ((line = readNextLine(fileContent)) != NULL)
  {
    int numberJustRead = FALSE;
    t_packet *currentPacket = pairToggle == 0 ? pair1 : pair2;
    pairToggle++;
    for (int i = 0; i < strlen(line); i++)
    {
      // create new packet with array type
      if (line[i] == '[')
      {
        t_packet *newPacket = packetFactory(PACKET_TYPE_ARRAY);
        addChildren(currentPacket, newPacket);
        currentPacket = newPacket;
        numberJustRead = FALSE;
      }
      else if (line[i] == ']')
      {
        currentPacket = currentPacket->parent;
        numberJustRead = FALSE;
      }
      // create new number packet
      else if (line[i] >= '0' && line[i] <= '9' && !numberJustRead)
      {
        t_packet *newPacket = packetFactory(PACKET_TYPE_NUMBER);
        numberJustRead = TRUE;
        sscanf(line + i, "%d", &newPacket->value);
        addChildren(currentPacket, newPacket);
      }
      else
        numberJustRead = FALSE;
    }
    // reset, analyze new pair
    if (strlen(line) == 0)
    {
      // printf("Comparing:\n");
      // displayPacket(pair1);
      // displayPacket(pair2);
      // printf("\n");
      pairCount++;
      if (isInRightOrder(pair1, pair2))
      {
        printf("Pair %d is in right order\n", pairCount);
        solution += pairCount;
      }
      else
        printf("Pair %d is NOT in right order\n", pairCount);
      freePacket(pair1);
      freePacket(pair2);
      pair1 = packetFactory(PACKET_TYPE_ARRAY);
      pair2 = packetFactory(PACKET_TYPE_ARRAY);
      pairToggle = 0;
    }
    free(line);
  }
  pairCount++;
  if (isInRightOrder(pair1, pair2))
  {
    printf("Pair %d is in right order\n", pairCount);
    solution += pairCount;
  }
  else
    printf("Pair %d is NOT in right order\n", pairCount);
  printf("Solution %d\n", solution);

  freePacket(pair1);
  freePacket(pair2);
  free(fileContent);
  return 0;
}