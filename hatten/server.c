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
#define MAXLEN 4096
#define DIGITS 4 //4 numbers hit and blow
#define COR 7    // Count of responses

void makeAnswer();
char answer[DIGITS] = {};
int new_sockfd, sn2; //send sock
void send_to_client(char *send_buffer);

int main(void)
{
    int sockfd, writer_len, rc, sn;
    int i, j, hit, blow;
    struct sockaddr_in sa;
    struct sockaddr_in writer_addr;


    // make read socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("sockfd = %d, errno=%d: %s\n", sockfd, errno, strerror(errno));
        return 1;
    }

    //setting
    memset((char *)&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(PORT_NO);

    //bind
    int bd = bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    if (bd == -1)
    {
        printf("bd = %d, errno=%d: %s\n", bd, errno, strerror(errno));
        return 1;
    }

    int ls = listen(sockfd, 5);
    if (ls == -1)
    {
        printf("ls = %d, errno=%d: %s\n", ls, errno, strerror(errno));
        return 1;
    }
    while (1)
    {
        new_sockfd = accept(sockfd, (struct sockaddr *)&writer_addr, &writer_len);
        if (new_sockfd == -1)
        {
            printf("new_sockfd = %d, errno=%d: %s\n", new_sockfd, errno, strerror(errno));
            break;
        }
        else
        {
            char receive_buffer[DIGITS] = "\0";
            char tmp[MAXLEN] = {};
            rc = recv(new_sockfd, receive_buffer, sizeof(receive_buffer), 0);
            if (rc == -1)
            {
                printf("rc = %d, errno=%d: %s\n", rc, errno, strerror(errno));
                break;
            }
            printf("recv :%s\n", receive_buffer);
            if (strcmp(receive_buffer, "s") == 0)
            {
                makeAnswer();
                sprintf(tmp, "i");
                send_to_client(tmp);
            }
                else
                {
                    hit = 0;
                    blow = 0;
                    for (i = 0; i < DIGITS; i++)
                    {
                        for (j = 0; j < DIGITS; j++)
                        {
                            if (answer[i] == receive_buffer[j])
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
                        sprintf(tmp, "g");
                        send_to_client(tmp);
                    }
                    else{
                    sprintf(tmp, "%dh%db\n", hit, blow);
                    send_to_client(tmp);
                }
            }
        }
    }
    close(sockfd);

    return 0;
}

void makeAnswer(){
    int i,j;
    srand((unsigned int)time(NULL));

    // 0から9までの乱数を発生
    for (i = 0; i < DIGITS; i++)
    {
        answer[i] = (rand() % 10) + '0';
        // 同じ数字が出ていたら排除
        for (j = 0; j < i; j++)
        {
            if (answer[i] == answer[j])
            {
                answer[i] = (rand() % 10) + '0';
            }
        }
    }
    printf("generate Answer %s\n",answer);
    return;
}
void send_to_client(char *send_buffer)
{
    //send
    char tmp[MAXLEN] = "\0";
    strcpy(tmp,send_buffer);
    printf("sn: %s\n", tmp);
    sn2 = send(new_sockfd, tmp, sizeof(tmp), 0);
    if (sn2 == -1)
    {
        printf("sn2 = %d, errno=%d: %s\n", sn2, errno, strerror(errno));
        return;
    }
}