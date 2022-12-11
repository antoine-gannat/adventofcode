#include <stdlib.h>

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
  char *result = malloc(2 * sizeof(char));
  result[0] = '1';
  result[1] = '\0';
  return result;
}

char *mod(const char *a, const char *b)
{
  char *result = malloc(2 * sizeof(char));
  result[0] = '1';
  result[1] = '\0';
  return result;
}