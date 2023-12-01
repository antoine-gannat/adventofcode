#include <stdio.h>
#include "../utils/utils.h"

#define ROCK 'A'
#define PAPER 'B'
#define SCISSORS 'C'

#define WIN 1
#define LOSE -1
#define DRAW 0

static int getChoicePoints(const char opponentChoice, const int outcome)
{
  switch (opponentChoice)
  {
  case ROCK:
    // rock to draw
    // paper to win,
    // scissors to lose,
    return outcome == DRAW ? 1 : outcome == WIN ? 2
                                                : 3;
  case PAPER:
    // rock to lose
    // paper to draw
    // scissors to win
    return outcome == DRAW ? 2 : outcome == WIN ? 3
                                                : 1;
  case SCISSORS:
    // rock to win
    // paper to lose
    // scissors to draw
    return outcome == DRAW ? 3 : outcome == WIN ? 1
                                                : 2;
  default:
    return 0;
  }
}

static int getOutcome(const char playerChoice)
{
  switch (playerChoice)
  {
  case 'X':
    return LOSE;
  case 'Y':
    return DRAW;
  case 'Z':
    return WIN;
  default:
    return 0;
  }
}

static int findSolution(const char *fileContent)
{
  int totalPoints = 0;
  int i = 0;

  while (fileContent[i] != END_OF_FILE)
  {
    const char opponentChoice = fileContent[i];
    const char playerChoice = fileContent[i + 2]; // +1 is space, +2 is player choice

    const outcome = getOutcome(playerChoice);
    switch (outcome)
    {
    case 1:
      // should win
      totalPoints += 6;
      break;
    case -1:
      // should lose
      break;
    default:
      // should tie
      totalPoints += 3;
      break;
    }
    // finally, add the points from the choice
    totalPoints += getChoicePoints(opponentChoice, outcome);
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