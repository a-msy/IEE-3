#include <stdio.h>
#include <fcntl.h>
#define SIZE 255
// Use:: $./a.out ./input/1.txt

int main(int argc, char *argv[]){
    char buf[SIZE];

    //file open
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        printf("E_FILE_OPEN\n");
        return -1;
    }

    //file read
    int fr = read(fd, buf, SIZE);
    if(fr == -1){
        printf("E_F_READ\n");
    }

    //print
    printf("%s\n",buf);

    //file close
    close(fd);

    return 0;
}