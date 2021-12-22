#include "../inc/libmx.h"

int mx_count_words(const char *str, char delimiter) {
  int words = 0;
  int i = 0;
  int is_word = 0;
  while (str[i] != '\0') {
    if (str[i] == delimiter && is_word == 1) {
      is_word = 0;
    } 
    else if (str[i] != delimiter && is_word == 0) {
      is_word = 1;
      words++;
    }
    i++;
  }
  return words;
}
