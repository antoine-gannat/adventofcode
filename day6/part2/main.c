#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define PACKET_START_CHAR_COUNT 14
#define READ_OFFSET 13

static int isStartOfPacket(const char *partOfLine)
{
  // if all character in partOfLine are different
  // then it is the start of a packet
  for (int i = 0; i < PACKET_START_CHAR_COUNT; i++)
  {
    for (int j = i + 1; j < PACKET_START_CHAR_COUNT; j++)
    {
      if (partOfLine[i] == partOfLine[j])
      {
        return FALSE;
      }
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

  for (int i = READ_OFFSET; i < strlen(fileContent); i++)
  {
    char buff[PACKET_START_CHAR_COUNT + 1];
    memcpy(buff, fileContent + i - READ_OFFSET, PACKET_START_CHAR_COUNT);
    buff[PACKET_START_CHAR_COUNT] = '\0';

    if (isStartOfPacket(buff) == TRUE)
    {
      printf("Solution: %d\n", i + 1);
      break;
    }
  }

  free(fileContent);
  return 0;
}