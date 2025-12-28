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
  char *addstring = NULL;
  bool newfile = false;
  bool list = false;
  int c;
  int dbfd = -1;
  struct dbheader_t *header = NULL;
  struct employee_t *employees = NULL;

  while ((c = getopt(argc, argv, "nf:a:l")) != -1) {
    switch (c) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filepath = optarg;
      break;
    case 'a':
      addstring = optarg;
      break;
    case 'l':
      list = true;
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
    } else {
      printf("Created new database file at %s\n", filepath);
    }
    if (create_db_header(dbfd, &header) == STATUS_ERROR) {
      printf("Unable to create database file header.\n");
      close(dbfd);
      return -1;
    } else {
      printf("Created new database file header.\n");
    }
  } else {
    dbfd = open_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to open database file.\n");
      return -1;
    } /* else {
      printf("Opened database file at %s\n", filepath);
    } */
    if (validate_db_header(dbfd, &header) == STATUS_ERROR) {
      printf("Database file header is invalid.\n");
      return -1;
    } /* else {
      printf("Database file header is valid.\n");
    } */
  }

  if (read_employees(dbfd, header, &employees) != STATUS_SUCCESS) {
    printf("Unable to read employees from database file.\n");
    return -1;
  }

  if (addstring != NULL) {
    if (add_employee(header, &employees, addstring) != STATUS_SUCCESS) {
      printf("Unable to add employee to database.\n");
      return STATUS_ERROR;
    }
  }

  if (list) {
    list_employees(header, employees);
  }

  output_file(dbfd, header, employees);

  return 0;
}

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <database file>\n", argv[0]);
  printf("\t-n\t\tCreate a new file\n");
  printf("\t-f <filepath> Specify the file path (required)\n");
  return;
}
