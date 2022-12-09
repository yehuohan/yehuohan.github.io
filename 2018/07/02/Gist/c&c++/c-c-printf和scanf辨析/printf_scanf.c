
#include <stdio.h>
#include <stdarg.h>

void v_scanf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    // 标准输入输出
    vscanf(format, args);

    va_end(args);
}

void v_printf(const char* format, ...)
{
    // vprintf, vfprintf, vsprintf类似
    va_list args;
    va_start(args, format);

    // 标准输入输出
    vprintf(format, args);

    va_end(args);
}

int main()
{
    int num;
    float var;
    char str[100];
    char input[] = "128 2.5 plus";
    char ouput[100];

    printf("This is ... parameters.\n");

    // 标准输入输出
    printf("scanf and printf:\n");
    scanf("%d %f %s", &num, &var, str);
    printf("num:%d, var:%f, str:%s\n\n", num, var, str);

    // 文件输入输出，以f开头
    printf("fscanf and fprintf:\n");
    fscanf(stdin, "%d %f %s", &num, &var, str);
    fprintf(stdout, "num:%d, var:%f, str:%s\n\n", num, var, str);

    // 字符串输入输出，以s开头
    printf("sscanf and sprintf:\n");
    sscanf(input, "%d %f %s", &num, &var, str);
    sprintf(ouput, "num:%d, var:%f, str:%s\n\n", num, var, str);
    printf("%s", ouput);

    // 以v开头的，需要传入va_list参数
    // vscanf, vfscanf, vsscanf类似
    // vprintf, vfprintf, vsprintf类似
    printf("\nThis is va_list parameters.\n");
    printf("vscanf and vprintf:\n");
    v_scanf("%d %f %s", &num, &var, str);
    v_printf("num:%d, var:%f, str:%s\n", num, var, str);

    while(1);
}

