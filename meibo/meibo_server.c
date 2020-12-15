/*
 File: meibo_server.c
 Author: 09430509

 Created on 2019/04/10
 update on 2019/07/26
*/

#include "meibo_server.h"

/*profile*/
struct profile *new_profile(struct profile *pro, char *str);
char *date_to_string(char buf[], struct date *date);

/*GLOBAL*/
struct profile profile_data_store[10000];
int profile_data_nitems = 0;
int quick_count = 0;
char send_buffer[MAXLEN] = {};

int subst(char *str, char c1, char c2)
{
    int count = 0;
    while (*str != '\0')
    {
        if (*str == c1)
        {
            *str = c2;
            count++;
        }
        str++;
    }
    return count;
}

int split(char *str, char *ret[], char sep, int max)
{
    int count = 0; //分割数

    while (1)
    {
        if (*str == '\0')
        {
            break; //からもじなら抜ける
        }

        if (count > max)
            break;
        ret[count++] = str; //str をいじれば ret も変わるように分割後の文字列にはポインタを入れる

        while ((*str != '\0') && (*str != sep))
        { //区切り文字が見つかるまでポインタすすめる
            str++;
        }

        if (*str == '\0')
        {
            break; //区切り文字がなかったら抜ける＝文字列はそのまま
        }

        *str = '\0'; //必ず区切り文字のはずだからくぎる
        str++;       //インクリメントさせる
    }

    if (count < max)
        count = luck;
    else if (count > max)
        count = over;
    return count;
}

int get_line(char *input)
{
    return get_line_fp(stdin, input);
}

int get_line_fp(FILE *fp, char *input)
{
    if (fgets(input, LIMIT + 1, fp) == NULL)
    {
        fprintf(stderr, "ERROR %d:NULL--getline()\n", null);
        return 0; /* 失敗EOF */
    }
    subst(input, '\n', '\0');

    return 1; /*成功*/
}

void error_split(int check)
{
    switch (check)
    {
    case luck:
        fprintf(stderr, "ERROR %d:luck--split()\n", LUCK);
        break;

    case over:
        fprintf(stderr, "ERROR %d:over--split()\n", OVER);
        break;

    default:
        break;
    }
    return;
}

void parse_line(char *line)
{
    char *ret[2];
    int com = 0;

    if (line[0] == '%')
    {
        com = split(line, ret, ' ', 2);
        exec_command(ret[0], ret[1]);
    }
    else
    {
        new_profile(&profile_data_store[profile_data_nitems], line);
    }
}

void exec_command(char *cmd, char *param)
{
    if (strcmp(cmd, "%C") == 0 || strcmp(cmd, "%c") == 0)
    {
        cmd_check();
    }
    else if (strcmp(cmd, "%P") == 0 || strcmp(cmd, "%p") == 0)
    {
        cmd_print(&profile_data_store[0], atoi(param));
    }
    else if (strcmp(cmd, "%H") == 0 || strcmp(cmd, "%h") == 0)
    {
        cmd_help();
    }
    else if (strcmp(cmd, "%W") == 0 || strcmp(cmd, "%w") == 0)
    {
        cmd_write();
    }
    else
    {
        sprintf(send_buffer, "ERROR %d:%s command is not defined.--exec_command()\n", NOTDEFINED, cmd);
        send_to_client(send_buffer);
    }
}

void cmd_help()
{
    char help_list[] = "Q : quit client\nC : check data num\nP [value] : print data\nR [filename] : read csv data from client\nW [filename] : write csv data to client\n";
    send_to_client(help_list);
    return;
}

void cmd_check()
{
    sprintf(send_buffer, "%d profile(s)\n", profile_data_nitems);
    send_to_client(send_buffer);
    return;
}

void cmd_print(struct profile *pro, int param)
{
    int i;

    if (profile_data_nitems == 0)
    {
        sprintf(send_buffer, "ERROR %d:No record. No print.--cmd_print()\n", NORECORD);
        send_to_client(send_buffer);
        return;
    }

    if (param == 0)
    { //０のとき
        for (i = 0; i < profile_data_nitems; i++)
        {
            printdata(pro + i, i);
        }
    }

    else if (param > 0)
    { //正のとき

        if (param > profile_data_nitems)
        {
            sprintf(send_buffer, "ERROR %d:over number of record.--cmd_print()\nERROR %d:number of item is %d\n", OVERNUMBERRECORD, NUMITEM, profile_data_nitems);
            send_to_client(send_buffer);
            return;
        }

        for (i = 0; i < param; i++)
        {
            printdata(pro + i, i);
        }
    }

    else if (param < 0)
    { //負の時

        param *= -1;
        if (param > profile_data_nitems)
        {
            sprintf(send_buffer, "ERROR %d:over number of record.--cmd_print()\nERROR %d:number of item is %d\n", OVERNUMBERRECORD, NUMITEM, profile_data_nitems);
            send_to_client(send_buffer);
            return;
        }

        pro += profile_data_nitems - param;

        for (i = 0; i < param; i++)
        {
            printdata(pro + i, profile_data_nitems - param + i);
        }
    }else{
        sprintf(send_buffer, "ERROR : No Param\n");
        send_to_client(send_buffer);
        return;
    }
    return;
}

void printdata(struct profile *pro, int i)
{
    char buf[MAXLEN] = {};
    sprintf(buf,"data : %5d ------------------------------\nId : %d\nName : %s\nBirth : %04d-%02d-%02d\nAddr : %s\nCom. : %s\n--------------------------------------------\n", i + 1, pro->id, pro->name, pro->found.y, pro->found.m, pro->found.d, pro->add, pro->others);
    send_to_client(buf);
    return;
}

void cmd_write()
{
    char buf[MAXLEN] = {};
    for (int i = 0; i < profile_data_nitems; i++)
    {
        sprintf(buf,"%d,%s,%04d-%02d-%02d,%s,%s\n", profile_data_store[i].id, profile_data_store[i].name, profile_data_store[i].found.y, profile_data_store[i].found.m, profile_data_store[i].found.d, profile_data_store[i].add, profile_data_store[i].others);
        send_to_client(buf);
    }
    send_to_client("END");
    return;
}

struct profile *new_profile(struct profile *pro, char *str)
{
    char *ret1[maxsplit], *ret2[maxsplit - 2];
    int count = 0;
    if (profile_data_nitems >= 10000)
    {
        sprintf(send_buffer, "ERROR %d:Can't add record--new_profile()\n", OVERNITEMS);
        return NULL;
    }
    count = split(str, ret1, ',', maxsplit);
    if (count != maxsplit)
    {
        error_split(count);
        sprintf(send_buffer, "ERROR %d:wrong format of input(ex.001,name,1999-01-01,address,other)--new_profile()\n", FORMATINPUT);
        return NULL;
    } //文字列用

    pro->id = strtol(ret1[0], endp, base1);
    if (pro->id == 0)
    {
        sprintf(send_buffer, "ERROR %d:ID is NUMBER.--new_profile()\n", FORMATID);
        return NULL;
    }

    strncpy(pro->name, ret1[1], 70); //名前のコピー
    strncpy(pro->add, ret1[3], 70);  //住所
    pro->others = (char *)malloc(sizeof(char) * (strlen(ret1[4]) + 1));
    strcpy(pro->others, ret1[4]); //備考,MAX 1024bytes

    if (split(ret1[2], ret2, '-', maxsplit - 2) != maxsplit - 2)
    {
        sprintf(send_buffer, "ERROR %d:wrong format of date.(ex.1999-01-01)--new_profile()\n", FORMATDATE);
        return NULL;
    } //設立日
    pro->found.y = strtol(ret2[0], endp, base1);
    pro->found.m = strtol(ret2[1], endp, base1);
    pro->found.d = strtol(ret2[2], endp, base1);

    send_to_client("Add profile.\n");
    profile_data_nitems++;
    return pro;
}

void send_to_client(char *send_buffer){
    //send
    char tmp[MAXLEN] = "\0";
    strcpy(tmp, send_buffer);
    
    sn2 = send(new_sockfd, tmp, sizeof(tmp), 0);
    printf("\n\n:::send:::\n%s\n\n", send_buffer);
    if (sn2 == -1)
    {
        printf("sn2 = %d, errno=%d: %s\n", sn2, errno, strerror(errno));
        exit(1);
    }
}