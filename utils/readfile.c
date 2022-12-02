#include <stdlib.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

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
