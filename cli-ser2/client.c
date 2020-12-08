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
    
    // get host
    struct hostent *host;
    host = gethostbyname("localhost");
    if(host == NULL){
        printf("hostError\n");
        return -1;
    }

    // socket
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == -1){
        printf("soc = %d, errno=%d: %s", soc, errno, strerror(errno));
        return -1;
    }

    //connect
    struct sockaddr_in sa;
    sa.sin_family = host->h_addrtype; //host address type
    sa.sin_port = htons(PORT_NO); // port number
    bzero((char *)&sa.sin_addr, sizeof(sa.sin_addr));
    memcpy((char *)&sa.sin_addr, (char *)host->h_addr, host->h_length);

    int con = connect(soc, (struct sockaddr *)&sa, sizeof(sa));
    if(con == -1){
        printf("con = %d, errno=%d: %s", con, errno, strerror(errno));
        return -1;
    }

    // send request
    char s_buf[MAXLEN] = {};
    read(0, s_buf, sizeof(s_buf));

    int sd = send(soc, s_buf, sizeof(s_buf), 0);
    if(sd == -1){
        printf("sd = %d, errno=%d: %s", sd, errno, strerror(errno));
        return -1;
    }

    // receive message
    char r_buf[4000]={};
    int rec = recv(soc, r_buf, sizeof(r_buf), 0);
    
    if(rec == -1){
        printf("rec = %d, errno=%d: %s", rec, errno, strerror(errno));
        return -1;
    }
    printf("%s\n",r_buf);

    close(soc);
    return 0;
}