#pragma once

#define EOL '\n'
#define END_OF_FILE '\0'

#define TRUE 1
#define FALSE 0

char *readFile(const char *path);
char *readNextLine(const char *fileContent);