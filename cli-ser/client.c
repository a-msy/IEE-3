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
        printf("This host is not exist\n");
        return -1;
    }

    // socket
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == -1){
        printf("Can not make socket\n");
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
        printf("Failed to connect\n");
        return -1;
    }

    // send request
    char s_buf[MAXLEN] = {};
    read(0, s_buf, sizeof(s_buf));

    int sd = send(soc, s_buf, sizeof(s_buf), 0);
    if(sd == -1){
        printf("Failed to send\n");
        return -1;
    }

    // receive message
    char r_buf[4000]={};
    int rec = recv(soc, r_buf, sizeof(r_buf), 0);
    
    if(rec == -1){
        printf("Failed to receive\n");
        return -1;
    }
    printf("Converted : %s\n",r_buf);

    close(soc);
    return 0;
}