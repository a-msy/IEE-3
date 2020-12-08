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
    int sockfd, new_sockfd, writer_len, rc, sn, sn2;
    struct sockaddr_in sa;
    struct sockaddr_in writer_addr;
    char receive_buffer[MAXLEN]={};

    // make read socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
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
    if(bd == -1){
        printf("bd = %d, errno=%d: %s\n", bd, errno, strerror(errno));
        return 1;
    }

    int ls = listen(sockfd, 5);
    if(ls == -1){
        printf("ls = %d, errno=%d: %s\n", ls, errno, strerror(errno));
        return 1;
    }

    while(1){
        new_sockfd = accept(sockfd, (struct sockaddr *)&writer_addr, &writer_len);
        if(new_sockfd == -1){
            printf("new_sockfd = %d, errno=%d: %s\n", new_sockfd, errno, strerror(errno));
            return 1;
        }else{
            rc = recv(new_sockfd, receive_buffer, sizeof(receive_buffer), 0);
            if(rc == -1){
                printf("rc = %d, errno=%d: %s\n", rc, errno, strerror(errno));
                return 1;
            }else{
                printf("Receive : %s\n",receive_buffer);
                /* アルファベットの小文字を大文字に変換 */
                for(int i=0; i<=strlen(receive_buffer); i++){
                    /* アルファベットの小文字なら変換 */
                    if(receive_buffer[i]>=97&&receive_buffer[i]<=122) receive_buffer[i]=receive_buffer[i]-32;
                }
                /* 変換した文字列を出力 */
                printf("Converted : %s\n", receive_buffer);
                
                //send
                sn2 = send(new_sockfd, receive_buffer, strlen(receive_buffer), 0);
                if(sn2 == -1){
                    printf("sn2 = %d, errno=%d: %s\n", rc, errno, strerror(errno));
                }
            }
        }
    }
    close(sockfd);

    return 0;
}