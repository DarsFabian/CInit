#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_SIZE 1024

typedef struct {
  char        cwd[MAX_SIZE];
  char        sfldr[MAX_SIZE];
  char        bfldr[MAX_SIZE];
  char        ifldr[MAX_SIZE];
  __int8_t    v;
  __int8_t    f;
} opts;

void concat_string(char *dest, char const *a, char const *b) { sprintf(dest, "%s%s%s", a, "/\0", b); }

void parse_opts(int *argc, char **argv, opts *opts) {
  /* GET CURRENT WORKING DIRECTORY */
  if(getcwd(opts->cwd, sizeof(opts->cwd)) == NULL) exit(EXIT_FAILURE);
  /* PARSE OPTS */
  int c;
  opts->v = 0b1;
  strcpy(opts->sfldr, "src");
  strcpy(opts->ifldr, "src/include");
  strcpy(opts->bfldr, "build");
  while((c = getopt(*argc, argv, "s:b:i:v")) != -1)
    switch(c) {
      case 's':
        strcpy(opts->sfldr, optarg);
        opts->f = opts->f | 0b00000001;
        break;
      case 'b':
        strcpy(opts->bfldr, optarg);
        opts->f = opts->f | 0b00000010;
        break;
      case 'i':
        strcpy(opts->ifldr, optarg);
        opts->f = opts->f | 0b00000100;
        break;
      case 'v':
         opts->v = 0b1;
         opts->f = opts->f | 0b00001000;
         break;
    };
}

void create_folders(opts *opts) {
  /* CREATE TEMPORARY STRING FOR MKDIR PATH */
  char tmp[MAX_SIZE * 2 + 1];

  /* CREATE SOURCE FOLDER */
  concat_string(tmp, opts->cwd, opts->sfldr);
  if(mkdir(tmp, 0777) != 0)
    printf("Couldn't create %s folder... Maybe it already exists?", opts->sfldr);
  else {
    printf("Creating %s directory...\n", opts->sfldr);
  }
  
  /* CREATE BUILD FOLDER */
  concat_string(tmp, opts->cwd, opts->bfldr);
  if(mkdir(tmp, 0777) != 0)
    printf("Couldn't create %s folder... Maybe it already exists?", opts->bfldr);
  else {
    printf("Creating %s directory...\n", opts->bfldr);
  }

  /* CREATE INCLUDE FOLDER */
  concat_string(tmp, opts->cwd, opts->ifldr);
  if(mkdir(tmp, 0777) != 0)
    printf("Couldn't create %s folder... Maybe it already exists?", opts->ifldr);
  else {
    printf("Creating %s directory...\n", opts->ifldr);
  }
}

void create_files(opts *opts) {
  /* CREATE .gitignore */
  FILE *f;
  char tmp[MAX_SIZE * 2];
  concat_string(tmp, opts->cwd, ".gitignore");
  f = fopen(tmp, "w+");
  if(f == NULL) exit(EXIT_FAILURE);
  sprintf(tmp, "%s%s", opts->bfldr, "/\0");
  fputs(tmp, f);
  if(fclose(f) == EOF) abort();
}

int main(int argc, char **argv) {
  /*
  GET PROGRAM OPTS
  */
  opts pargs;
  parse_opts(&argc, argv, &pargs);
  create_folders(&pargs);
  create_files(&pargs);

  return EXIT_SUCCESS;
}