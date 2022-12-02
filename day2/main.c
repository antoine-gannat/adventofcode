#include <stdio.h>
#include "../utils/utils.h"

static int getChoicePoints(const char playerChoice)
{
  switch (playerChoice)
  {
  case 'X':
    return 1;
  case 'Y':
    return 2;
  case 'Z':
    return 3;
  default:
    return 0;
  }
}

static int isWinner(const char opponentChoice, const char playerChoice)
{
  // to compare the choices, we need to convert them to numbers
  if (opponentChoice + (int)('X' - 'A') == playerChoice)
  {
    return 0;
  }
  if (opponentChoice == 'A')
  {
    return playerChoice == 'Y' ? 1 : -1;
  }
  if (opponentChoice == 'B')
  {
    return playerChoice == 'Z' ? 1 : -1;
  }
  if (opponentChoice == 'C')
  {
    return playerChoice == 'X' ? 1 : -1;
  }
  return 0;
}

static int findSolution(const char *fileContent)
{
  int totalPoints = 0;
  int i = 0;

  while (fileContent[i] != END_OF_FILE)
  {
    const char opponentChoice = fileContent[i];
    const char playerChoice = fileContent[i + 2]; // +1 is space, +2 is player choice

    const outcome = isWinner(opponentChoice, playerChoice);
    switch (outcome)
    {
    case 1:
      // win
      totalPoints += 6;
      break;
    case -1:
      // lose, no points
      break;
    default:
      // tie
      totalPoints += 3;
      break;
    }
    // finally, add the points from the choice
    totalPoints += getChoicePoints(playerChoice);
    i += 4; // +1 is space, +2 is player choice, +3 is EOL
  }
  return totalPoints;
}

int main()
{
  char *fileContent = readFile("./input.txt");
  if (fileContent == NULL)
  {
    printf("Failed to read input file");
    return 1;
  }
  printf("Solution: %d", findSolution(fileContent));
  free(fileContent);
  return 0;
}