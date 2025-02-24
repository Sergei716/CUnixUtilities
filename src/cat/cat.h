#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int error_flags;
  int number_nonblank;
  int mark_endl;
  int number_all;
  int squeeze_blank;
  int tab;
  int print_non_printable;
} Flags;

Flags cat_read_flags(int, char **);
void read_file(char **, Flags *);

#endif  // CAT_H
