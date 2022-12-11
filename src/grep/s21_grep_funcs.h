#ifndef SRC_GREP_S21_GREP_FUNCS_H_
#define SRC_GREP_S21_GREP_FUNCS_H_

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
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
} dflag;

int get_flags(const char *short_options, int argc, char **argv, dflag *flag,
              char **patterns);
void files_controller(int optind, const int argc, char **argv, const dflag flag,
                      char **patterns);

#endif /* SRC_GREP_S21_GREP_FUNCS_H_*/
