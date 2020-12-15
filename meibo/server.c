#include "meibo_server.h"

/*MAIN*/
int main(void)
{
    int sockfd, writer_len, rc, sn;
    struct sockaddr_in sa;
    struct sockaddr_in writer_addr;
    char receive_buffer[MAXLEN] = {};
    char line[LIMIT + 1];

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
            return 1;
        }
        else
        {
            char tmp[MAXLEN] = "\0";
            strcpy(tmp, receive_buffer);

            rc = recv(new_sockfd, tmp, sizeof(tmp), 0);
            if (rc == -1)
            {
                printf("rc = %d, errno=%d: %s\n", rc, errno, strerror(errno));
                return 1;
            }
            else
            {
                printf("\n\n:::receive:::\n%s\n\n", tmp);
                parse_line(tmp);
            }
        }
    }
    close(sockfd);
    return 0;
}