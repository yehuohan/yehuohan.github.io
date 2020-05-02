
#include <stdio.h>
#include <stdarg.h>

/*
宏 : 本质是文本替换，替换的终止条件是：文件中不再含有宏。而当宏参数是另一个宏的
     时候，凡宏定义里有用'#'或'##'的地方宏参数是不会再展开， 所以有#或##的宏需
     要增加一层转换宏。
#  : 将参数转成字符串。
## : 连接符号，将参数连接在一起。
*/
#define TEST        hello
#define _TO_STR(s)  #s          // 转换宏
#define TO_STR(s)   _TO_STR(s)

#define _JOIN(s)   #s
#define JOIN(a, b)  ("String " _JOIN(a##b))   // 字符串可以直接并在一起

// 可变参数 __VA_ARGS__ 需要用 ## 连接，防止可变参数个数为0个
#define LOG(fmt, ...) printf(fmt, ## __VA_ARGS__)

/*
c/c++中预定义的宏：
__FILE__     : 文件名
__FUNCTION__ : 函数名
__LINE__     : 行号
__DATE__     : 日期
__TIME__     : 时间
__STDC__     : 当要求程序严格遵循ANSIC标准时该标识符被赋值为1
__cplusplus  : 当用C++编译程序编译时，__cplusplus就会被定义
*/
#define debug(fmt, ...) LOG("DEBUG %s(L%d) " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__)

int main()
{
    // TO_STR(TEST)第一次展开为：_TO_STR(hello)，然后再展开为 #hello，
    // 之后hello即使是宏也不会再展开，而是用#将参数hello转成了字符串
    printf(TO_STR(a'b'c\n));    // "a'b'c\n"
    printf(TO_STR(TEST));       // "hello"
    printf(_TO_STR(a'b'c\n));   // "a'b'c\n"
    printf(_TO_STR(TEST));      // "TEST"
    printf("\n");

    printf(JOIN(TEST, abf));   // "String TESTabc"
    printf("\n");

    LOG("log\n");
    LOG("log %d\n", 102);

    debug("Error: %d \n", 1030);
    printf("%s\n", __FILE__);
    printf("%s\n", __FUNCTION__);
    printf("%d\n", __LINE__);
    printf("%s\n", __DATE__);
    printf("%s\n", __TIME__);
    printf("%d\n", __STDC__);

    return 0;
}
