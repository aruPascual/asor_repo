#include <unistd.h>
#include <stdio.h>
//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    printf("Maximum number of links to the file: %li\n",
           fpathconf(open("ejercicio7.c", O_RDONLY), _PC_LINK_MAX));
    printf("Maximum length of a relative pathname: %li\n",
           fpathconf(open("ejercicio7.c", O_RDONLY), _PC_PATH_MAX));
    printf("Maximum length of a filename: %li\n",
           fpathconf(open("ejercicio7.c", O_RDONLY), _PC_NAME_MAX));
    return 0;
}

/*
Maximum number of links to the file: 65000
Maximum length of a relative pathname: 4096
Maximum length of a filename: 255
*/