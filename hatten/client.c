#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT_NO 10090
#define DIGITS 4 //4 numbers hit and blow
#define COR 7 // Count of responses
#define MAXLEN 4096

int main(void){
    int count = 0;
    int start = 0;
    char r_buf[MAXLEN] = {};

    // get host
    struct hostent *host;
    host = gethostbyname("localhost");
    if (host == NULL)
    {
        printf("hostError\n");
        return 0;
    }
    printf("Input s to start hit and blow.\nInput e to end this program.\n");
    while (1)
    {
        // socket
        int soc = socket(AF_INET, SOCK_STREAM, 0);
        if (soc == -1)
        {
            printf("soc = %d, errno=%d: %s", soc, errno, strerror(errno));
            close(soc);
            break;
        }

        //connect
        struct sockaddr_in sa;
        sa.sin_family = host->h_addrtype; //host address type
        sa.sin_port = htons(PORT_NO);     // port number
        bzero((char *)&sa.sin_addr, sizeof(sa.sin_addr));
        memcpy((char *)&sa.sin_addr, (char *)host->h_addr, host->h_length);

        int con = connect(soc, (struct sockaddr *)&sa, sizeof(sa));
        if (con == -1)
        {
            printf("con = %d, errno=%d: %s", con, errno, strerror(errno));
            close(soc);
            break;
        }

        char input[DIGITS] = {};
        scanf("%4s", &input);

        if(strcmp(input,"e") == 0){
            printf("bye.\n");
            exit(1);
        }

        if (strcmp(input, "s") == 0 && start == 1)
        {
            printf("Game is already stared.\n");
            printf("Please Input Number of %d Degits => ", DIGITS);
            close(soc);
            continue;
        }
        else
        {
            int sd = send(soc, input, sizeof(input), 0);
            if (sd == -1)
            {
                printf("sd = %d, errno=%d: %s", sd, errno, strerror(errno));
                close(soc);
                break;
            }

            // receive message
            char tmp2[MAXLEN] = "\0";
            int rec = recv(soc, tmp2, sizeof(tmp2), 0);

            if (rec == -1)
            {
                printf("rec = %d, errno=%d: %s", rec, errno, strerror(errno));
                close(soc);
                break;
            }

            if(strcmp(tmp2,"i") == 0){
                start = 1;
                count++;
                printf("Please Input Number of %d Degits => ", DIGITS);
            }
            else if(strcmp(tmp2,"g") == 0){
                printf("Answer is %s\n %d tried.\n",input,count);
                close(soc);
                break;
            }
            else
            {
                count++;
                printf("%s\n", tmp2);
                printf("Please Input Number of %d Degits => ", DIGITS);
            }
        }
    }
    return 0;
}