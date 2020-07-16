



#include <stdio.h>
#include <malloc.h>

/*!
 * @brief 迭代法
 *
 *    高位 -> 低位
 * n: 0001 1101 0100
 * 每次判断bit0是否为1，然后n向右移一位，直至n=0；
 * 循环次数为n的二进制有效长度。
 *
 */
int count_bits_iteration(unsigned int n)
{
    int cnt = 0;
    while(n)
    {
        cnt += n & 0x1u;
        n >>= 1;
    }
    return cnt;
}

/*!
 * @brief 稀疏向量（二进制中'0'的比重比较小）
 *
 *         高位 -> 低位
 * n       : ****1000
 * n-1     : ****0111
 * n&(n-1) : ****0000
 *
 * n-1相对于n，高4位****不会有变化；
 * 且n与上n-1，低4位均变成了0；
 * 这样，对低4位'1'的计数，只需要一次循环。
 *
 * 所以，循环的次数，等于n中'1'的个数。
 * 例子(n的变化过程)：
 * 110011 -> 110010 -> 110000 -> 100000 -> 000000
 *
 */
int count_bits_sparse(unsigned int n)
{
    int cnt = 0;
    while(n)
    {
        cnt ++;
        n &= (n-1);
    }
    return cnt;
}

/*!
 * @brief 稠密向量（二进制中'0'的比重比较大）
 *
 * 与count_bits_sparse类似；
 * 但是，循环的次数，等于n中'0'的个数。
 *
 */
int count_bits_dense(unsigned int n)
{
    int cnt = 8 * sizeof(int);  // 一个字节最多8个'1'
    n ^= (unsigned int)-1;      // (uint)-1 = 0xFFFFFFFF
                                // n为0的位，变成1
                                // n为1的位，变成0
                                // 相当于 n = ~n
    while (n)
    {
        cnt --;
        n &= (n-1);
    }
    return cnt;
}

/** 8bits表 */
static char bits_in_8bits[0xffu];
/** 16bits表 */
static char bits_in_16bits[0xffffu];
/** 初始化bits表 */
void init_bits_table(char bit[] , unsigned int n)
{
    for (unsigned int k = 0; k < n; k ++)
        bit[k] = count_bits_sparse(k);
}

/*!
 * @brief 预计算法，即打表法
 * 32bit共4个8bit，将每个8bit的'1'的个数相加即可。
 */
int count_bits_precompute8bit(unsigned int n)
{
    return bits_in_8bits[n       & 0xffu]
         + bits_in_8bits[(n>>8)  & 0xffu]
         + bits_in_8bits[(n>>16) & 0xffu]
         + bits_in_8bits[(n>>24) & 0xffu];

}

/*!
 * @brief 预计算法，即打表法
 * 32bit共2个16bit，将每个16bit的'1'的个数相加即可。
 */
int count_bits_precompute16bit(unsigned int n)
{
    return bits_in_16bits[n       & 0xffffu]
         + bits_in_16bits[(n>>16) & 0xffffu];
}

/*!
 * @brief 并行计算
 *
 * (1)MASK作用解释（用CB表示CountBits）：
 *
 * CB(1) = 1&1 = 1
 * 二进制1，有1个'1'，且CB结果用1个bit即可存储；
 *
 * CB(11) = CB(1)&1 + CB(1)&1 = 10
 * 二进制11，有2个'1'，且CB结果用2个bit即可存储；
 *
 * CB(1111) = CB(11)&11 + CB(11)&11 = 100
 * 二进制1111，有4个'1'，且CB结果用4个bit即可存储；
 *
 * CB(11111111) = CB(1111)&1111 + CB(1111)&1111 = 1000
 * 二进制11111111，有4个'1'，且CB结果用8个bit即可存储；
 *
 * 依次类推，可一直计算下去。
 *
 * (2)最后 n%255，结果等同于将n的256进制的各个数位（即8bit）的值相加；
 * 即 n%255 即是将n的4个8bit的CB值相加。
 *
 * 对 n%255 的简单解释：
 * 利用取模性质(以10进制为例)
 * (a+b) % m = (a%m + b%m) % m
 * (a-b) % m = (a%m - b%m) % m
 * (a*b) % m = (a%m * b%m) % m
 *
 * 将1723表示成a=1,b-7,c=2,d=3
 * 由取模性质可知
 * (10^3) = (10%9 * 10%9 * 10%9 ) % 9 = 1
 * 则有
 * 1723%9 = (a*10^3%9 + b*10^2%9 + c*10^1%9 + d*10^0%9) % 9
 *        = (a%9 + b%9 + c%9 + d%9) % 9
 *        = (a + b + c + d) % 9
 */
int count_bits_nifty_parallel(unsigned int n)
{
#define MASK_01010101 (((unsigned int)(-1))/3)
#define MASK_00110011 (((unsigned int)(-1))/5)
#define MASK_00001111 (((unsigned int)(-1))/17)
    n = (n & MASK_01010101) + ((n>>1) & MASK_01010101);
    n = (n & MASK_00110011) + ((n>>2) & MASK_00110011);
    n = (n & MASK_00001111) + ((n>>4) & MASK_00001111);
    return n % 255;
}

/*!
 * @brief 并行计算
 * count_bits_parallel 与 count_bits_nifty_parallel 原理一样；
 * 只是MASK是通过计算而来。
 */
int count_bits_parallel(unsigned int n)
{
#define TWO(c)     (0x1u << (c))
#define MASK(c)    (((unsigned int)(-1)) / (TWO(TWO(c)) + 1u))
#define COUNT(x,c) ((x) & MASK(c)) + (((x) >> (TWO(c))) & MASK(c))
    n = COUNT(n, 0);
    n = COUNT(n, 1);
    n = COUNT(n, 2);
    n = COUNT(n, 3);
    n = COUNT(n, 4);
    //n = COUNT(n, 5);  // for 64 bit
    return n;
#undef TWO
#undef MASK
#undef COUNT
}

/*!
 * @brief MIT算法
 *
 * 原理与count_bits_nifty_parallel类似
 *
 * 033333333333 = 11011011011011011011011011 011 011
 * 011111111111 = 01001001001001001001001001 001 001
 * 030707070707 = 11000111000111000111000111 000 111
 *
 * 将n分割，每部分3bit，经过以下计算：
 * n - ((n>>1) & 033333333333) - ((n>>2) & 011111111111);
 * 每部分3bit的值即为CB的结果（用CB表示CountBits）
 * 例如(以二进制显示)：
 * CB(111) = 111 - 011&011 + 001&001 = 011
 *
 * 最后一步，即是将每 6bit 的 高3bit 和 低3bit 的CB值相加。
 * ((tmp + (tmp >> 3)) & 030707070707) % 63;
 *
 */
int count_bits_mit(unsigned int n)
{
    // 只支持32bit
    unsigned int tmp;
    tmp = n - ((n>>1) & 033333333333)
            - ((n>>2) & 011111111111);
    return ((tmp + (tmp >> 3)) & 030707070707) % 63;
}

/*!
 * @brief 计算[0, num)所有数的CountBits
 *
 * bit[]数组的长度为num。
 *
 *
 * bits[0] = 0;
 * bits[1] = 1;
 * bits[k] = bits[k >> 1] + bits[k & 1];
 *
 * 对k >> 1的理解：
 * 向右移一位，'1'的个数不变；
 * 向右移一位即除2，最终的结果不是0就是1，所以要先给rcb[0]和rcb[1]赋值；
 *
 * 对k & 1的理解：
 * 向右移一位，则最低的一位将丢弃，如果丢弃的是'1'，则要加上；
 */
void count_num_bits(unsigned int bits[], unsigned int num)
{
    bits[0] = 0;
    bits[1] = 1;
    for (unsigned int k = 2; k < num; k ++)
        bits[k] = bits[k >> 1] + bits[k & 1];
}

int main(void)
{
    init_bits_table(bits_in_8bits, 0xffu);
    init_bits_table(bits_in_16bits, 0xffffu);

    unsigned int n = 398127982;
    printf("n = %d : %d\n", n, count_bits_iteration(n));
    printf("n = %d : %d\n", n, count_bits_sparse(n));
    printf("n = %d : %d\n", n, count_bits_dense(n));
    printf("n = %d : %d\n", n, count_bits_precompute8bit(n));
    printf("n = %d : %d\n", n, count_bits_precompute16bit(n));
    printf("n = %d : %d\n", n, count_bits_nifty_parallel(n));
    printf("n = %d : %d\n", n, count_bits_parallel(n));
    printf("n = %d : %d\n", n, count_bits_mit(n));

    unsigned int bits[100];
    count_num_bits(bits, 100);
    for (int k = 0; k < 100; k ++)
    {
        printf(" %3d:%2d ", k, bits[k]);
        if ((k+1) % 15 == 0)
            printf("\n");
    }

    return 0;
}

