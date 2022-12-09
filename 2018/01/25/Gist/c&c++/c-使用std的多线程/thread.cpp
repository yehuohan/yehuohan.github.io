
#include <iostream>
//#include <conio.h>
#include <thread>
#include <mutex>
#include <cstdio>

int				g_cnt = 0;
std::mutex		g_mtx;					// 互斥量

void tfunc(int k)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

    g_mtx.lock();
	g_cnt++;
    std::cout << "k: " << k << "\t\tg_cnt: " << g_cnt << std::endl;
    g_mtx.unlock();
}

int main()
{
    std::thread* thrd_child[10];

    for (int k = 0; k < 10; k++)
		thrd_child[k] = new std::thread(tfunc, k);    // 按值传递参数k

    for (int k = 0; k < 10; k++)
    {
        // 阻塞当前线程，直至 *this 所标识的线程完成其执行；
        // 即10个线程执行完毕，main所在主线程才会结束。
        thrd_child[k]->join();

        // 从 thread 对象分离执行的线程，允许执行独立地执行；
        // 即10个线程还没执行完毕，main所在线程可能已经退出了；
        // 所以可以看不到tfunc的输出内容
        //thrd_child[k]->detach();
    }

    //for (int k = 0; k < 10; k++)
    //    delete thrd_child[k];

	return 0;
}

