#include "../inc/uls.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool file_exist(char *name)
{
    struct stat buf;
    return (stat(name, &buf) == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

int mx_no_flags(void)
{
    char *curr_dir = NULL;
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    unsigned int count = 0;
    long *ptr = NULL;
    struct winsize w;

    //чтобы получить количество строк и столбцов, видимых на терминале
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // Получить текущий рабочий каталог
    curr_dir = "./";

    if (NULL == curr_dir)
    {
        write(2, "ERROR : Could not get the working directory\n", mx_strlen("ERROR : Could not get the working directory\n"));
        return -1;
    }

    // Переменная для хранения количества файлов внутри каталога
    int num_files = 0;

    // open the directory
    dp = opendir((const char *)curr_dir);

    // Start reading the directory contents
    while ((dptr = readdir(dp)) != NULL)
    {
        // Не считайте файлы, начинающиеся с "."
        if (dptr->d_name[0] != '.')
            num_files++;
    }

    // Наша цель состояла в том, чтобы подсчитать
    // количество файлов/папок в текущем рабочем каталоге.
    // Поскольку это сделано, закройте каталог.
    closedir(dp);

    // Восстановите значения, так как мы будем использовать
    // их позже снова
    dp = NULL;
    dptr = NULL;

    // Убедитесь, что в текущем
    // рабочем каталоге должен быть хотя бы один файл/папка.
    // (бесполезное условие, т.к у нас всегда будет хотя бы один исполнительный файл)
    if (!num_files)
    {
        return 0;
    }
    else
    {
        // Выделить память для хранения
        // адресов имен содержимого в текущем рабочем каталоге
        ptr = malloc(num_files * 8);
        if (NULL == ptr)
        {
            write(2, "Memory allocation failed\n", mx_strlen("Memory allocation failed\n"));
            return -1;
        }
        else
        {
            // Инициализировать память нулями
            mx_memset(ptr, 0, num_files * 8);
        }
    }

    // Open the directory again
    dp = opendir((const char *)curr_dir);

    if (NULL == dp)
    {
        write(2, "ERROR : Could not open the working directory\n", mx_strlen("ERROR : Could not open the working directory\n"));
        free(ptr);
        return -1;
    }

    // Начните перебирать каталог и прочитайте все его
    // содержимое внутри массива, выделенного выше
    unsigned int j = 0;

    for (count = 0; (dptr = readdir(dp)) != NULL; count++)
    {
        if (dptr->d_name[0] != '.')
        {
            ptr[j] = (long)dptr->d_name;
            j++;
        }
    }

    // Начните сортировать имена в алфавитном порядке
    // Используя пузырьковую сортировку здесь
    for (int count = 0; count < num_files - 1; count++)
    {
        for (int j = count + 1; j < (num_files); j++)
        {
            char *c = (char *)ptr[count];
            char *d = (char *)ptr[j];

            // Убедитесь, что два символа должны быть из одного набора
            if (((*c >= 65) && (*d >= 65)) || ((*c <= 122) && (*d <= 122)))
            {
                int i = 0;
                // Если начальные символы совпадают, продолжайте
                // сравнивать символы, пока не будет обнаружено различие
                if (*c == *d)
                {
                    while (*(c + i) == *(d + i))
                    {
                        i++;
                    }
                }

                // Проверьте, не превышает ли ранее сохраненное
                // значение в алфавитном порядке следующему значению
                if (*(c + i) > *(d + i))
                {
                    // Да, меняем значение
                    long temp = 0;
                    temp = ptr[count];
                    ptr[count] = ptr[j];
                    ptr[j] = temp;
                }
            }
            else
            {
                // если два начальных символа не принадлежат к
                // одному и тому же набору ASCII, сделайте их одинаковыми
                // и затем сравните
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

                // После того, как набор символов будет таким же, проверьте,
                // совпадают ли начальные символы. Если да, продолжайте поиск,
                // пока не обнаружите разницу
                if (*c + off_1 == *d + off_2)
                {
                    while (*(c + off_1 + i) == *(d + off_2 + i))
                    {
                        i++;
                    }
                }
                // После того, как разница будет обнаружена, проверьте, требуется ли замена
                if ((*c + off_1 + i) > (*d + off_2 + i))
                {
                    // Да, идем дальше и меняем
                    long temp = 0;
                    temp = ptr[count];
                    ptr[count] = ptr[j];
                    ptr[j] = temp;
                }
            }
        }
    }

    // Теперь имена отсортированы по алфавиту. Начать вывод на консоль
    for (int count = 0; count < num_files; count++)
    {
        // Проверьте, является ли файл/папка исполняемым
        if (!file_exist((char *)ptr[count]))
        {
            int fd = -1;
            struct stat st;

            if (!opendir((char *)ptr[count]))
            {
                write(2, "Opening file/Directory failed\n", mx_strlen("Opening file/Directory failed\n"));
                free(ptr);
                return -1;
            }

            stat((char *)ptr[count], &st);
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
            close(fd);
        }
        else
        {
            write(1, (char *)ptr[count], mx_strlen((char *)ptr[count]));
            write(1, "\n", 1);
        }
    }

    // Free the allocated memory
    free(ptr);
    return 0;
}

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
                //mx_printchar('\n');
//////////////////////////////////////////////////////////////////// здесь вывод содержимого указанных папок
                i = 0;
                char *dir_name = malloc(len2);
                int k = 0;
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

                    while ((dptr = readdir(dp)) != NULL)
                    {
                        // Не считайте файлы, начинающиеся с "."
                        if (dptr->d_name[0] != '.')
                        {
                            mx_printstr(dptr->d_name);
                            mx_printchar('\n');
                        }
                    }
                    mx_printchar('\n');
                    k = 0;
                    closedir(dp);
                    i++;
                }
                free(files);
                free(dir_name);
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
