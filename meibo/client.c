#include "meibo_client.h"

int main(void)
{
    char s_buf[MAXLEN] = {};
    host = gethostbyname("localhost");
    if (host == NULL)
    {
        printf("hostError\n");
        return;
    }
    while (get_line(s_buf))
    {
        char tmp[MAXLEN] = "\0";
        strcpy(tmp, s_buf);
        parse_line(tmp);
    }
    return 0;
}