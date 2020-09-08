/*----------------------------------------------------------------

*

* Programación avanzada: Examen 1

* Fecha: 8-Sep-2020

* Autor: A01208937 Santiago Arellano 

*

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

unsigned char buffer[1];

#define USAGE_ERROR -2
#define FILE_ERROR -3

int main(int argc, char **argv)
{
    int fd_origen, fd_destino, nbytes, start_byte = 0, end_byte = -1;

    // ERROR HANDLING
    if (argc != 5 && argc != 3)
    {
        fprintf(stderr, "Usage: %s source destination [start] [end]\n", argv[0]);
        return USAGE_ERROR;
    }

    if ((fd_origen = open(argv[1], O_RDONLY)) < 0)
    {
        fprintf(stderr, "%s: No such file or directory: %s\n", argv[0], argv[1]);
        return FILE_ERROR;
    }

    if (argc == 5)
    {
        start_byte = (int)strtol(argv[3], NULL, 10);
        end_byte = (int)strtol(argv[4], NULL, 10);

        if (start_byte < 0)
        {
            fprintf(stderr, "%s: start must be a positive number.\n", argv[0]);
            return USAGE_ERROR;
        }
        if (end_byte < 0)
        {
            fprintf(stderr, "%s: end must be a positive number.\n", argv[0]);
            return USAGE_ERROR;
        }
        if (end_byte < start_byte)
        {
            fprintf(stderr, "%s: end must be greater than start.\n", argv[0]);
            return USAGE_ERROR;
        }
    }

    if ((fd_destino = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0)
    {
        fprintf(stderr, "%s: Error with file: %s\n", argv[0], argv[2]);
        return FILE_ERROR;
    }

    // Copying...
    int pos = 0;
    while ((nbytes = read(fd_origen, buffer, sizeof(buffer))) > 0)
    {
        if (pos >= start_byte && (pos <= end_byte || end_byte == -1))
        {
            write(fd_destino, buffer, nbytes);
        }
        pos++;
    }

    //printf("start: %i, end: %i, pos: %i.   ", start_byte, end_byte, pos);
    if (start_byte > pos - 1)
    {
        fprintf(stderr, "%s: Start must be a valid position in the file.\n", argv[0]);
        return USAGE_ERROR;
    }
    if (end_byte > pos - 1)
    {
        fprintf(stderr, "%s: End must be a valid position in the file.\n", argv[0]);
        return USAGE_ERROR;
    }

    if (argc == 5)
        printf("%s: Copy from %i to %i.\n", argv[0], start_byte, end_byte);
    else if (argc == 3)
        printf("%s: Copy from %i to %i.\n", argv[0], start_byte, pos);

    close(fd_origen);
    close(fd_destino);
    return 0;
}
