#include <stdlib.h>
#include <math.h>

int toInt(char n)
{
  return n - '0';
}
char toChar(int n)
{
  return n + '0';
}

char *reverse(const char *str)
{
  int len = strlen(str);
  char *result = malloc((len + 1) * sizeof(char));
  memset(result, 0, len + 1);
  for (int i = 0; i < len; i++)
  {
    result[i] = str[len - i - 1];
  }
  return result;
}

// infinite addition of two numbers
char *add(const char *a, const char *b)
{
  int aLen = strlen(a);
  int bLen = strlen(b);
  int longer = aLen > bLen ? aLen : bLen;
  const int resultSize = longer + 2;

  char *result = malloc(resultSize * sizeof(char));
  memset(result, 0, resultSize);

  int rest = 0;
  char *reversedA = reverse(a);
  char *reversedB = reverse(b);
  for (unsigned int i = 0; i < longer || rest != 0; i++)
  {
    if (i >= longer && rest != 0)
    {
      result[i] = toChar(rest);
      break;
    }
    // adds up the numbers
    const int additionResult = toInt(i < aLen ? reversedA[i] : '0') + toInt(i < bLen ? reversedB[i] : '0') + rest;
    if (additionResult < 10)
    {
      result[i] = toChar(additionResult);
      rest = 0;
    }
    else
    {
      result[i] = toChar(additionResult % 10);
      rest = additionResult / 10;
    }
  }
  result[resultSize - 1] = '\0';
  free(reversedA);
  free(reversedB);
  const char *reversedResult = reverse(result);
  return reverse(result);
}

char *multiply(const char *a, const char *b)
{
  int aLen = strlen(a);
  int bLen = strlen(b);
  char *result = malloc((aLen + bLen + 1) * sizeof(char));
  memset(result, 0, aLen + bLen + 1);
  int ***lattice;
  lattice = malloc(aLen * sizeof(int **));
  for (int i = 0; i < aLen; i++)
  {
    lattice[i] = malloc(bLen * sizeof(int *));
    for (int j = 0; j < bLen; j++)
    {
      lattice[i][j] = malloc(2 * sizeof(int));
      const int multiplicationResult = toInt(a[i]) * toInt(b[j]);
      lattice[i][j][0] = multiplicationResult / 10;
      lattice[i][j][1] = multiplicationResult % 10;
    }
  }

  int diagonalSum = 0;
  int k = 0;
  double jStart = bLen + 0.5;
  for (int i = aLen - 1; i >= 0; i--)
  {
    int toggle = 0;
    double i2 = (int)floor(i);
    // early case for the last row of the lattice
    if (jStart > 1)
    {
      jStart -= 1;
      toggle = 1;
      i++;
      i2 += 0.5;
    }
    else
    {
      jStart = 0;
    }
    for (double j = jStart; j < bLen && (int)floor(i2) >= 0; j += 0.5)
    {
      int x = (int)floor(i2);
      int y = (int)floor(j);
      diagonalSum += lattice[x][y][toggle];
      toggle = toggle == 0 ? 1 : 0;
      i2 -= 0.5;
    }
    if (i == 0 && diagonalSum == 0)
    {
      continue;
    }
    result[k] = toChar(diagonalSum % 10);
    k++;
    diagonalSum = diagonalSum / 10;
  }
  // free the allocated memory
  for (int i = 0; i < aLen; i++)
  {
    for (int j = 0; j < bLen; j++)
    {
      free(lattice[i][j]);
    }
    free(lattice[i]);
  }
  free(lattice);
  char *reversedResult = reverse(result);
  free(result);
  return reversedResult;
}

char *mod(const char *a, const char *b)
{
  char *result = malloc(2 * sizeof(char));
  result[0] = '1';
  result[1] = '\0';
  return result;
}