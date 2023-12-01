#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../utils/utils.h"

#define FILE_NAME_LENGTH 15
#define MAX_EL_PER_DIR 100

#define DISK_SIZE 70000000
#define UPDATE_SIZE 30000000

typedef struct s_file
{
  char name[FILE_NAME_LENGTH];
  unsigned int size;
} t_file;

typedef struct s_dir
{
  char name[FILE_NAME_LENGTH];
  t_file files[MAX_EL_PER_DIR];
  struct s_dir *parent;
  struct s_dir *children[MAX_EL_PER_DIR];
  unsigned short nbFiles;
  unsigned short nbChildren;
  unsigned int size;
} t_dir;

t_dir filesystem; // root

t_dir *currentDir = NULL;

unsigned int solution = 0;

static void findSolution(t_dir *dir, const unsigned int minSizeToFreeUp)
{
  if (dir->size > minSizeToFreeUp && (dir->size < solution || solution == 0))
  {
    solution = dir->size;
  }
  for (int i = 0; i < dir->nbChildren; i++)
  {
    findSolution(dir->children[i], minSizeToFreeUp);
  }
}

static void updateDirSize(t_dir *dir)
{
  dir->size = 0;
  // recursively find the size of children then update self
  for (int i = 0; i < dir->nbChildren; i++)
  {
    updateDirSize(dir->children[i]);
    dir->size += dir->children[i]->size;
  }
  // add the size of files
  for (int i = 0; i < dir->nbFiles; i++)
  {
    dir->size += dir->files[i].size;
  }
}

static void initDir(t_dir *dir, const char *name, t_dir *parent)
{
  // fill with 0 the name
  memset(dir->name, 0, FILE_NAME_LENGTH);
  // copy name
  strcpy(dir->name, name);
  // init files array
  memset(dir->files, 0, MAX_EL_PER_DIR * sizeof(t_file));
  // set parent
  dir->parent = parent;
  memset(dir->children, 0, MAX_EL_PER_DIR * sizeof(t_dir *));
  // set file/children counters
  dir->nbFiles = 0;
  dir->nbChildren = 0;
  dir->size = 0;
}

static void addNewDir(const char *name)
{
  t_dir *newDir = malloc(sizeof(t_dir));
  initDir(newDir, name, currentDir);
  currentDir->children[currentDir->nbChildren] = newDir;
  currentDir->nbChildren++;
}

static void addNewFile(t_file el)
{
  currentDir->files[currentDir->nbFiles] = el;
  currentDir->nbFiles++;
}

static void processCommandResponse(const char *rawOutput)
{
  t_file file = {{0}, 0};
  const int nbValAssigned = sscanf(rawOutput, "%d %s", &file.size, file.name);
  // if we could not assign 2 values, then it is not a file but a dir
  if (nbValAssigned < 2)
  {
    char name[FILE_NAME_LENGTH] = {0};
    sscanf(rawOutput, "dir %s", name);
    addNewDir(name);
  }
  else
  {
    // process file
    addNewFile(file);
  }
}

static void processCommand(const char *rawCommand)
{
  const char command[3] = {rawCommand[0], rawCommand[1], '\0'};
  const char *argument = strlen(rawCommand) <= 2 ? NULL : rawCommand + 3;

  // nothing to do for 'ls'
  if (strcmp(command, "ls") == 0)
  {
    return;
  }

  // process 'cd'
  if (strcmp(argument, "..") == 0) // go to parent
  {
    currentDir = currentDir->parent;
  }
  else if (strcmp(argument, "/") == 0) // go to root
  {
    currentDir = &filesystem;
  }
  else // otherwise, find where to go
  {
    // find the dir in the current dir
    int found = FALSE;
    for (int i = 0; i < currentDir->nbChildren; i++)
    {
      t_dir *dir = currentDir->children[i];
      if (strcmp(dir->name, argument) == 0)
      {
        currentDir = dir;
        found = TRUE;
        break;
      }
    }
    // to remove
    if (!found)
    {
      printf("ERROR: Dir not found [%s]\n", argument);
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
  initDir(&filesystem, "/", NULL);
  while ((line = readNextLine((const char *)fileContent)) != NULL)
  {
    // if command line
    if (line[0] == '$')
    {
      processCommand(line + 2 /* +2 to skip '$ ' */);
    }
    else
    {
      processCommandResponse(line);
    }
    free(line);
  }

  updateDirSize(&filesystem);
  findSolution(&filesystem, UPDATE_SIZE - (DISK_SIZE - filesystem.size));
  printf("solution: %u\n", solution);
  free(fileContent);
  return 0;
}