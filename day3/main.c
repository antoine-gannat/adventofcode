#include <stdio.h>
#include "../utils/utils.h"

#define UPPERCASE_PRIORITY_MAGIC 38 // A(65) - 38 == 27
#define LOWERCASE_PRIORITY_MAGIC 96 // a(97) - 96 == 1

#define CURRENT_LINE_BUFFER_SIZE 100

static const int getPriority(const char item)
{
  // if uppercase letter
  if (item >= 'A' && item <= 'Z')
  {
    return item - UPPERCASE_PRIORITY_MAGIC;
  }
  // if lowercase letter
  if (item >= 'a' && item <= 'z')
  {
    return item - LOWERCASE_PRIORITY_MAGIC;
  }
  return -1;
}

static int checkForDuplicate(char *currentLine)
{
  int result = 0;
  // calculate the length of the line
  const int lineLength = strlen(currentLine);

  // check which char are present in both halves of the string
  for (int lineIndex = 0; lineIndex < lineLength / 2; lineIndex++)
  {
    int found = FALSE;
    for (int j = lineLength / 2; j < lineLength; j++)
    {
      // check if char is the same
      if (currentLine[lineIndex] == currentLine[j])
      {
        // replace current match with 0 to not re-analyze it later on.
        currentLine[j] = '+';
        // if match is already found, leave
        if (found)
        {
          continue;
        }
        // get its priority and add to total
        result += getPriority(currentLine[lineIndex]);
        found = TRUE; // save that we found the match
      }
    }
  }
  return result;
}

int main()
{
  const char *fileContent = readFile("input.txt");
  if (fileContent == NULL)
  {
    printf("Failed to read input file");
    return 1;
  }

  int totalLines = 0;
  int totalSum = 0;
  int i = 0;
  char currentLine[CURRENT_LINE_BUFFER_SIZE];
  memset(currentLine, 0, CURRENT_LINE_BUFFER_SIZE);
  // for each line
  for (int i = 0; fileContent[i] != END_OF_FILE; i++)
  {
    // if end of line
    if (fileContent[i] == EOL && strlen(currentLine) > 0)
    {
      totalSum += checkForDuplicate(currentLine);
      // reset current line
      memset(currentLine, 0, CURRENT_LINE_BUFFER_SIZE);
    }
    else if (fileContent[i] != EOL)
    {
      // add to current line
      currentLine[strlen(currentLine)] = fileContent[i];
    }
  }
  totalSum += checkForDuplicate(currentLine);

  printf("Total sum: %d", totalSum);
  free(fileContent);
  return 0;
}