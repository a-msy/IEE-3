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
    null,
    LUCK,
    OVER,
    NOTDEFINED,
    NORECORD,
    OVERNUMBERRECORD,
    FORMATINPUT,
    FORMATID,
    FORMATDATE,
    NUMITEM,
    ERRORNUM,
    NOFILEOPEN,
    OVERNITEMS,
    PARAMERROR,
} ERROR;
typedef enum
{
    Q,
    C,
    P,
    E,
    R,
    W,
    BR,
    BW,
    F,
    FB,
    S,
    QS,
    D,
    SIZE,
    LIST
} HELP;
struct date
{
    int y; //year
    int m; //month
    int d; //day
};
struct profile
{
    int id;        //id
    char name[70]; //schoolname
    struct date found;
    char add[70]; //address
    char *others; //備考
};

int sn2, new_sockfd; //send sock

/*subst*/
int subst(char *str, char c1, char c2);
/*split*/
int split(char *str, char *ret[], char sep, int max);
void error_split(int check);
/*get_line*/
int get_line(char *input);
int get_line_fp(FILE *fp, char *input);
/*parse_line*/
void parse_line(char *line);
void printdata(struct profile *pro, int i);
/*cmd*/
void exec_command(char *cmd, char *param);
void cmd_quit();
void cmd_check();
void cmd_print(struct profile *pro, int param);
void cmd_pex(int param);
void cmd_read(char *filename);
void cmd_read_server(char *filename);
void cmd_write(char *filename);
void swap_struct(struct profile *i, struct profile *j);
int compare_profile(struct profile *p1, struct profile *p2, int column);
int compare_date(struct date *d1, struct date *d2);
int partition(int left, int right, int youso);
void quick_sort(int left, int right, int youso);
void cmd_help();
int find_kai(char *s, char *cp);

void send_to_client(char *send_buffer);