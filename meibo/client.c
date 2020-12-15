#include "meibo_client.h"

int main(void)
{
    char s_buf[MAXLEN] = {};

    while (get_line(s_buf))
    {
        parse_line(s_buf);
        s_buf[0] = '\0';
    }
    return 0;
}