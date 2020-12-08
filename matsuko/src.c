#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 4000
#define PORT_NO 80

int main(int argc, char *argv[])
{
    char buf[BUFSIZE];
    int s, con, send_res, recv_res;
    struct hostent *hostname;
    
    //1. 通信相手のIPアドレスの取得（ドメイン名→IPアドレスの変換）
    hostname = gethostbyname("www.edu.cs.okayama-u.ac.jp");
    if (hostname == NULL)
    {
        printf("error_gethost");
        return -1;
    }

    //2. ソケットの作成
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
    {
        printf("error_socket");
        return -1;
    }

    //3. 接続の確立
    struct sockaddr_in sa;

    sa.sin_family = hostname->h_addrtype; // host address type
    sa.sin_port = htons(PORT_NO); // port number
    bzero((char *) &sa.sin_addr, sizeof(sa.sin_addr));
    memcpy((char *)&sa.sin_addr, (char *)hostname->h_addr, hostname->h_length);
    /* ここでNo.16を参考にメンバに適切な値を設定 */
    con = connect(s, (struct sockaddr*)&sa, sizeof(sa));
    if (con == -1)
    {
        printf("error_connect");
        return -1;
    }

    //4. 要求メッセージを送信
    char request[] = "GET /index.html\r\n";
    send_res = send(s, request, sizeof(request), 0);
    if (send_res == -1)
    {
        printf("error_send");
        return -1;
    }

    //5. 応答メッセージを受信
    recv_res = recv(s, buf, BUFSIZE, 0);
    if (recv_res == -1)
    {
        printf("error_recv");
        return -1;
    }

    //6. 応答メッセージを処理
    printf("%s\n", buf);

    //7. ソケットの削除
    close(s);

    return 0;
}