/*----------------------------------------------------------------

*

* Programación avanzada: Manejo de directorios

* Fecha: 15-Sep-2020

* Autor: A01421954Fernando Amézquita, A01208937 Santiago Arellano

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>

#define NEQ(str1, str2) (strcmp(str1, str2) != 0)
#define USAGE_ERROR -2
#define FILE_ERROR -3
#define WORD_MAX

long SizeOfStr(char *str)
{
    return sizeof(str) / sizeof(char);
}

int ValidName(char *key, char *name)
{
    long si1 = strlen(key);
    long si2 = strlen(name);
    if (si1 > si2)
    {
        return 0;
    }
    int i;
    for (i = 0; i < si1 && i < si2; i++)
    {
        if (key[i] != name[i])
        {
            return 0;
        }
    }
    return 1;
}

void find(char *path, char *searchWord, int recursive)
{
    DIR *dirp;
    struct dirent *dp;
    struct stat buf;
    char fpath[256], file_type;
    int ok;

    if ((dirp = opendir(path)) == NULL)
    {
        perror(path);
        return;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        if (NEQ(dp->d_name, ".") && NEQ(dp->d_name, ".."))
        {
            // iterate over files in directory
            sprintf(fpath, "%s/%s", path, dp->d_name);
            ok = stat(fpath, &buf);
            if (ok != -1 && ((buf.st_mode & S_IFMT) == S_IFDIR))
            {
                // es un directorio
                if (recursive)
                {
                    find(fpath, searchWord, recursive);
                }
            }
            else if (ok != -1 && ValidName(searchWord, dp->d_name) == 1)
            {
                printf("Name: %s\n", dp->d_name);
                printf("Path: %s\n", path);
                printf("Last access: %s", ctime(&buf.st_ctime));
                printf("Last modification: %s\n", ctime(&buf.st_atime));
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "usage: find string initial_directory [-r]\n");
        return USAGE_ERROR;
    }

    // attempt to open directory
    DIR *dir;
    errno = 0;
    if ((dir = opendir(argv[2])) == NULL)
    {
        switch (errno)
        {
        case ENOTDIR:
            fprintf(stderr, "find: Not a directory\n");
            break;

        default:
            fprintf(stderr, "find: No such file or directory %s\n", argv[2]);
            break;
        }
        return FILE_ERROR;
    }

    // check if 4th param exists and is recursive.
    if (argc == 4 && strcmp(argv[3], "-r") != 0)
    {
        // usage isnt -r
        fprintf(stderr, "usage: find string initial_directory [-r]\n");
        return USAGE_ERROR;
    }
    else if (argc == 4 && strcmp(argv[3], "-r") == 0)
    {
        // call is recursive
        find(argv[2], argv[1], 1);
    }
    else
    {
        // standard usage
        find(argv[2], argv[1], 0);
    }

    return 0;
}