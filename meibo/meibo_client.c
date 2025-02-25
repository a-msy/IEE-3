#include "meibo_client.h"

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
        fprintf(stderr, "NULL--getline()\n", null);
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

void request(char *s_buf)
{
    char r_buf[MAXLEN] = {};
    soc_con();

    // send request
    char tmp[MAXLEN] = "\0";
    strcpy(tmp, s_buf);

    sd = send(soc, tmp, sizeof(tmp), 0);
    if (sd == -1)
    {
        printf("sd = %d, errno=%d: %s", sd, errno, strerror(errno));
        return ;
    }

    // receive message
    char tmp2[MAXLEN] = "\0";
    strcpy(tmp2, r_buf);

    int rec = recv(soc, tmp2, sizeof(tmp2), 0);

    if (rec == -1)
    {
        printf("rec = %d, errno=%d: %s", rec, errno, strerror(errno));
        return ;
    }
    printf("%s", tmp2);
    close(soc);
}

void request_p(char *s_buf, int count)
{
    char r_buf[MAXLEN] = {};
    char send_text[MAXLEN] = {};

    soc_con();

    // send request
    sprintf(send_text, "%s %d", s_buf, count);
    sd = send(soc, send_text, strlen(send_text), 0);
    if (sd == -1)
    {
        printf("sd = %d, errno=%d: %s", sd, errno, strerror(errno));
        return ;
    }

    // receive message
    rec = recv(soc, r_buf, sizeof(r_buf), 0);
    if (rec == -1)
    {
        printf("rec = %d, errno=%d: %s", rec, errno, strerror(errno));
        return ;
    }
    count = atoi(r_buf);
    printf("print %d profile(s)\n", count);

    for (int i = 0; i < count; i++)
    {
        // receive message
        rec = recv(soc, r_buf, sizeof(r_buf), 0);
        if (rec == -1)
        {
            printf("rec = %d, errno=%d: %s", rec, errno, strerror(errno));
            return ;
        }
        printf("%s", r_buf);
    }
    close(soc);
}
void request_r(char *filename)
{
    char line[LIMIT + 1];
    char r_buf[MAXLEN] = {};
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("ERROR %d:openfile error!!!---cmd_read()\n", NOFILEOPEN);
        return;
    }

    while (get_line_fp(fp, line))
    {
        soc_con();
        // send request
        char tmp[MAXLEN] = "\0";
        strcpy(tmp, line);

        sd = send(soc, tmp, sizeof(tmp), 0);
        if (sd == -1)
        {
            printf("sd = %d, errno=%d: %s", sd, errno, strerror(errno));
            return ;
        }
        close(soc);
    }
    fclose(fp);
    return;
}

void request_w(char *filename)
{
    FILE *fp;
    int i, rec;
    char send_text[MAXLEN] = {};
    char r_buf[MAXLEN] = {};

    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("ERROR %d:openfile error!!!---cmd_write()\n", NOFILEOPEN);
        return ;
    }

    soc_con();

    // send request
    sprintf(send_text, "%%W");
    sd = send(soc, send_text, strlen(send_text), 0);
    if (sd == -1)
    {
        printf("sd = %d, errno=%d: %s", sd, errno, strerror(errno));
        return ;
    }
    while (1)
    {
        rec = recv(soc, r_buf, sizeof(r_buf), 0);
        if (rec == -1)
        {
            printf("rec = %d, errno=%d: %s", rec, errno, strerror(errno));
            return ;
        }
        if (strcmp(r_buf, "END") == 0)
        {
            break;
        }
        else
        {
            char tmp[MAXLEN] = "\0";
            strcpy(tmp, r_buf);
            fprintf(fp, "%s", tmp);
        }
    }

    //
    fclose(fp);
    printf("wrote %s\n", filename);
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
    }else{
        request(line);
    }
}

void exec_command(char *cmd, char *param)
{
    if (strcmp(cmd, "%Q") == 0 || strcmp(cmd, "%q") == 0)
    {
        printf("Bye.\n");
        exit(1);
    }
    else if (strcmp(cmd, "%C") == 0 || strcmp(cmd, "%c") == 0)
    {
        request(cmd);
    }
    else if (strcmp(cmd, "%P") == 0 || strcmp(cmd, "%p") == 0)
    {
        request_p(cmd, strtol(param, 0, 10));
    }
    else if (strcmp(cmd, "%R") == 0 || strcmp(cmd, "%r") == 0)
    {
        request_r(param);
    }
    else if (strcmp(cmd, "%W") == 0 || strcmp(cmd, "%w") == 0)
    {
        request_w(param);
    }
    else if (strcmp(cmd, "%H") == 0 || strcmp(cmd, "%h") == 0)
    {
        request(cmd);
    }
    else
    {
        printf("ERROR %d:%s command is not defined.--exec_command()\n", NOTDEFINED, cmd);
    }
}

void soc_con(){
    // socket
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc == -1)
    {
        printf("soc = %d, errno=%d: %s", soc, errno, strerror(errno));
        close(soc);
        exit(1);
    }

    //connect
    sa.sin_family = host->h_addrtype; //host address type
    sa.sin_port = htons(PORT_NO);     // port number
    bzero((char *)&sa.sin_addr, sizeof(sa.sin_addr));
    memcpy((char *)&sa.sin_addr, (char *)host->h_addr, host->h_length);

    con = connect(soc, (struct sockaddr *)&sa, sizeof(sa));
    if (con == -1)
    {
        printf("con = %d, errno=%d: %s", con, errno, strerror(errno));
        close(soc);
        exit(1);
    }
}