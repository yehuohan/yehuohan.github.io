
import numpy as np

n = 2;
a = np.array([1, 2])
b = np.array([[1, 2],
              [1, 2]])

"""
c_i = \sum_j b_{ij}
"""
c1 = np.zeros(n)
for i in np.arange(n):
    for j in np.arange(n):
        c1[i] += b[i, j]
c2 = np.zeros(n)
for i in np.arange(n):
    c2[i] = np.sum(b[i])
c3 = np.sum(b, axis=1)      # 即以axis为自变量遍历进行sum
# print(c1)
# print(c2)
# print(c3)


"""
c_{ij} = a_i b_{ij}
\sum_i^n \sum_j^n c_{ij}

np.divide 与 np.multiply 类似
"""
c1 = np.zeros((n, n))
for i in np.arange(n):
    for j in np.arange(n):
        c1[i, j] = a[i] * b[i, j]
c2 = np.multiply(a.reshape(n, 1), b)    # i为行，a以i遍历，则a与b的行相同
# print(c1, np.sum(c1))
# print(c2, np.sum(c2))


"""
c_{ij} = a_j b_{ij}
\sum_i^n \sum_j^n c_{ij}
"""
c1 = np.zeros((n, n))
for i in np.arange(n):
    for j in np.arange(n):
        c1[i, j] = a[j] * b[i, j]
c2 = np.multiply(a.reshape(1, n), b)    # j为列，a以j遍历，则a与b的列相同
# print(c1, np.sum(c1))
# print(c2, np.sum(c2))


"""
c_i  = \sum_i \sum_j a_j b_{ij}
"""
c1 = np.zeros((n))
for i in np.arange(n):
    for j in np.arange(n):
        c1[i] += a[j] * b[i][j]
c2 = np.dot(a, b.T)                     # dot代替'求积累加'
# print(c1)
# print(c2)


"""
c = \sum_i a_i a_{n-i}
"""
c1 = 0
for i in np.arange(n):
        c1 += a[i] * a[n-i-1]
c2 = np.dot(a, a[::-1])
# print(c1)
# print(c2)


"""
c_{ij} = \sum_i \sum_j \sum_k b_{ik} b_{kj}
"""
c1 = np.zeros((n, n))
for i in np.arange(n):
    for j in np.arange(n):
        for k in np.arange(n):
            c1[i, j] += b[i, k] * b[k, j]
c2 = np.dot(b, b)
# print(c1)
# print(c2)
