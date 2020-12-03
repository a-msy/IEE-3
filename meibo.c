/*
 File: meibo.c
 Author: 09430509

 Created on 2019/04/10
 update on 2019/07/26
*/
include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIMIT 1024
#define maxsplit 5 //最大分割数
#define luck -1
#define over -2
#define endp NULL //strtol 用ポインタ
#define base1 10  //10進数
    typedef enum {
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
void cmd_write(char *filename);
void cmd_binread(char *filename);
void cmd_binwrite(char *filename);
void cmd_find(char *keyword);
void cmd_findb(char *keyword);
void swap_struct(struct profile *i, struct profile *j);
int compare_profile(struct profile *p1, struct profile *p2, int column);
int compare_date(struct date *d1, struct date *d2);
void cmd_sort(int youso);
void cmd_qsort(int youso);
int partition(int left, int right, int youso);
void quick_sort(int left, int right, int youso);
void cmd_delete(int param);
void cmd_help();
void cmd_size();
int find_kai(char *s, char *cp);
/*profile*/
struct profile *new_profile(struct profile *pro, char *str);
char *date_to_string(char buf[], struct date *date);
/*GLOBAL*/
struct profile profile_data_store[10000];
int profile_data_nitems = 0;
int quick_count = 0;
/*MAIN*/
int main(void)
{
    char line[LIMIT + 1];
    while (get_line(line))
    {
        parse_line(line);
    }
    return 0;
}

int subst(char *str, char c1, char c2)
{
    int count = 0;
    while (*str != ’\0’)
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
        if (*str == ’\0’)
        {
            break; //からもじなら抜ける
        }

        if (count > max)
            break;
        ret[count++] = str; //str をいじれば ret も変わるように分割後の文字列にはポインタを入れる

        while ((*str != ’\0’) && (*str != sep))
        { //区切り文字が見つかるまでポインタすすめる
            str++;
        }

        if (*str == ’\0’)
        {
            break; //区切り文字がなかったら抜ける＝文字列はそのまま
        }

        *str = ’\0’; //必ず区切り文字のはずだからくぎる
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
    fprintf(stderr, "\n>>>>>");

    if (fgets(input, LIMIT + 1, fp) == NULL)
    {
        fprintf(stderr, "ERROR %d:NULL--getline()\n", null);
        return 0; /* 失敗EOF */
    }
    subst(input, ’\n’, ’\0’);

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

    if (line[0] ==’%’)
    {
        com = split(line, ret,’ ’, 2);
        exec_command(ret[0], ret[1]);
    }
    else
    {
        new_profile(&profile_data_store[profile_data_nitems], line);
    }
}

void exec_command(char *cmd, char *param)
{
    if (strcmp(cmd, "%Q") == 0 || strcmp(cmd, "%q") == 0)
    {
        cmd_quit();
    }
    else if (strcmp(cmd, "%C") == 0 || strcmp(cmd, "%c") == 0)
    {
        cmd_check();
    }
    else if (strcmp(cmd, "%E") == 0 || strcmp(cmd, "%e") == 0)
    {
        cmd_pex(strtol(param, endp, base1));
    }
    else if (strcmp(cmd, "%P") == 0 || strcmp(cmd, "%p") == 0)
    {
        cmd_print(&profile_data_store[0], strtol(param, endp, base1));
    }
    else if (strcmp(cmd, "%R") == 0 || strcmp(cmd, "%r") == 0)
    {
        cmd_read(param);
    }
    else if (strcmp(cmd, "%W") == 0 || strcmp(cmd, "%w") == 0)
    {
        cmd_write(param);
    }
    else if (strcmp(cmd, "%F") == 0 || strcmp(cmd, "%f") == 0)
    {
        cmd_find(param);
    }
    else if (strcmp(cmd, "%FB") == 0 || strcmp(cmd, "%fb") == 0)
    {
        cmd_findb(param);
    }
    else if (strcmp(cmd, "%D") == 0 || strcmp(cmd, "%d") == 0)
    {
        cmd_delete(strtol(param, endp, base1));
    }
    else if (strcmp(cmd, "%S") == 0 || strcmp(cmd, "%s") == 0)
    {
        cmd_sort(strtol(param, endp, base1));
    }
    else if (strcmp(cmd, "%QS") == 0 || strcmp(cmd, "%qs") == 0)
    {
        cmd_qsort(strtol(param, endp, base1));
    }
    else if (strcmp(cmd, "%H") == 0 || strcmp(cmd, "%h") == 0)
    {
        cmd_help();
    }
    else if (strcmp(cmd, "%BW") == 0 || strcmp(cmd, "%bw") == 0)
    {
        cmd_binwrite(param);
    }
    else if (strcmp(cmd, "%BR") == 0 || strcmp(cmd, "br") == 0)
    {
        cmd_binread(param);
    }
    else if (strcmp(cmd, "%SIZE") == 0 || strcmp(cmd, "size") == 0)
    {
        cmd_size();
    }
    else
    {
        fprintf(stderr, "ERROR %d:%s command is not defined.--exec_command()\n", NOTDEFINED, cmd);
        fprintf(stderr, "command list : %%H\n");
    }
}

void cmd_help()
{
    int i;
    char help_list[LIST][40] =
        {
            "Q : quit system",
            "C : check data num",
            "P [value] : print data",
            "E : print specified data",
            "R [filename] : read csv data",
            "W [filename] : write csv data",
            "BR : read binary data",
            "BW : write binary data",
            "F [word] : Exact match search",
            "FB [word] : Partial match search",
            "S [value] : sort (bubble)",
            "QS [value] : quick sort",
            "D [value] : delete data",
            "SIZE : size check",
        };

    for (i = 0; i < LIST; i++)
    {
        fprintf(stderr, "%s\n", help_list[i]);
    }

    return;
}

void cmd_quit()
{
    fprintf(stderr, "Are you sure you want to quit?(y or n)>>>");
    if (fgetc(stdin) ==’y’)
    {
        fprintf(stderr, "END SYSTEM.\n");
        exit(0);
    }
    else
    {
        return;
    }
}

void cmd_check()
{
    fprintf(stdout, "%d profile(s)\n", profile_data_nitems);
    return;
}

void cmd_print(struct profile *pro, int param)
{
    if (profile_data_nitems == 0)
    {
        fprintf(stderr, "ERROR %d:No record. No print.--cmd_print()\n", NORECORD);
        return;
    }
    int i;

    if (param == 0)
    { //０のとき
        fprintf(stderr, "******print record data******\n");
        for (i = 0; i < profile_data_nitems; i++)
        {
            printdata(pro + i, i);
        }
    }

    else if (param > 0)
    { //正のとき

        if (param > profile_data_nitems)
        {
            fprintf(stderr, "ERROR %d:over number of record.--cmd_print()\n", OVERNUMBERRECORD);
            fprintf(stderr, "ERROR %d:number of item is %d\n", NUMITEM, profile_data_nitems);
            return;
        }
        fprintf(stderr, "******print record data******\n");
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
            fprintf(stderr, "ERROR %d:over number of record.--cmd_print()\n", OVERNUMBERRECORD);
            fprintf(stderr, "ERROR %d:number of item is %d\n", NUMITEM, profile_data_nitems);
            return;
        }
        pro += profile_data_nitems - param;
        fprintf(stderr, "******print record data******\n");
        for (i = 0; i < param; i++)
        {
            printdata(pro + i, profile_data_nitems - param + i);
        }
    }
    return;
}

void printdata(struct profile *pro, int i)
{
    fprintf(stderr, "data : %5d ------------------------------\n", i + 1);
    fprintf(stdout, "Id : %d\n", pro->id);
    fprintf(stdout, "Name : %s\n", pro->name);
    fprintf(stdout, "Birth : %04d-%02d-%02d\n", pro->found.y, pro->found.m, pro->found.d);
    fprintf(stdout, "Addr : %s\n", pro->add);
    fprintf(stdout, "Com. : %s\n\n", pro->others);
    fprintf(stderr, "--------------------------------------------\n");
}

void cmd_pex(int param)
{
    if (profile_data_nitems == 0 || param == 0)
    {
        fprintf(stderr, "ERROR %d:No record. No print.--cmd_print()\n", NORECORD);
        return;
    }

    if (param < 0)
    {
        param *= (-1);
    }

    if (param > profile_data_nitems)
    {
        fprintf(stderr, "ERROR %d:over number of record.--cmd_print()\n", OVERNUMBERRECORD);
        fprintf(stderr, "ERROR %d:number of item is %d\n", NUMITEM, profile_data_nitems);
        return;
    }
    param -= 1;
    printdata(&profile_data_store[param], param);
    return;
}

void cmd_read(char *filename)
{
    char line[LIMIT + 1];
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "ERROR %d:openfile error!!!---cmd_read()\n", NOFILEOPEN);
        return;
    }
    while (get_line_fp(fp, line))
    {
        parse_line(line);
    }
    fclose(fp);
    return;
}

void cmd_write(char *filename)
{
    FILE *fp;
    int i;
    if ((fp = fopen(filename, "w")) == NULL)
    {
        fprintf(stderr, "ERROR %d:openfile error!!!---cmd_write()\n", NOFILEOPEN);
        return;
    }
    for (i = 0; i < profile_data_nitems; i++)
    {
        fprintf(fp, "%d,", profile_data_store[i].id);
        fprintf(fp, "%s,", profile_data_store[i].name);
        fprintf(fp, "%04d-%02d-%02d,", profile_data_store[i].found.y, profile_data_store[i].found.m, profile_data_store[i].found.d);
        fprintf(fp, "%s,", profile_data_store[i].add);
        fprintf(fp, "%s", profile_data_store[i].others);
        fprintf(fp, "\n");
    }
    fclose(fp);
    fprintf(stderr, "wrote %s\n", filename);
    return;
}

void cmd_binread(char *filename)
{
    return;
}

void cmd_binwrite(char *filename)
{
    FILE *fp;
    int i = 0;
    if (fopen(filename, "wb") == NULL)
    {
        fprintf(stderr, "ERROR %d:openfile error!!!---cmd_write()\n", NOFILEOPEN);
        return;
    }
    fwrite(&profile_data_store[0], sizeof(struct profile), 1, fp);
    fclose(fp);
    fprintf(stderr, "wrote %s\n", filename);
    return;
}

char *date_to_string(char buf[], struct date *date)
{
    sprintf(buf, "%04d-%02d-%02d", date->y, date->m, date->d);
    return buf;
}

void cmd_find(char *keyword)
{
    int i, check = 0;
    struct profile *p;
    char found_str[11];

    for (i = 0; i < profile_data_nitems; i++)
    {
        p = &profile_data_store[i];
        date_to_string(found_str, &p->found);
        if (
            (p->id) == strtol(keyword, endp, base1) ||
            strcmp(p->name, keyword) == 0 ||
            strcmp(p->add, keyword) == 0 ||
            strcmp(p->others, keyword) == 0 ||
            strcmp(found_str, keyword) == 0)
        {
            printdata(p, i);
            check = 1;
        }
    }

    if (check == 0)
    {
        fprintf(stderr, "No match data.\n");
    }

    return;
}

void cmd_findb(char *keyword)
{
    int i, check = 0;
    struct profile *p;
    char found_str[11];

    for (i = 0; i < profile_data_nitems; i++)
    {
        p = &profile_data_store[i];
        date_to_string(found_str, &p->found);
        if (
            (p->id) == strtol(keyword, endp, base1) ||
            find_kai(p->name, keyword) == 0 ||
            find_kai(p->add, keyword) == 0 ||
            find_kai(p->others, keyword) == 0 ||
            find_kai(found_str, keyword) == 0)
        {
            printdata(p, i);
            check = 1;
        }
    }
    if (check == 0)
    {
        fprintf(stderr, "No match data.\n");
    }
    return;
}

int find_kai(char *s, char *cp)
{
    char *s1, *s2;
    if (*cp == ’\0’)
        return 1; /*cp の文字列長が 0 なら s を返す*/

    while (*s != ’\0’)
    {
        while (*s != ’\0’ && *s != *cp)
        { /*先頭文字が合うまで探す*/
            s++;
        }
        if (*s == ’\0’)
            return 1; /*見つからない*/
        s1 = s;
        s2 = cp;
        while (*s1 == *s2 && *s1 != ’\0’)
        { /*cp の先頭以降の文字列が一致するか*/
            s1++;
            s2++;
        }
        if (*s2 == ’\0’)
        { /* cp の文字列は，全て一致した*/

            return 0;
        }
        s++; /*次の位置から，調べ直す*/
    }
    return 1; /*見つからない*/
}

void swap_struct(struct profile *i, struct profile *j)
{
    struct profile temp;

    temp = *j;
    *j = *i;
    *i = temp;

    return;
}

int compare_profile(struct profile *p1, struct profile *p2, int youso)
{
    if (youso < 0)
        youso *= -1;
    switch (youso)
    {
    case 1:
        return (p1->id) - (p2->id);
        break;

    case 2:
        return strcmp(p1->name, p2->name);
        break;

    case 3:
        return compare_date(&p1->found, &p2->found);
        break;

    case 4:
        return strcmp(p1->add, p2->add);
        break;

    case 5:
        return strcmp(p1->others, p2->others);
        break;

    default:
        return 0;
        break;
    }
}

int compare_date(struct date *d1, struct date *d2)
{
    if (d1->y != d2->y)
        return d1->y - d2->y;
    if (d1->m != d2->m)
        return d1->m - d2->m;
    return (d1->d) - (d2->d);
}

void cmd_sort(int youso)
{
    int i, j;
    int check = 0;

    if (youso > 5 || youso < 1)
    {
        fprintf(stderr, "ERROR %d:sort param is 1 to 5.---cmd_sort()\n", PARAMERROR);
        return;
    }

    if (profile_data_nitems <= 0)
    {
        return;
    }

    for (i = 0; i < profile_data_nitems; i++)
    {
        for (j = 0; j < profile_data_nitems - 1; j++)
        {
            if (compare_profile(&profile_data_store[j], &profile_data_store[j + 1], youso) > 0)
            {
                swap_struct(&profile_data_store[j], &profile_data_store[j + 1]);
                check++;
            }
        }
    }
    fprintf(stderr, "%d swap.\n", check);
    return;
}

void cmd_qsort(int youso)
{
    if (youso > 5 || youso < 1)
    {
        fprintf(stderr, "ERROR %d:sort param is 1 to 5.---cmd_sort()\n", PARAMERROR);
        return;
    }

    if (profile_data_nitems <= 0)
    {
        return;
    }
    quick_sort(0, profile_data_nitems - 1, youso);
    fprintf(stderr, "quicksort end.===count:%d\n", quick_count);
    quick_count = 0;
    return;
}

void quick_sort(int left, int right, int youso)
{
    int i, j, pivot;

    i = left;
    j = right;
    pivot = right;

    while (1)
    {
        while (compare_profile(&profile_data_store[i], &profile_data_store[pivot], youso) < 0)
        {
            i++;
        }
        while (compare_profile(&profile_data_store[pivot], &profile_data_store[j], youso) < 0)
        {
            j++;
        }
        if (i >= j)
            break;
        swap_struct(&profile_data_store[i], &profile_data_store[j]);
        quick_count++;
        i++;
        j--;
    }

    if (left < i - 1)
    {                                   /* 基準値の左に 2 以上要素があれば */
        quick_sort(left, i - 1, youso); /* 左の配列をソートする */
    }
    if (j + 1 < right)
    {                                    /* 基準値の右に 2 以上要素があれば */
        quick_sort(j + 1, right, youso); /* 右の配列をソートする */
    }
    return;
}

void cmd_delete(int param)
{
    int i;
    if (param > profile_data_nitems || param <= 0)
    {
        fprintf(stderr, "ERROR %d:error param.--cmd_delete()\n", OVERNUMBERRECORD);
        fprintf(stderr, "ERROR %d:number of item is %d\n", NUMITEM, profile_data_nitems);
        return;
    }
    for (i = param - 1; i < profile_data_nitems - 1; i++)
    {
        profile_data_store[i] = profile_data_store[i + 1];
    }
    profile_data_nitems -= 1;
    return;
}

struct profile *new_profile(struct profile *pro, char *str)
{
    char *ret1[maxsplit], *ret2[maxsplit - 2];
    int count = 0;
    if (profile_data_nitems >= 10000)
    {
        fprintf(stderr, "ERROR %d:Can’t add record--new_profile()\n", OVERNITEMS);
        return NULL;
    }
    count = split(str, ret1,’,’, maxsplit);
    if (count != maxsplit)
    {
        error_split(count);
        fprintf(stderr, "ERROR %d:wrong format of input(ex.001,name,1999-01-01,address,other)--new_profile()\n", FORMATINPUT);
        return NULL;
    } //文字列用

    pro->id = strtol(ret1[0], endp, base1);
    if (pro->id == 0)
    {
        fprintf(stderr, "ERROR %d:ID is NUMBER.--new_profile()\n", FORMATID);
        return NULL;
    }

    strncpy(pro->name, ret1[1], 70); //名前のコピー
    strncpy(pro->add, ret1[3], 70);  //住所
    pro->others = (char *)malloc(sizeof(char) * (strlen(ret1[4]) + 1));
    strcpy(pro->others, ret1[4]); //備考,MAX 1024bytes

    if (split(ret1[2], ret2,’-’, maxsplit - 2) != maxsplit - 2)
    {
        fprintf(stderr, "ERROR %d:wrong format of date.(ex.1999-01-01)--new_profile()\n", FORMATDATE);
        return NULL;
    } //設立日
    pro->found.y = strtol(ret2[0], endp, base1);
    pro->found.m = strtol(ret2[1], endp, base1);
    pro->found.d = strtol(ret2[2], endp, base1);

    fprintf(stderr, "Add profile.\n");
    profile_data_nitems++;
    return pro;
}

void cmd_size()
{
    fprintf(stderr, "struct profile = %d\n", sizeof(struct profile));
    fprintf(stderr, "id = %d\n", sizeof(profile_data_store[0].id));
    fprintf(stderr, "name = %d\n", sizeof(profile_data_store[0].name));
    fprintf(stderr, "add = %d\n", sizeof(profile_data_store[0].add));
    fprintf(stderr, "found = %d\n", sizeof(profile_data_store[0].found));
    fprintf(stderr, "found.y = %d\n", sizeof(profile_data_store[0].found.y));
    fprintf(stderr, "found.m = %d\n", sizeof(profile_data_store[0].found.m));
    fprintf(stderr, "found.d = %d\n", sizeof(profile_data_store[0].found.d));
    fprintf(stderr, "Com. = %d\n", sizeof(profile_data_store[0].others));
    return;
    //araiment seiyaku
}