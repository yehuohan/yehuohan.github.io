
#include <iostream>
//#include <conio.h>
#include <thread>
#include <mutex>
#include <cstdio>

int				g_cnt = 0;
std::mutex		g_mtx;					// ������

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
		thrd_child[k] = new std::thread(tfunc, k);    // ��ֵ���ݲ���k

    for (int k = 0; k < 10; k++)
    {
        // ������ǰ�̣߳�ֱ�� *this ����ʶ���߳������ִ�У�
        // ��10���߳�ִ����ϣ�main�������̲߳Ż������
        thrd_child[k]->join();

        // �� thread �������ִ�е��̣߳�����ִ�ж�����ִ�У�
        // ��10���̻߳�ûִ����ϣ�main�����߳̿����Ѿ��˳��ˣ�
        // ���Կ��Կ�����tfunc���������
        //thrd_child[k]->detach();
    }

    //for (int k = 0; k < 10; k++)
    //    delete thrd_child[k];

	return 0;
}

