#!/usr/bin/python3

## @file loan_calculation.py
#  @brief 贷款计算，包括等额本息和等额本金
#  
#  @date 2017-07-21
#  @version
#  @author yehuohan, yehuohan@qq.com, yehuohan@gmail.com
#  @copyright


import sys


## @brief average capital plus interest
#  等额本息，每个月还款数额相同，一部分还利息，一部分还本金
#  @param A 本金，单位：元
#  @param ry 年利率，%
#  @param time 贷款时间，单位：年
#  @return 每月还款
#  @retval None
def calc_average_ci(A, ry, time_mouth):
    rate_mouth = ry/12;
    money_per_mouth = A * (rate_mouth * ((1 + rate_mouth) ** time_mouth)) / ((1+rate_mouth) ** time_mouth - 1)
    return money_per_mouth


## @brief average capital
#  等额本金，每月还的本金一样，这样本金逐渐减少，每月还的利息也会逐渐减少，
#  所以每个月还的数额不相同
#  @param A 本金，单位：元
#  @param ry 年利率，%
#  @param time 贷款时间，单位：年
#  @return 每月还款
#  @retval None
def calc_average_c(A, ry, time_mouth):
    rate_mouth = ry/12
    A_per_mouth = A / time_mouth
    money_all_mouth = ()
    k = 0
    count_sum = 0
    while (k < time_mouth):
        # 每月还款计算利息时，本金要减去已还的
        money_all_mouth += (A_per_mouth + (A - count_sum) * rate_mouth, )
        count_sum += A_per_mouth 
        k += 1
    return money_all_mouth


if __name__ == "__main__":
    A = 12000           # 单位：元
    rate_year = 11.88   # 单位：%
    time = 12           # 单位：月

    if len(sys.argv) == 1 + 3:
        A = sys.argv[2]
        rate_year = sys.argv[3]
        time = sys.argv[4]

    m1 = calc_average_ci(A, rate_year/100, time)
    m2 = calc_average_c(A, rate_year/100, time)

    print("\n贷款计算")
    print("本金  ：{}元".format(A))
    print("年利率：{}%".format(rate_year))
    print("时间  ：{}个月".format(rate_year))

    print("\n等额本息（单位：元）：")
    print("\t 每月还款： {}".format(m1))
    print("\t 总利息  ： {}".format(m1*time - A))
    print("\t 本息合计： {}".format(m1*time))

    print("\n贷款本金（单位：元）：")
    print("\t 每月还款： {}".format(m2))
    print("\t 总利息  ： {}".format(sum(m2) - A))
    print("\t 本息合计： {}".format(sum(m2)))
    print("\n")

