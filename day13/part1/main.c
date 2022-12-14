#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define PACKET_TYPE_ARRAY 1
#define PACKET_TYPE_NUMBER 2
#define MAX_CHILDREN 100

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
        // printf("new packet (array)\n");
        t_packet *newPacket = packetFactory(PACKET_TYPE_ARRAY);
        addChildren(currentPacket, newPacket);
        currentPacket = newPacket;
        numberJustRead = FALSE;
      }
      else if (line[i] == ']')
      {
        // printf("end packet (array)\n");
        currentPacket = currentPacket->parent;
        numberJustRead = FALSE;
      }
      // create new number packet
      else if (line[i] >= '0' && line[i] <= '9' && !numberJustRead)
      {
        // printf("new packet (number)\n");
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
      displayPacket(pair1->children[0]);
      printf("\n");
      displayPacket(pair2->children[0]);
      printf("\n\n");
      freePacket(pair1);
      freePacket(pair2);
      pair1 = packetFactory(PACKET_TYPE_ARRAY);
      pair2 = packetFactory(PACKET_TYPE_ARRAY);
      pairToggle = 0;
    }
    free(line);
  }
  displayPacket(pair1->children[0]);
  printf("\n");
  displayPacket(pair2->children[0]);
  printf("\n");

  freePacket(pair1);
  freePacket(pair2);
  free(fileContent);
  return 0;
}