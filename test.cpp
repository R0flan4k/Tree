#include <stdio.h>
#include <string.h>

int main(void)
{
    const char * str1 = "abobiks.dot";
    char str2[64] = "";
    strcpy(str2, str1);
    strncpy(strchr(str2, '.') + 1, "png", 3);
    printf("%s", str2);
    return 0;
}
