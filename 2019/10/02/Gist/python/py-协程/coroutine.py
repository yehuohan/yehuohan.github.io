#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#%% generator with next and send
def foo():
    cnt = 100
    print('foo start')
    while True:
        cnt += 1
        res = yield cnt
        print('res: ', res)

f = foo()           # 带yield的函数不会立即执行，而是返回一个generator
print('step 1')
print(next(f))      # 调用next，开始执行foo函数
                    # 遇到yield时暂停执行，并返回cnt，且没有对res赋值
print('step 2')
print(next(f))      # 再次调用next，从上一次yield处继续执行
                    # 而res仍不会赋值（所以为None），因为cnt被yield返回了
print('step 3')
print(f.send(3))    # 调用send，从上一次yeild处继续执行
                    # 同时通将send传入一个参数，yield接收参数并赋给res


#%% producer-consumer model with generator
def consumer():
    r = ''
    while True:
        n = yield r         # cosume来自producer的数据
        if not n:
            return
        print('[Consumer] consuming {}'.format(n))
        r = 'OK'            # 完成一次生产消费过程

def producer(c):
    next(c)
    n = 0
    while n < 5:
        n += 1
        print('[Producer] producing {}'.format(n))
        print(c.send(n))    # produce数据给consumer
    c.close()

c = consumer()
producer(c)

#%% asyncio(python3.4+) for coroutine
# coroutine是协程，也叫微线程
# 协程的本质是一个线程，在一个线程中实现多任务的并发
# 单线程内开启协程，当遇到耗时IO时，由应用程序控制多任务的上下文切换
# 而python的线程由系统调度，遇到耗时IO时，会切换到其它线程运行
import asyncio
import threading

@asyncio.coroutine
def test(Id, T):
    print('step 1 task %s in %s' % (Id, threading.currentThread()))
    r = yield from asyncio.sleep(T)
    print('step 2 task %s in %s' % (Id, threading.currentThread()))

# tasks中是两个coroutine，在一个线程中通过coroutine实现并发（异步IO）
# task B先执行，但其IO耗时更长，故task A会先执行完毕
loop = asyncio.get_event_loop()
tasks = [test('A', 0.1), test('B', 0.5)]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()

#%% async/await(python3.5+) for coroutine
import asyncio
import threading
async def coroutine(Id, T):
    print('step 1 task %s in %s' % (Id, threading.currentThread()))
    r = await asyncio.sleep(T)
    print('step 2 task %s in %s' % (Id, threading.currentThread()))

loop = asyncio.get_event_loop()
tasks = [coroutine('A', 0.1), coroutine('B', 0.5)]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
