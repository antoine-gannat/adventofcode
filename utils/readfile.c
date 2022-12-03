#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 5

const size_t readSize = BUFFER_SIZE - 1;

char *readFile(const char *path)
{
  size_t bytesRead = 0;
  size_t bufferSize = 0;
  char readBuffer[BUFFER_SIZE];
  const int fd = open(path, O_RDONLY);

  if (fd == -1)
  {
    return NULL;
  }

  char *buffer = malloc(readSize * sizeof(char));
  memset(buffer, 0, readSize * sizeof(char));
  while ((bytesRead = read(fd, readBuffer, readSize)) > 0)
  {
    buffer = realloc(buffer, (bufferSize + bytesRead) * sizeof(char));
    strncpy(buffer + bufferSize, readBuffer, bytesRead);
    bufferSize += bytesRead;
  }
  buffer = realloc(buffer, (bufferSize) * sizeof(char));
  buffer[bufferSize] = '\0';

  if (bytesRead == -1)
  {
    printf("Error reading file.");
    return NULL;
  }
  close(fd);
  return buffer;
}
