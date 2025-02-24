#ifndef S21_GREP_H
#define S21_GREP_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int error;
} Flags;

typedef struct {
  char *path;
  int count_files;
  int count_pattern;
} Values;

int grep_parser(int, char *[], Flags *, Values *, char **);  // flags
void grep(Flags, Values, char **);
void fun_print(Flags, Values, int, char *);
void fun_o(char *, regex_t);
void fun_c_l(Flags, Values, int);
int find_pattern(int, char *[], char **, Values);
void fun_f(char *, char **, Values *);

#endif