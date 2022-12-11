#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"
#include "header.h"

#define ROUND_COUNT 10000

#define ITEM_LIST_SIZE 200
#define MONKEY_LIST_SIZE 10

#define OPERATION_VALUE_OLD -1 // if operation value is 'old'
// operation types
#define OPERATION_TYPE_ADD 1
#define OPERATION_TYPE_MULTIPLY 2

typedef struct s_monkey
{
  int id;
  char *items[ITEM_LIST_SIZE];
  int itemCount;
  unsigned long testCondition;
  unsigned long testSuccessMonkey;
  unsigned long testFailureMonkey;
  // operation
  int operationValue;
  unsigned long operationType;

  unsigned long inspectionCount;
} t_monkey;

t_monkey monkeys[MONKEY_LIST_SIZE];

static void initMonkeyList()
{
  for (unsigned long i = 0; i < MONKEY_LIST_SIZE; i++)
  {
    t_monkey *monkey = &monkeys[i];
    monkey->id = -1;
    monkey->itemCount = 0;
    monkey->testCondition = 0;
    monkey->testSuccessMonkey = 0;
    monkey->testFailureMonkey = 0;
    monkey->operationValue = 0;
    monkey->operationType = 0;
    monkey->inspectionCount = 0;
  }
}

static void initMonkey(char *line, unsigned long monkeyIndex)
{
  t_monkey *monkey = &monkeys[monkeyIndex];
  int items[8] = {0};
  // get the monkey ID
  sscanf(line, "Monkey %d", &monkey->id);
  // get the items (up to 8 items)
  if (sscanf(line, "  Starting items: %d, %d, %d, %d, %d, %d, %d, %d", &items[0], &items[1],
             &items[2], &items[3], &items[4], &items[5], &items[6], &items[7]) > 0)
  {
    for (unsigned long i = 0; i < 8 && items[i] != 0; i++)
    {
      monkey->items[i] = malloc(10 * sizeof(char));
      monkey->itemCount++;
      itoa(items[i], monkey->items[i], 10);
    }
  }
  // get the operation
  char operationValue[10] = {0};
  char operationType;
  if (sscanf(line, "  Operation: new = old %c %s", &operationType, operationValue) == 2)
  {
    monkey->operationType = operationType == '+' ? OPERATION_TYPE_ADD : OPERATION_TYPE_MULTIPLY;
    if (strcmp(operationValue, "old") == 0)
    {
      monkey->operationValue = OPERATION_VALUE_OLD;
    }
    else
    {
      monkey->operationValue = atoi(operationValue);
    }
  }
  // get the test condition
  sscanf(line, "  Test: divisible by %u", &monkey->testCondition);
  // get the test success monkey
  sscanf(line, "    If true: throw to monkey %u", &monkey->testSuccessMonkey);
  // get the test failure monkey
  sscanf(line, "    If false: throw to monkey %u", &monkey->testFailureMonkey);
}

static void throwItem(t_monkey *currentMonkey, unsigned long targetMonkeyId)
{
  // find the target monkey
  t_monkey *targetMonkey = &monkeys[targetMonkeyId];

  // throw the item to target monkey
  targetMonkey->items[targetMonkey->itemCount] = strdup(currentMonkey->items[0]);
  targetMonkey->itemCount++;

  // remove the item from current monkey inventory
  free(currentMonkey->items[0]);
  for (unsigned long i = 0; i < currentMonkey->itemCount - 1; i++)
  {
    currentMonkey->items[i] = currentMonkey->items[i + 1];
  }
  currentMonkey->itemCount--;
  // printf("monkey %d has %d items:\n", currentMonkey->id, currentMonkey->itemCount);
  // for (unsigned long i = 0; i < currentMonkey->itemCount; i++)
  // {
  //   printf("%s  ", currentMonkey->items[i]);
  // }
  // printf("\n");
}

static void evalItem(t_monkey *monkey)
{
  // increase inspection count
  monkey->inspectionCount++;
  // apply operation
  char operationValue[100] = {0};

  monkey->operationValue == OPERATION_VALUE_OLD ? strcpy(operationValue, monkey->items[0]) : itoa(monkey->operationValue, operationValue, 10);

  if (monkey->operationType == OPERATION_TYPE_ADD)
  {
    char *newItem = add((const char *)monkey->items[0], (const char *)operationValue);
    free(monkey->items[0]);
    monkey->items[0] = newItem;
  }
  else if (monkey->operationType == OPERATION_TYPE_MULTIPLY)
  {
    char *newItem = multiply((const char *)monkey->items[0], (const char *)operationValue);
    free(monkey->items[0]);
    monkey->items[0] = newItem;
  }
  char *testResult = mod((const char *)monkey->items[0], (const char *)monkey->testCondition);
  // run test condition to check if number is divisible by test condition
  if (strcmp((const char *)testResult, "0") == 0)
  {
    // if true, throw to test success monkey
    throwItem(monkey, monkey->testSuccessMonkey);
  }
  else
  {
    // if false, throw to test failure monkey
    throwItem(monkey, monkey->testFailureMonkey);
  }
  free(testResult);
}

static void runRound()
{
  for (unsigned long i = 0; i < MONKEY_LIST_SIZE && monkeys[i].id != -1; i++)
  {
    t_monkey *monkey = &monkeys[i];
    // go through each items in that monkey inventory
    // eventually, all items in that monkey will be sent to another monkey
    while (monkey->itemCount != 0)
    {
      evalItem(monkey);
    }
  }
  // // display monkeys inventory
  // for (unsigned long i = 0; i < MONKEY_LIST_SIZE && monkeys[i].id != -1; i++)
  // {
  //   t_monkey *monkey = &monkeys[i];
  //   printf("Monkey %d: ", monkey->id);
  //   for (unsigned long j = 0; j < ITEM_LIST_SIZE && monkey->items[j] != 0; j++)
  //   {
  //     printf("%u ", monkey->items[j]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");
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
  unsigned long monkeyIndex = 0;
  initMonkeyList();
  // read the file to get the monkeys
  while ((line = readNextLine(fileContent)) != NULL)
  {
    // if empty line, new monkey
    if (strlen(line) == 0)
    {
      monkeyIndex++;
    }
    else
    {
      initMonkey(line, monkeyIndex);
    }
    free(line);
  }
  for (unsigned long i = 0; i < ROUND_COUNT; i++)
  {
    runRound();
    if (i % 100 == 0)
    {
      printf("Round %u\n", i);
      if (monkeys[0].itemCount != 0)
      {
        printf("%s\n", monkeys[0].items[0]);
      }
    }
  }
  // now find the 2 monkeys with most inspection
  unsigned long maxInspectionCount[2] = {0, 0};
  for (unsigned long i = 0; i < MONKEY_LIST_SIZE && monkeys[i].id != -1; i++)
  {
    t_monkey *monkey = &monkeys[i];
    printf("aMonkey %d: %u\n", monkey->id, monkey->inspectionCount);
    if (monkey->inspectionCount > maxInspectionCount[0])
    {
      maxInspectionCount[1] = maxInspectionCount[0];
      maxInspectionCount[0] = monkey->inspectionCount;
    }
    else if (monkey->inspectionCount > maxInspectionCount[1])
    {
      maxInspectionCount[1] = monkey->inspectionCount;
    }
  }
  printf("Solution: %u\n", maxInspectionCount[0] * maxInspectionCount[1]);
  free(fileContent);
  return 0;
}