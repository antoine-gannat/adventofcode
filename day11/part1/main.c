#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define ROUND_COUNT 20

#define ITEM_LIST_SIZE 100
#define MONKEY_LIST_SIZE 10

#define OPERATION_VALUE_OLD -1 // if operation value is 'old'
// operation types
#define OPERATION_TYPE_ADD 1
#define OPERATION_TYPE_MULTIPLY 2

typedef struct s_monkey
{
  int id;
  unsigned int items[ITEM_LIST_SIZE];
  unsigned int testCondition;
  unsigned int testSuccessMonkey;
  unsigned int testFailureMonkey;
  // operation
  int operationValue;
  unsigned int operationType;

  unsigned int inspectionCount;
} t_monkey;

t_monkey monkeys[MONKEY_LIST_SIZE];

static void initMonkeyList()
{
  for (unsigned int i = 0; i < MONKEY_LIST_SIZE; i++)
  {
    t_monkey *monkey = &monkeys[i];
    monkey->id = -1;
    memset(monkey->items, 0, ITEM_LIST_SIZE);
    monkey->testCondition = 0;
    monkey->testSuccessMonkey = 0;
    monkey->testFailureMonkey = 0;
    monkey->operationValue = 0;
    monkey->operationType = 0;
    monkey->inspectionCount = 0;
  }
}

static void initMonkey(char *line, unsigned int monkeyIndex)
{
  t_monkey *monkey = &monkeys[monkeyIndex];
  // get the monkey ID
  sscanf(line, "Monkey %d", &monkey->id);
  // get the items (up to 8 items)
  sscanf(line, "  Starting items: %u, %u, %u, %u, %u, %u, %u, %u", &monkey->items[0], &monkey->items[1],
         &monkey->items[2], &monkey->items[3], &monkey->items[4], &monkey->items[5], &monkey->items[6], &monkey->items[7]);
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

static void throwItem(t_monkey *currentMonkey, unsigned int targetMonkeyId)
{
  // find the target monkey
  t_monkey *targetMonkey = &monkeys[targetMonkeyId];

  // find the first empty slot in target monkey inventory
  unsigned int targetMonkeyItemIndex = 0;
  while (targetMonkey->items[targetMonkeyItemIndex] != 0)
  {
    targetMonkeyItemIndex++;
  }
  // throw the item to target monkey
  targetMonkey->items[targetMonkeyItemIndex] = currentMonkey->items[0];
  // remove the item from current monkey inventory
  for (unsigned int i = 0; i < ITEM_LIST_SIZE - 1; i++)
  {
    currentMonkey->items[i] = currentMonkey->items[i + 1];
  }
  currentMonkey->items[ITEM_LIST_SIZE - 1] = 0;
}

static void evalItem(t_monkey *monkey)
{
  // increase inspection count
  monkey->inspectionCount++;
  unsigned int *item = &monkey->items[0];
  // apply operation
  unsigned int operationValue = monkey->operationValue == OPERATION_VALUE_OLD ? *item : monkey->operationValue;

  if (monkey->operationType == OPERATION_TYPE_ADD)
  {
    (*item) += operationValue;
  }
  else if (monkey->operationType == OPERATION_TYPE_MULTIPLY)
  {
    (*item) *= operationValue;
  }
  // divide by 3 because of boredom
  (*item) /= 3;
  // run test condition to check if number is divisible by test condition
  if (*item % monkey->testCondition == 0)
  {
    // if true, throw to test success monkey
    throwItem(monkey, monkey->testSuccessMonkey);
  }
  else
  {
    // if false, throw to test failure monkey
    throwItem(monkey, monkey->testFailureMonkey);
  }
}

static void runRound()
{
  for (unsigned int i = 0; i < MONKEY_LIST_SIZE && monkeys[i].id != -1; i++)
  {
    t_monkey *monkey = &monkeys[i];
    // go through each items in that monkey inventory
    // eventually, all items in that monkey will be sent to another monkey
    while (monkey->items[0] != 0)
    {
      evalItem(monkey);
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

  char *line;
  unsigned int monkeyIndex = 0;
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
  for (unsigned short i = 0; i < ROUND_COUNT; i++)
  {
    runRound();
  }
  // now find the 2 monkeys with most inspection
  unsigned int maxInspectionCount[2] = {0, 0};
  for (unsigned int i = 0; i < MONKEY_LIST_SIZE && monkeys[i].id != -1; i++)
  {
    t_monkey *monkey = &monkeys[i];
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
  printf("Solution: %d\n", maxInspectionCount[0] * maxInspectionCount[1]);
  free(fileContent);
  return 0;
}