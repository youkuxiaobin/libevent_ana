/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  定时器使用libevent来完成
 *
 *        Version:  1.0
 *        Created:  2015年03月03日 14时43分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liyankun (), liyankun@baidu.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "event.h"
#include <iostream>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <list>
#include <pthread.h>
#include <sys/epoll.h>
using namespace std;
#define LOGDBG(fmt, args...) do {\
    printf("[%s:%d]%s   "fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args);\
}while(0);

#define LOGERR(fmt, args...) do {\
    printf("[%s:%d]%s   "fmt"(%s)\n", __FILE__, __LINE__, __FUNCTION__, ##args, strerror(errno));\
}while(0);

struct event g_listen_ev;
struct event g_conn_ev[1024];
uint16_t g_uindex = 0;

struct event g_time_ev;
struct timeval tv;
struct event_base* g_base;
uint32_t g_time_count = 0;

void usage()
{
    printf("usage:\n");
    printf("    -i ip -p port\n");
    exit(-1);
}
void timer_cb(int fd, short event, void* args)
{
    LOGDBG("g_listen_ev count(%u)", g_time_count);
    g_time_count = 0;
    event_add(&g_time_ev, &tv);
}

void client_rcb(int fd, short event, void* args)
{
    char buf[1024] = {0};
    int rsize = read(fd, buf, 1024);
    if (rsize <=0 )
    {
        event_del((struct event*) args);
        close(fd);
        LOGDBG("client close");
        return ;
    }
    int wsize = write(fd, buf, rsize);
    if (wsize != rsize)
    {
        LOGERR("write to client failed wsize(%d) rsize(%d)", wsize, rsize);
        event_del((struct event*) args);
        close(fd);
        return ;
    }
    event_add((struct event*) args, NULL);
    g_time_count++;
}
void listen_cb(int fd, short event, void* args)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int connect = accept(fd, (struct sockaddr*)&addr, &len);
    if (connect < 0)
    {
        if (errno == EINTR)
        {
            LOGERR("accept is interrupt by signo");
            return;
        }
        return ;
    }
    LOGDBG("connect client once"); 
    event_set(&g_conn_ev[g_uindex], connect, EV_READ, client_rcb, &g_conn_ev[g_uindex]);
    event_add(&g_conn_ev[g_uindex], NULL);
    g_uindex++;
}
    
int main(int argc, char** argv)
{
    string host;
    uint16_t port;
    char ch;
    while (ch = getopt(argc, argv, "i:p:"))
    {
        if (ch == -1)
            break;
        switch(ch)
        {
            case 'i':
                host = optarg;
                break;
            case 'p':
                port = (uint16_t)atoi(optarg);
                break;
            default:
                usage();
        }
    }
    
    if (host.empty() || !port)
    {
        LOGERR("host is empty or port is zero");
        usage();
    }


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        LOGERR("create socket failed");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr.s_addr);
    addr.sin_port = htons(port);

    int ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0)
    {
        LOGERR("server bind addr failed");
        return -1;
    }

    ret = listen(sock, 10240);
    if (ret < 0)
    {
        LOGERR("listen sock failed");
        return -1;
    }
    g_base = event_init();
    event_set(&g_listen_ev, sock, EV_READ|EV_PERSIST, listen_cb ,NULL);
    evtimer_set(&g_time_ev, timer_cb, NULL); 
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    event_add(&g_time_ev, &tv);
    event_add(&g_listen_ev, NULL);
    event_base_dispatch(g_base);
    return 0;
}
