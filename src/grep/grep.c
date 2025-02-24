#include "grep.h"

int main(int argc, char *argv[]) {
  char **array_pattern = malloc(BUFFER_SIZE * BUFFER_SIZE * sizeof(char) +
                                BUFFER_SIZE * sizeof(char *));
  char *ptr = (char *)(array_pattern + BUFFER_SIZE);
  for (int i = 0; i < BUFFER_SIZE; ++i)
    array_pattern[i] = ptr + BUFFER_SIZE * i;
  Flags flags = {0};
  Values value = {0};
  int value_optind = 0;
  int file_path = 0;
  value_optind = grep_parser(argc, argv, &flags, &value, array_pattern);
  if (argc >= 3 && flags.error != 1) {
    file_path = find_pattern(value_optind, argv, array_pattern, value);
    while (file_path < argc) {
      if (argv[file_path + 1] != NULL) value.count_files += 1;
      value.path = argv[file_path];
      grep(flags, value, array_pattern);

      ++file_path;
    }
  }
  free(array_pattern);
  return 0;
}

int grep_parser(int argc, char *argv[], Flags *flags, Values *value,
                char **array_pattern) {
  int currentFlags = getopt_long(argc, argv, "e:ivclnhsf:o", 0, NULL);
  for (; currentFlags != -1;
       currentFlags = getopt_long(argc, argv, "e:ivclnhsf:o", 0, NULL)) {
    switch (currentFlags) {
      case 'e':
        flags->e = 1;
        strcpy(array_pattern[value->count_pattern], optarg);
        value->count_pattern += 1;
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        fun_f(optarg, array_pattern, value);
        flags->f = 1;
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        flags->error = 1;
    }
  }
  return optind;
}

void fun_f(char *path, char **array_pattern, Values *value) {
  FILE *file_f;
  file_f = fopen(path, "rb");
  int size_string = 0;
  if (file_f != NULL) {
    while (!feof(file_f)) {
      fgets(array_pattern[value->count_pattern], BUFFER_SIZE, file_f);
      size_string = strlen(array_pattern[value->count_pattern]);
      if (array_pattern[value->count_pattern][0] != '\n' &&
          array_pattern[value->count_pattern][size_string - 1] == '\n')
        array_pattern[value->count_pattern][size_string - 1] = '\0';

      value->count_pattern += 1;
    }
    fclose(file_f);
  } else
    printf("No such file or directory");
}

void grep(Flags flags, Values value, char **array_pattern) {
  FILE *file;
  int count_lines = 0, count_matched_lines = 0, tmp_flag = REG_EXTENDED,
      stop = 0, result_regex = 0;
  regex_t preg;
  if (flags.v == 1 || flags.c == 1 || flags.l == 1) flags.o = 0;
  file = fopen(value.path, "rb");
  if (file != NULL) {
    if (value.count_pattern == 0 && flags.e == 0 && flags.f == 0)
      value.count_pattern = 1;
    if (flags.i == 1) tmp_flag = REG_ICASE;
    char *tmp_string = (char *)malloc(BUFFER_SIZE * sizeof(char));
    while ((fgets(tmp_string, BUFFER_SIZE, file)) && (stop == 0)) {
      int find_line = 0;
      ++count_lines;
      int pattern_matched_lines = 0;
      if (strchr(tmp_string, '\n') == NULL) strcat(tmp_string, "\n");
      for (int i = 0; i < value.count_pattern; ++i) {
        int tmp = 0;
        regcomp(&preg, array_pattern[i], tmp_flag | REG_NEWLINE);
        result_regex = regexec(&preg, tmp_string, 0, NULL, 0);
        if (result_regex == 0 && flags.v == 0) tmp = 1;
        if (result_regex == REG_NOMATCH && flags.v == 1) {
          ++pattern_matched_lines;
          if (pattern_matched_lines == value.count_pattern) tmp = 1;
        }
        if (flags.l == 1 && tmp == 1 && flags.c == 0) {
          printf("%s\n", value.path);
          tmp = 0;
          stop = 1;
        }
        if (flags.c == 1 && tmp == 1) {
          ++count_matched_lines;
          tmp = 0;
        }
        if (find_line == 0 && tmp == 1) {
          fun_print(flags, value, count_lines, tmp_string);
          ++find_line;
        }
        if (flags.o == 1) fun_o(tmp_string, preg);
        regfree(&preg);
      }
    }
    if (flags.c == 1) fun_c_l(flags, value, count_matched_lines);
    free(tmp_string);
    fclose(file);
  } else if (flags.s == 0)
    fprintf(stderr, "No such file or directory: %s\n", value.path);
}

void fun_print(Flags flags, Values value, int count_lines, char *tmp_string) {
  if (value.count_files >= 1 && flags.h == 0) printf("%s:", value.path);
  if (flags.n == 1) printf("%d:", count_lines);
  if (flags.o == 0) printf("%s", tmp_string);
}

void fun_o(char *tmp_string, regex_t preg) {
  regmatch_t match[1];
  while (!regexec(&preg, tmp_string, 1, match, 0)) {
    for (int i = 0; i < match->rm_eo; ++i) {
      if (i >= match->rm_so) {
        printf("%c", tmp_string[i]);
      }
      tmp_string[i] = 127;
    }
    printf("\n");
  }
}

void fun_c_l(Flags flags, Values value, int count_lines) {
  if (value.count_files >= 1 && flags.h == 0) {
    printf("%s:", value.path);
  }
  if (flags.l == 0)
    printf("%d\n", count_lines);
  else if (flags.l == 1) {
    if (count_lines > 0) {
      printf("1\n%s\n", value.path);
    } else
      printf("0\n");
  }
}

int find_pattern(int count_parser, char *argv[], char **array_pattern,
                 Values value) {
  int file_count = 0;
  if (value.count_pattern == 0) {
    file_count = count_parser + 1;
    strcpy(array_pattern[0], argv[optind]);
  } else
    file_count = count_parser;

  return file_count;
}
