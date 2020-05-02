
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <linux/hidraw.h>           // hidraw设备操作
#include <linux/kernel.h>
#include <linux/input.h>            // input设备中断处理
#include <sys/ioctl.h>
#include <sys/inotify.h>            // inotify文件系统状态变化通知机制
#include <sys/poll.h>               // poll机制，用于应用程序查询设备资源
#include <sys/types.h>

int loop = 1;

void sig_cb(int sig)
{
    loop = 0;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_cb);     // Handle ctrl-c

    const char* hiddev = "/dev/hidraw1";
    unsigned char   buf[1024];
    struct pollfd   pfd;
    struct hidraw_devinfo   hidinfo;
    int fd, res, k;

    // Open device.
    fd = open(hiddev, O_RDWR);
    if (fd < 0)
    {
        printf("Can not open %s.\n", hiddev);
        return 0;
    }

    // Check pid,vid.
    res = ioctl(fd, HIDIOCGRAWINFO, &hidinfo);
    if (res == 0)
    {
        printf("dev bustype: %x", hidinfo.bustype);
        printf("dev     vid: %x", hidinfo.vendor);
        printf("dev     pid: %x", hidinfo.product);
    }
    else
        perror("HIDIOCGRAWINFO");

    // Listening drive event of hidraw
    pfd.fd = fd;
    pfd.events = POLLIN | POLLHUP | POLLNVAL | POLLERR ;
    while (loop)
    {
        poll(&pfd, 1, 8);               // Check event
        if (pfd.revents & POLLIN)
        {
            // Read
            res = read(pfd.fd, buf, sizeof(buf));
            if (res > 0)
            {
                printf("Received message size: %d", res);
                for (k = 0; k < res; k ++)
                    printf("\t\tbuf[%3d]: %x", k, buf[k]);
            }

            // Send
            usleep(100000);
            res = write(pfd.fd, buf, res);
            if (res > 0)
            {
                printf("Send message size: %d", res);
                for (k = 0; k < res; k ++)
                    printf("\t\tbuf[%3d]: %x", k, buf[k]);
            }
        }
    }

    close(fd);
    printf("Terminated.");

    return 0;
}
