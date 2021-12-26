#include "../inc/uls.h"

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
