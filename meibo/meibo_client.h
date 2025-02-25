#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT_NO 10090
#define MAXLEN 4096
#define LIMIT 1024
#define maxsplit 5 //最大分割数
#define luck -1
#define over -2
#define endp NULL //strtol 用ポインタ
#define base1 10  //10進数

typedef enum
{
    null,LUCK,OVER,
    NOTDEFINED,NORECORD,
    OVERNUMBERRECORD,
    FORMATINPUT,FORMATID,
    FORMATDATE,NUMITEM,
    ERRORNUM,NOFILEOPEN,
    OVERNITEMS,PARAMERROR,
} ERROR;

struct hostent *host;
struct sockaddr_in sa;
int soc, con, sd, rec;
void request(char *s_buf);
void request_p(char *s_buf, int count);
void request_r(char *filename);
void request_w(char *filename);

/*subst*/
int subst(char *str, char c1, char c2);
/*split*/
int split(char *str, char *ret[], char sep, int max);
void error_split(int check);
/*get_line*/
int get_line(char *input);
int get_line_fp(FILE *fp, char *input);

void parse_line(char *line);
void exec_command(char *cmd, char *param);
void soc_con();