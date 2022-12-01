#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define ELVES_BUFFER_SIZE 1024

const size_t readSize = BUFFER_SIZE;

char *readFile(const char *path)
{
  size_t bytesRead = 0;
  size_t bufferSize = readSize;
  char readBuffer[BUFFER_SIZE + 1];
  int fd = open(path, O_RDONLY);

  if (fd == -1)
  {
    return NULL;
  }

  char *buffer = malloc(readSize * sizeof(char));
  if (buffer == NULL)
  {
    return NULL;
  }

  memset(buffer, 0, readSize);
  do
  {
    memset(readBuffer, 0, readSize + 1);

    bytesRead = read(fd, readBuffer, readSize);
    if (bytesRead == -1)
    {
      free(buffer);
      return NULL;
    }
    bufferSize += bytesRead;
    buffer = realloc(buffer, bufferSize);
    if (buffer == NULL)
    {
      return NULL;
    }
    strcat(buffer, readBuffer);
  } while (bytesRead == readSize);
  close(fd);
  return buffer;
}

int *parse(char *buffer)
{
  int elvesIndex = 0;
  int elfTotal = 0;
  size_t i = 0;
  size_t bufferIndex = 0;
  char numberBuffer[15];

  int *elves = malloc(ELVES_BUFFER_SIZE * sizeof(int));
  if (elves == NULL)
  {
    return NULL;
  }
  memset(numberBuffer, 0, 15);
  memset(elves, -1, ELVES_BUFFER_SIZE);

  while (buffer[i] != '\0')
  {
    // if end of elf
    if (i > 0 && buffer[i - 1] == '\n' && buffer[i] == '\n')
    {
      // save the total for this elf
      elves[elvesIndex] = elfTotal;
      elvesIndex++;
      // reset the total
      elfTotal = 0;
    }
    else if (buffer[i] == '\n')
    {
      int number = atoi(numberBuffer);
      elfTotal += number;
      memset(numberBuffer, 0, 15);
      bufferIndex = 0;
    }
    else
    {
      numberBuffer[bufferIndex] = buffer[i];
      bufferIndex++;
    }
    i++;
  }
  // add the last elf
  elves[elvesIndex] = elfTotal;
  // fill the rest with -1
  for (int i = elvesIndex + 1; i < ELVES_BUFFER_SIZE; i++)
  {
    elves[i] = -1;
  }
  return elves;
}

// Basic sorting algorithm to sort the elves
int *sortElves(int *arr)
{
  int i, j, temp;
  for (i = 0; i < ELVES_BUFFER_SIZE; i++)
  {
    for (j = i + 1; j < ELVES_BUFFER_SIZE; j++)
    {
      if (arr[i] < arr[j])
      {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
  return arr;
}

int main()
{
  char *fileContent = readFile("./input.txt");
  if (fileContent == NULL)
  {
    printf("Failed to read input file");
    return 1;
  }
  int i = 0;
  int *elves = sortElves(parse(fileContent));

  printf("%d", elves[0] + elves[1] + elves[2]);
  free(fileContent);
  free(elves);
  return 0;
}