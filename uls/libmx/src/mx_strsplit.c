#include "../inc/libmx.h"

char **mx_strsplit(char const *s, char c) {
	if (s == NULL) return NULL;
    while (*s == c) s++;
    int count = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] != c 
            && (s[i + 1] == c 
            || s[i + 1] == '\0'))
        {
            count++;
        }
    }
    char **s_arr = malloc((count + 1) * sizeof(char **));
    s_arr[count] = NULL;
    int cur_w_len = 0;
    int j = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] != c)
        {
            cur_w_len = mx_get_char_index(&s[i], c);
            if (cur_w_len == -1) cur_w_len = mx_strlen(&s[i]);
            s_arr[j] = mx_strndup(&s[i], cur_w_len);
            j++;
            i += cur_w_len;
            if (s[i] == '\0') break;
        }
    }
    return s_arr;
}

