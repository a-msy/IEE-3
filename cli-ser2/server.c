#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT_NO 8080
#define MAXLEN 128

int main(void){
    int sockfd, new_sockfd, writer_len, rc, sn, sn2, count;
    struct sockaddr_in sa;
    struct sockaddr_in writer_addr;
    char receive_buffer[MAXLEN]={};
    char send_buffer[MAXLEN]={};

    // make read socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("sockfd = %d, errno=%d: %s", sockfd, errno, strerror(errno));
        return 1;
    }

    //setting
    memset((char *)&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(PORT_NO);

    //bind
    int bd = bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    if(bd == -1){
        printf("bd = %d, errno=%d: %s", bd, errno, strerror(errno));
        return 1;
    }

    int ls = listen(sockfd, 5);
    if(ls == -1){
        printf("ls = %d, errno=%d: %s", ls, errno, strerror(errno));
        return 1;
    }

    while(1){
        new_sockfd = accept(sockfd, (struct sockaddr *)&writer_addr, &writer_len);
        if(new_sockfd == -1){
            printf("new_sockfd = %d, errno=%d: %s", new_sockfd, errno, strerror(errno));
            return 1;
        }else{
            rc = recv(new_sockfd, receive_buffer, sizeof(receive_buffer), 0);
            if(rc == -1){
                printf("rc = %d, errno=%d: %s", rc, errno, strerror(errno));
                return 1;
            }else{
                printf("Receive : %s",receive_buffer);
                count = strlen(receive_buffer);
                sprintf(send_buffer, "Count : %d kaigyou moji ga aru", count);
                printf("%s",send_buffer);
            
                //send
                sn2 = send(new_sockfd, send_buffer, strlen(send_buffer), 0);
                if(sn2 == -1){
                    printf("sn2 = %d, errno=%d: %s", rc, errno, strerror(errno));
                    return 1;
                }
            }
        }
    }
    close(sockfd);
    return 0;
}