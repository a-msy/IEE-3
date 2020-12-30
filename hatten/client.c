#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define DIGITS 4 //4 numbers hit and blow
#define COR 7 // Count of responses

char answer[DIGITS] = {};

int main(void){
    int i, j, hit, blow;
    int count = 0;
    srand((unsigned int)time(NULL));

    // 0から9までの乱数を発生
    for (i = 0; i < DIGITS; i++)
    {
        answer[i] = (rand() % 10) + '0';
        // 同じ数字が出ていたら排除
        for (j = 0; j < i; j++){
            if(answer[i] == answer[j]){
                answer[i] = (rand() % 10) + '0';
            }
        }
    }

    while (count < COR)
    {
        i = 0;
        hit = 0;
        blow = 0;
        char input[DIGITS] = {};
        int c;
        printf("Please Input Number of %d Degits => ", DIGITS);
        scanf("%4s", &input);

        for (i = 0; i < DIGITS; i++)
        {
            for (j = 0; j < DIGITS; j++)
            {
                if (answer[i] == input[j])
                {
                    if (i == j)
                    {
                        hit++;
                    }
                    else
                    {
                        blow++;
                    }
                }
            }
        }
        if (hit == DIGITS)
        {
            printf("GG.\n");
        }
        printf("%s is %d Hit, %d Blow.\n\n", input, hit, blow);
        count++;
    }
    printf("ANSWER IS %s\n", answer);
    return 0;
}