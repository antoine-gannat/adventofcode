#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define PACKET_TYPE_ARRAY 1
#define PACKET_TYPE_NUMBER 2
#define MAX_CHILDREN 300
#define MAX_PACKETS 500

#define UNKNOWN -1

typedef struct s_packet
{
  unsigned short type;
  int value;
  void *children[MAX_CHILDREN];
  void *parent;
  int isMagicPacket;
} t_packet;

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

static t_packet *packetFactory(unsigned short type)
{
  t_packet *p = malloc(sizeof(t_packet));
  p->type = type;
  p->value = 0;
  p->isMagicPacket = FALSE;
  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    p->children[i] = NULL;
  }
  p->parent = NULL;
  return p;
}

static t_packet *magicPacketFactory(int value)
{
  t_packet *parent = packetFactory(PACKET_TYPE_ARRAY);
  t_packet *child = packetFactory(PACKET_TYPE_ARRAY);
  addChildren(parent, child);

  t_packet *magic = malloc(sizeof(t_packet));
  magic->type = PACKET_TYPE_NUMBER;
  magic->value = value;
  magic->isMagicPacket = TRUE;
  addChildren(child, magic);
  return parent;
}

static int isMagic(t_packet *p)
{
  if (p->isMagicPacket == TRUE)
    return TRUE;
  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    if (p->children[i] != NULL)
    {
      if (isMagic(p->children[i]) == TRUE)
        return TRUE;
    }
  }
  return FALSE;
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

static void sortPackets(t_packet **packets)
{
  // do a bubble sort for packets
  for (int i = 0; i < MAX_PACKETS; i++)
  {
    for (int j = 0; j < MAX_PACKETS - 1; j++)
    {
      if (packets[j] == NULL)
        break;
      if (packets[j + 1] == NULL)
        break;

      const int res = isInRightOrder(packets[j], packets[j + 1]);
      if (res == FALSE)
      {
        t_packet *tmp = packets[j];
        packets[j] = packets[j + 1];
        packets[j + 1] = tmp;
      }
    }
  }
}

int main()
{
  char *fileContent = readFile("input.txt");
  if (fileContent == NULL)
  {
    printf("Error reading file");
    return 1;
  }

  t_packet *packets[MAX_PACKETS];

  char *line;
  int lineCount = 0;
  while ((line = readNextLine(fileContent)) != NULL)
  {
    if (strlen(line) == 0)
    {
      free(line);
      continue;
    }
    int numberJustRead = FALSE;
    t_packet *currentPacket = packets[lineCount] = packetFactory(PACKET_TYPE_ARRAY);
    for (int i = 1; i < strlen(line) - 1; i++)
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
    free(line);
    lineCount++;
  }
  // finally add the magic packets
  packets[lineCount] = magicPacketFactory(2);
  packets[++lineCount] = magicPacketFactory(6);
  packets[++lineCount] = NULL;
  sortPackets(packets);

  int solution = 1;
  // free each packet and find the position of the magic ones
  for (int i = 0; i < lineCount; i++)
  {
    if (isMagic(packets[i]) == TRUE)
    {
      printf("Magic packet found at position %d\n", i + 1);
      solution *= i + 1;
    }
    freePacket(packets[i]);
  }
  printf("Solution %d\n", solution);
  free(fileContent);
  return 0;
}