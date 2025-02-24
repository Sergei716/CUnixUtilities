#include "cat.h"

int main(int argc, char *argv[]) {
  Flags flags = cat_read_flags(argc, argv);
  if (flags.error_flags) {
    printf("usage: my_cat [-benstuv] [file ...]");
    exit(1);
  }
  while (optind < argc) {
    read_file(argv, &flags);
    ++optind;
  }
  return 0;
}

Flags cat_read_flags(int argc, char *argv[]) {
  struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                  {"number", 0, NULL, 'n'},
                                  {"squeeze-blank", 0, NULL, 's'},
                                  {NULL, 0, NULL, 0}};
  int current_flag = getopt_long(argc, argv, "+bevEnstT", long_options, NULL);
  Flags flags = {0};
  for (; current_flag != -1; current_flag = getopt_long(argc, argv, "+bevEnstT",
                                                        long_options, NULL)) {
    switch (current_flag) {
      case 'b':
        flags.number_nonblank = 1;
        break;
      case 'e':
        flags.mark_endl = 1;
        flags.print_non_printable = 1;
        break;
      case 'v':
        flags.print_non_printable = 1;
        break;
      case 'E':
        flags.mark_endl = 1;
        break;
      case 'n':
        flags.number_all = 1;
        break;
      case 's':
        flags.squeeze_blank = 1;
        break;
      case 't':
        flags.tab = 1;
        flags.print_non_printable = 1;
        break;
      case 'T':
        flags.tab = 1;
        break;
      case '?':
        flags.error_flags = 1;
        break;
    }
    if (flags.number_nonblank) flags.number_all = 0;
  }
  return flags;
}

void read_file(char *argv[], Flags *flags) {
  FILE *file;
  file = fopen(*(argv + optind), "r");
  if (file == NULL) {
    printf("%s: %s: No such file or directory\n", "s21_cat", *(argv + optind));
  } else {
    int ch;
    int string_count = 0;
    int empty_string = 0;
    int last_symb = '\n';
    while ((ch = getc(file)) != EOF) {
      if (flags->squeeze_blank && ch == '\n' && last_symb == '\n') {
        ++empty_string;
        if (empty_string > 1) continue;
      } else {
        empty_string = 0;
      }
      if (last_symb == '\n' &&
          ((flags->number_nonblank && ch != '\n') || flags->number_all))
        printf("%6d\t", ++string_count);
      if (flags->tab && ch == '\t') {
        printf("^");
        ch = 'I';
      }
      if (flags->mark_endl && ch == '\n') {
        printf("$");
      }
      if (flags->print_non_printable) {
        if (ch > 127 && ch < 160) printf("M-^");
        if ((ch < 32 && ch != '\n' && ch != '\t') || ch == 127) printf("^");
        if ((ch < 32 || (ch > 126 && ch < 160)) && ch != '\n' && ch != '\t')
          ch = ch > 126 ? ch - 128 + 64 : ch + 64;
      }
      putchar(ch);
      last_symb = ch;
    }
    fclose(file);
  }
}
