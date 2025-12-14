#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]);

int main(int argc, char *argv[]) {

  char *filepath = NULL;
  bool newfile = false;
  int c;
  int dbfd = -1;
  struct dbheader_t *header = NULL;

  while ((c = getopt(argc, argv, "nf:")) != -1) {
    switch (c) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filepath = optarg;
      break;
    case '?':
      printf("Unknown option: -%c\n", c);
      break;
    default:
      return -1;
    };
  };

  if (filepath == NULL) {
    printf("Filepath is a required argument. \nUse -f <filepath> to specify "
           "the file. \n");
    print_usage(argv);
    return -1;
  }

  if (newfile) {
    dbfd = create_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to create new database file.\n");
      return -1;
    }
    create_db_header(dbfd, &header);
  } else {
    dbfd = open_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to open database file.\n");
      return -1;
    }
    if (validate_db_header(dbfd, &header) == STATUS_ERROR) {
      printf("Database file header is invalid.\n");
      close(dbfd);
      return -1;
    }
  }

  // printf("Newfile: %d\n", newfile);
  // printf("Filepath: %s\n", filepath);
  output_file(dbfd, header);

  // free(header);
  // close(dbfd);

  return 0;
}

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <database file>\n", argv[0]);
  printf("\t-n\t\tCreate a new file\n");
  printf("\t-f <filepath> Specify the file path (required)\n");
  return;
}
