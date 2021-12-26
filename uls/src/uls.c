#include "../inc/uls.h"

int main(int argc, char *argv[])
{
    char *flags = "ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1";
    if (argc > 1)
    {
        int count = 1;
        int len = 0, len2 = 0;
        while (count != argc)
        {
            len += mx_strlen(argv[count]);
            len2 += mx_strlen(argv[count]);
            count++;
        }
        
        char *files = malloc(len + 1);
        char *dir = malloc(len2 + 1);
        count = 1;
        while (count != argc)
        {

            if (mx_strstr(argv[count], "-") == 0)
            {
                /*сравниваем введенные параметры с тем, что есть в текущей папке*/
                char *curr_dir = NULL;
                DIR *dp = NULL;
                struct dirent *dptr = NULL;
                struct winsize w;

                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                curr_dir = "./";

                dp = opendir((const char *)curr_dir);
                int f = 0;

                while ((dptr = readdir(dp)) != NULL)
                {
                    // Не считайте файлы, начинающиеся с "."
                    if (dptr->d_name[0] != '.')
                    {
                        if (mx_strcmp(argv[count], dptr->d_name) == 0 && mx_strlen(argv[count]) == mx_strlen(dptr->d_name))
                        {
                            f = 1;
                        }
                    }
                }
                closedir(dp);
                /*когда такой папки или файла не существует*/
                if (f == 0)
                {
                    write(2, "uls: ", mx_strlen("uls: "));
                    write(2, argv[count], mx_strlen(argv[count]));
                    write(2, ": No such file or directory\n", mx_strlen(": No such file or directory\n"));
                }
                else
                {
                    struct stat st;
                    stat(argv[count], &st);
                    /*заносим названия файлов в строку*/
                    if (!S_ISDIR(st.st_mode))
                    {
                        mx_strcat(files, argv[count]);
                        mx_strcat(files, " ");
                    }
                    /*заносим названия папок в строку*/
                    else if (S_ISDIR(st.st_mode))
                    {
                        mx_strcat(dir, argv[count]);
                        mx_strcat(dir, " ");
                    }
                }
            }
            else
            {
                /*переходим в функцию, когда флаг л*/
                if (mx_strcmp(argv[1], "-l") == 0)
                {
                    mx_flag_l();
                    return 0;
                }
                /*здесь если флага такого не существует*/
                char *s = malloc(argc * 2);

                int i = 0, j = 0, fl = 0, c = 1, cc = 0;
                while (c != argc)
                {
                    mx_strcpy(&s[cc], argv[c]);
                    cc += 2;
                    c++;
                }
                char *s2 = malloc(mx_strlen(argv[1]));
                while (s[i] != '\0')
                {
                    if (s[i] != '-')
                    {
                        s2[j] = s[i];
                        for (int i = 0; i <= 38; i++)
                        {
                            if (mx_strcmp(&s2[j], &flags[i]) == 0)
                            {
                                fl = 1;
                            }
                        }
                        if (fl == 0)
                        {
                            write(2, "uls: illegal option -- ", mx_strlen("uls: illegal option -- "));
                            write(2, &s2[j], mx_strlen(&s2[j]));
                            write(2, "\n", 1);
                            write(2, "usage: uls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n", mx_strlen("usage: uls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n"));
                            return -1;
                        }
                    }
                    fl = 0;
                    j++;
                    i++;
                }
            }
            count++;
            if (count == argc)
            {
                /////////////////////////////////////////////////////////////////// здесь вывод всех файлов в текущей папке
                int i = 0;
                while (files[i] != '\0')
                {
                    while (files[i] != ' ')
                    {
                        mx_printchar(files[i]);
                        i++;
                    }
                    i++;
                    if (files[i] != '\0')
                        write(1, " ", 1);
                }
                mx_printchar('\n');
                //////////////////////////////////////////////////////////////////// здесь вывод содержимого указанных папок
                i = 0;
                char *dir_name = malloc(len2);
                int k = 0;
                mx_printchar('\n');
                while (dir[i] != '\0')
                {
                    while (dir[i] != ' ')
                    {
                        dir_name[k] = dir[i];
                        mx_printchar(dir[i]);
                        i++;
                        k++;
                    }
                    write(1, ":\n", 2);

                    char *curr_dir = NULL;
                    DIR *dp = NULL;
                    struct dirent *dptr = NULL;
                    struct winsize w;

                    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                    curr_dir = dir_name;

                    // Переменная для хранения количества файлов внутри каталога
                    dp = opendir((const char *)curr_dir);
                    int buf = 0;
                    while ((dptr = readdir(dp)) != NULL)
                    {
                        char *str_dptr = malloc(mx_strlen(dptr->d_name) + 1);
                        mx_strcpy(str_dptr, dptr->d_name);

                        /*проверка на пустоту папок*/
                        
                        buf = mx_empty_dir(str_dptr);
                        // Не считайте файлы, начинающиеся с "."
                        if (dptr->d_name[0] != '.')
                        {
                            mx_printstr(dptr->d_name);
                            mx_printchar('\n');
                        }
                    }
                    if (buf != 0)
                    {
                        mx_printchar('\n');
                        mx_printstr("# empty directory");
                        mx_printchar('\n');
                    }
                    mx_printchar('\n');
                    k = 0;
                    closedir(dp);
                    i++;
                    mx_memset(dir_name, 0, mx_strlen(dir_name));
                }
                free(files);
                free(dir);
                return 0;
            }
        }
    }
    ////////////////////////////////////////////// если просто "./uls "
    else if (argc == 1)
    {
        mx_no_flags();
    }
}
