#include <stdio.h>
#include <fcntl.h>
#define SIZE 255
// Use:: $./a.out | ../kadai1/a.out ../kadai1/output/1.txt
// Use:: $../kadai2/a.out ../kadai2/input/1.txt | ./a.out

int main(int argc, char *argv[]){
    char buf[SIZE] = {};

    //file read
    int fr = read(0, buf, SIZE);
    if(fr == -1){
        printf("E_READ\n");
    }

    //print
    printf("%s\n",buf);

    return 0;
}