#include "../inc/uls.h"

int mx_flag_l(void)
{
    char *curr_dir = NULL;
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    int count = 0;
    long *ptr = NULL;
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    curr_dir = "./";

    if (NULL == curr_dir)
    {
        write(2, "ERROR : Could not get the working directory\n", mx_strlen("ERROR : Could not get the working directory\n"));
        return -1;
    }

    int num_files = 0;
    dp = opendir((const char *)curr_dir);

    while ((dptr = readdir(dp)) != NULL)
    {
        if (dptr->d_name[0] != '.')
            num_files++;
    }
    closedir(dp);

    dp = NULL;
    dptr = NULL;

    if (!num_files)
    {
        return 0;
    }
    else
    {
        ptr = malloc(num_files * 8);
        if (NULL == ptr)
        {
            write(2, "Memory allocation failed\n", mx_strlen("Memory allocation failed\n"));
            return -1;
        }
        else
        {
            mx_memset(ptr, 0, num_files * 8);
        }
    }

    dp = opendir((const char *)curr_dir);
    if (dp == NULL)
    {
        write(2, "ERROR : Could not open the working directory\n", mx_strlen("ERROR : Could not open the working directory\n"));
        free(ptr);
        return -1;
    }

    int j = 0;
    for (count = 0; (dptr = readdir(dp)) != NULL; count++)
    {
        if (dptr->d_name[0] != '.')
        {
            ptr[j] = (long)dptr->d_name;
            j++;
        }
    }

    for (count = 0; count < num_files - 1; count++)
    {
        for (j = count + 1; j < (num_files); j++)
        {
            char *c = (char *)ptr[count];
            char *d = (char *)ptr[j];

            if (((*c >= 65) && (*d >= 65)) || ((*c <= 122) && (*d <= 122)))
            {
                int i = 0;
                if (*c == *d)
                {
                    while (*(c + i) == *(d + i))
                    {
                        i++;
                    }
                }
                if (*(c + i) > *(d + i))
                {
                    long temp = 0;
                    temp = ptr[count];
                    ptr[count] = ptr[j];
                    ptr[j] = temp;
                }
            }
            else
            {
                int off_1 = 0, off_2 = 0;

                if (*c <= 90)
                {
                    off_1 = 32;
                }
                if (*d <= 90)
                {
                    off_2 = 32;
                }

                int i = 0;
                if (*c + off_1 == *d + off_2)
                {
                    while (*(c + off_1 + i) == *(d + off_2 + i))
                    {
                        i++;
                    }
                }
                if ((*c + off_1 + i) > (*d + off_2 + i))
                {
                    long temp = 0;
                    temp = ptr[count];
                    ptr[count] = ptr[j];
                    ptr[j] = temp;
                }
            }
        }
    }

    for (count = 0; count < num_files; count++)
    {
        struct stat st;

        // Вызовите stat, чтобы получить статистическую информацию о файле
        if (stat((char *)ptr[count], &st))
        {
            // If stat() fails
            write(2, "Stat() failed\n", mx_strlen("Stat() failed\n"));
            free(ptr);
            return -1;
        }

        // Check if a directory
        if (S_ISDIR(st.st_mode))
        {
            write(1, "d", 1);
        }
        else
        {
            write(1, "-", 1);
        }

        // Проверить разрешение владельца.
        // Чтение, запись и поиск или выполнение для владельца файла
        mode_t permission = st.st_mode & S_IRWXU;

        //Разрешение на чтение для владельца файла
        if (permission & S_IRUSR)
        {
            write(1, "r", 1);
        }
        else
        {
            write(1, "-", 1);
        }

        //Разрешение на запись для владельца файла.
        if (permission & S_IWUSR)
        {
            write(1, "w", 1);
        }
        else
        {
            write(1, "-", 1);
        }

        //Разрешение на поиск (для каталога) или разрешение на
        // выполнение (для файла) для владельца файла
        if (permission & S_IXUSR)
        {
            write(1, "x", 1);
        }
        else
        {
            write(1, "-", 1);
        }

        // Разрешение на чтение, запись и поиск или выполнение для группы файла
        permission = st.st_mode & S_IRWXG;

        if (permission & S_IRGRP)
        {
            write(1, "r", 1);
        }
        else
        {
            write(1, "-", 1);
        }

        if (permission & S_IWGRP)
        {
            write(1, "w", 1);
        }
        else
        {
            write(1, "-", 1);
            ;
        }

        if (permission & S_IXGRP)
        {
            write(1, "x", 1);
            ;
        }
        else
        {
            write(1, "-", 1);
        }

        // CHeck other's permission
        permission = st.st_mode & S_IRWXO;

        //Разрешение на чтение для пользователей, кроме владельца файла
        if (permission & S_IROTH)
        {
            write(1, "r", 1);
        }
        else
        {
            write(1, "-", 1);
        }

        //Разрешение на запись для пользователей, отличных от владельца файла
        if (permission & S_IWOTH)
        {
            write(1, "w", 1);
        }
        else
        {
            write(1, "-", 1);
        }

        if (permission & S_IXOTH)
        {
            write(1, "x", 1);
        }
        else
        {
            write(1, "-", 1);
        }

        // Вывести количество жестких ссылок
        write(1, " ", 1);
        mx_printint((int)st.st_nlink);
        write(1, " ", 1);

        // Get the user name
        struct passwd *pt = getpwuid(st.st_uid);
        write(1, pt->pw_name, mx_strlen(pt->pw_name));
        write(1, " ", 1);

        // Get the group name
        struct group *p = getgrgid(st.st_gid);
        write(1, p->gr_name, mx_strlen(p->gr_name));

        /* int len = 0, tmp = 0;
         int n = (long long)st.st_size;
         if (n / 10 == 0) len = 1;
         else
         {
             while (n > 0)
             {
                 n = n / 10;
                 len += 1;
             }
         }

         if (len > tmp) {
             write(1, " ", 1);
         }
         else
             write(1, "  ", 2);
         tmp = len;*/

        // Get the file size
        write(1, " ", 1);
        mx_printint((long long)st.st_size);
        write(1, " ", 1);

        // Получение даты и времени
        // Обратите внимание, что здесь применяется некоторая логика
        // для удаления завершающего символа новой строки
        char date_time[100];

        mx_memset(date_time, 0, sizeof(date_time));

        mx_strncpy(date_time, ctime(&st.st_ctime), sizeof(date_time));

        int c = 0;
        while (date_time[c] != '\0')
        {
            if (date_time[c] == '\n')
                date_time[c] = '\0';
            c++;
        }
        write(1, date_time, mx_strlen(date_time));
        write(1, " ", 1);

        if (!file_exist((char *)ptr[count]))
        {
            if (S_ISDIR(st.st_mode))
            {
                write(1, (char *)ptr[count], mx_strlen((char *)ptr[count]));
                write(1, "\n", 1);
            }
            else
            {
                write(1, (char *)ptr[count], mx_strlen((char *)ptr[count]));
                write(1, "\n", 1);
            }
        }
        else
        {
            mx_printstr((char *)ptr[count]);
            write(1, "\n", 1);
        }
    }

    free(ptr);
    return 0;
}
