#include <stdio.h>
#include <fcntl.h>
#define SIZE 255
// Use:: $./a.out ./output/1.txt

int main(int argc, char *argv[]){
    char buf[SIZE];

    // read stdin
    int rs = read(0, buf, SIZE);
    if(rs == -1){
        printf("E_READ");
        return -1;
    }

    //file open
    int fd = open(argv[1], O_WRONLY);
    if(fd == -1){
        printf("E_OPEN");
        return -1;
    }

    //file write
    int fw = write(fd, buf, rs);
    if(fw == -1){
        printf("E_WRITE");
        return -1;
    }

    //file close
    close(fd);

    return 0;
}