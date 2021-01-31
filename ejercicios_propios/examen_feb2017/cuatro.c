#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>

int main()
{
    mkfifo("tuberia1", 0644);
    mkfifo("tuberia2", 0644);

    int tub1 = open("tuberia1", O_RDONLY | O_NONBLOCK);
    int tub2 = open("tuberia2", O_RDONLY | O_NONBLOCK);

    char buffer[256];
    int selection, tubSelected;
    do{
        fd_set set;
        FD_ZERO(&set);
        FD_SET(tub1, &set);
        FD_SET(tub2, &set);

        selection = select((tub1 < tub2) ? tub2 + 1 : tub1 + 1, &set, NULL, NULL, NULL);
        if(selection > 0)
        {
            int numTub = 0;
            if(FD_ISSET(tub1, &set))
            {
                tubSelected = tub1;
                numTub = 1;
            }
            else if(FD_ISSET(tub2, &set))
            {
                tubSelected = tub2;
                numTub = 2;
            }

            ssize_t bufferSize = 256;
            while (bufferSize == 256)
            {
                bufferSize = read(tubSelected, buffer, 256);
                buffer[bufferSize] = '\0';
                printf ("[Tuberia%i]: %s", numTub, buffer);
            }

            if(bufferSize != 256 && numTub == 1){
                close(tub1);
                tub1 = open("tuberia1", O_RDONLY | O_NONBLOCK);
            }

            if(bufferSize != 256 && numTub == 2){
                close(tub2);
                tub2 = open("tuberia2", O_RDONLY | O_NONBLOCK);
            }

        }

    }while(selection != -1);

    close(tub1);
    close(tub2);

    return 0;
}