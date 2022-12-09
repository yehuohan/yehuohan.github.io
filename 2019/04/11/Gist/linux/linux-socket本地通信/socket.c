
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <pthread.h>
#include <error.h>
#include <stddef.h>

int server_start_tcp(const char* name, int size)
{
    int sfd, cfd, len;
    struct sockaddr_un un;

    /* create server socket of unix */
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return -1;

    /* set listen address */
    unlink(name); /* in case it already exists*/
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    memcpy(un.sun_path, name, size);
    len = offsetof(struct sockaddr_un, sun_path) + size;

    /* bind to address */
    if (bind(sfd, (struct sockaddr*)&un, len) < 0)
    {
        close(sfd);
        return -2;
    }

    /* listen to sfd */
    if (listen(sfd, 10) < 0)
    {
        close(sfd);
        return -3;
    }

    /* accept client connection */
    len = sizeof(un);
    if ((cfd = accept(sfd, (struct sockaddr*)&un, &len)) < 0)
        return -1;

    const char* str = "Hello monday.\0";
    char        buf[1024];
    int         res, cnt = 0;

    while (1)
    {
        cnt ++;
        printf("%3d-th loop\n\t", cnt);

        res = read(cfd, buf, sizeof(buf));
        for (int k = 0; k < res; k ++)
            printf("%c", buf[k]);
        printf("\n\t");

        res = write(cfd, str, strlen(str));
        for (int k = 0; k < res; k ++)
            printf("%c", str[k]);
        printf("\n");

        sleep(1);
    }

    close(sfd);
    return 0;
}

int server_udp(const char* name, int size)
{
    int fd, len, cli_len;
    struct sockaddr_un un, cli;

    /* create server socket of unix */
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("[server_udp] socket\n");
        return -1;
    }

    /* bind to address */
    unlink(name);
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    memcpy(un.sun_path, name, size);
    len = offsetof(struct sockaddr_un, sun_path) + size;
    if (bind(fd, (struct sockaddr*)&un, len) < 0)
    {
        perror("[server_udp] bind\n");
        close(fd);
        return -1;
    }

    const char* str = "Hello sunday.\0";
    char        buf[1024];
    int         res, cnt = 0;

    while (1)
    {
        cnt ++;
        printf("%3d-th loop\n", cnt);

        res = recvfrom(fd, buf, sizeof(buf), 0, (const struct sockaddr*)&cli, &cli_len);
        if (res < 0)
        {
            perror("\t[server_udp] recvfrom\n");
            close(fd);
            return -1;
        }
        printf("\t[server_udp]: msg recv from %s\n\t", cli.sun_path);
        for (int k = 0; k < res; k ++)
            printf("%c", buf[k]);
        printf("\n");

        res = sendto(fd, str, strlen(str), 0, (const struct sockaddr*)&cli, cli_len);
        if (res < 0)
        {
            perror("\t[server_udp] sendto\n");
            close(fd);
            return -1;
        }
        printf("\t[server_udp]: msg send to %s\n\t", cli.sun_path);
        for (int k = 0; k < res; k ++)
            printf("%c", str[k]);
        printf("\n");

        sleep(1);
    }

    close(fd);
    return 0;
}

int client_connect_tcp(const char* name, int size)
{
    int cfd, len;
    struct sockaddr_un un;

    /* create client socket of unix */
    if ((cfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return -1;

    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    memcpy(un.sun_path, name, size);
    len = offsetof(struct sockaddr_un, sun_path) + size;

    /* connect to server socket */
    if (connect(cfd, (struct sockaddr*)&un, len) < 0)
    {
        close(cfd);
        return -2;
    }

    const char* str = "Hello sunday.\0";
    char        buf[1024];
    int         res, cnt = 0;

    while(1)
    {
        cnt ++;
        printf("%3d-th loop\n\t", cnt);

        res = write(cfd, str, strlen(str));
        for (int k = 0; k < res; k ++)
            printf("%c", str[k]);
        printf("\n\t");

        res = read(cfd, buf, sizeof(buf));
        for (int k = 0; k < res; k ++)
            printf("%c", buf[k]);
        printf("\n");

        sleep(1);
    }

    close(cfd);
    return 0;
}

int client_udp(const char* name, int size, const char* server, int server_size)
{
    int fd, len, ser_len;
    struct sockaddr_un un, ser;

    /* create client socket of unix */
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("[client_udp] socket\n");
        return -1;
    }

    /* bind to address */
    unlink(name);
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    memcpy(un.sun_path, name, size);
    len = offsetof(struct sockaddr_un, sun_path) + size;
    if (bind(fd, (struct sockaddr*)&un, len) < 0)
    {
        perror("[client_udp] bind\n");
        close(fd);
        return -1;
    }

    /* server address */
    memset(&ser, 0, sizeof(ser));
    ser.sun_family = AF_UNIX;
    memcpy(ser.sun_path, server, server_size);
    ser_len = offsetof(struct sockaddr_un, sun_path) + server_size;

    const char* str = "Hello monday.\0";
    char        buf[1024];
    int         res, cnt = 0;

    while (1)
    {
        cnt ++;
        printf("%3d-th loop\n", cnt);

        res = sendto(fd, str, strlen(str), 0, (const struct sockaddr*)&ser, ser_len);
        if (res < 0)
        {
            perror("\t[client_udp] sendto\n");
            close(fd);
            return -1;
        }
        printf("\t[client_udp]: msg send to %s\n\t", ser.sun_path);
        for (int k = 0; k < res; k ++)
            printf("%c", str[k]);
        printf("\n");

        res = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        if (res < 0)
        {
            perror("\t[client_udp] recvfrom\n");
            close(fd);
            return -1;
        }
        printf("\t[client_udp]: msg recv from %s\n\t", ser.sun_path);
        for (int k = 0; k < res; k ++)                      
            printf("%c", buf[k]);                           
        printf("\n");                                       

        sleep(1);
    }

    close(fd);
    return 0;
}

int main()
{
    // 使用 netstat -an 命令可以看到name
    const char* name = "foo.socket";            // 普通名字空间
    /*const char* name = "\0foo.socket";          // 虚拟名字空间*/
    const char* sock = "bar.socket";            // 普通名字空间
    /*const char* sock = "\0bar.socket";          // 虚拟名字空间*/
    int size = strlen(&name[1]) + 1;
    int sock_size = strlen(&name[1]) + 1;

#if 0
    #if 1
        int ret = server_start_tcp(name, size);
        printf("%d", ret);
    #else
        int ret = client_connect_tcp(name, size);
        printf("%d", ret);
    #endif
#else
    #if 1
        int ret = server_udp(name, size);
        printf("%d", ret);
    #else
        int ret = client_udp(sock, sock_size, name, size);
        printf("%d", ret);
    #endif
#endif
}
