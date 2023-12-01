#include <stdio.h>
#include "../../utils/utils.h"

#define UPPERCASE_PRIORITY_MAGIC 38 // A(65) - 38 == 27
#define LOWERCASE_PRIORITY_MAGIC 96 // a(97) - 96 == 1

#define ITEM_LIST_SIZE 52 // 26 uppercase + 26 lowercase

typedef struct s_item
{
  unsigned int count;
  unsigned int lastFoundOnLine;
  char name;
} t_item;

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

static void initItemList(t_item *itemList)
{
  for (int i = 0; i < ITEM_LIST_SIZE; i++)
  {
    itemList[i] = (t_item){0, -1, 0};
  }
}

int main()
{
  const char *fileContent = readFile("input.txt");
  if (fileContent == NULL)
  {
    printf("Failed to read input file");
    return 1;
  }

  int totalSum = 0;
  int lineCount = 0;
  t_item items[ITEM_LIST_SIZE];
  initItemList(items);

  // for each line
  for (int i = 0; fileContent[i] != END_OF_FILE; i++)
  {
    if (fileContent[i] == '\n')
    {
      lineCount++;
      // every third line
      if (lineCount % 3 == 0)
      {
        // reset list
        initItemList(items);
      }
    }
    else
    {
      // add to list
      const int itemPriority = getPriority(fileContent[i]);
      const int itemIndex = itemPriority - 1;
      if (itemPriority != -1 && items[itemIndex].lastFoundOnLine != lineCount)
      {
        // use priority as index
        items[itemIndex].count++;
        items[itemIndex].name = fileContent[i];
        items[itemIndex].lastFoundOnLine = lineCount;
        // if we have an item that is present in all three lines
        if (items[itemIndex].count == 3)
        {
          totalSum += itemPriority;
        }
      }
    }
  }

  printf("Total sum: %d", totalSum);
  free(fileContent);
  return 0;
}