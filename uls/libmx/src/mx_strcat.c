#include "../inc/libmx.h"

char *mx_strcat(char *s1, const char *s2) {
  int src_len = mx_strlen(s2);
  int dst_len = mx_strlen(s1);

  for (int i = 0; i < src_len + 1; i++) {
    s1[dst_len + i] = s2[i];
  }
  return (char *)s1;
}
