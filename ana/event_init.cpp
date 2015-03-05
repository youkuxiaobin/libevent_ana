/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  libevent代码分析流程
 *
 *        Version:  1.0
 *        Created:  2014年09月20日 20时10分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liyankun (), liyankun@baidu.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#define TAILQ_INIT(head) do {  \
    (head)->tqh_first = NULL;  \
    (head)->tqh_last = &(head)->tqh_first;              \
} while (0)    

struct event_base* event_init()
{
    base = calloc(1, sizeof(struct event_base));

    event_sigcb = NULL;
    event_gotsig = 0;
    detect_monotonic()
    {
        //查看系统是否支持clock_gettime(CLOCK_MONOTONIC, &ts)
        //如果支持 use_monotonic = 1
    }

    //获取系统时间
    gettime(base, &base->event_tv)
    {
        clock_gettime(CLOCK_MONOTONIC, &ts)
        base->event_tv->tv_sec = ts.tv_sec;
        base->event_tv->tv_usec = ts.tv_nsec/1000;
    }

    //初始化定时器最小堆
    min_heap_ctor(&base->timeheap)
    {
        base->timeheap.p = 0;
        base->timeheap.n = 0;
        base->timeheap.a = 0;
    }
    //初始化事件队列，是一个双向链表
    TAILQ_INIT(&base->eventqueue);

    base->sig.ev_signal_pair[0] = -1;
    base->sig.ev_signal_pair[1] = -1; 
    base->evbase = NULL;

    for(i = 0; eventops[i] && !base->evbase; i++)
    {
        //会按照eventops[]数组的索引顺序来遍历数组，选用第一个可用的网络模型，这里是epoll
        base->evsel = eventops[i];
        base->evbase = base->evsel->init(base)//此时调用的是epoll_init(base)
        {
            if (getenv("EVENT_NOEPOLL"))//如果设置不能使用epoll的话，就直接初始化失败
                return NULL;
            //如果说描述符没有设置没有成无限，那么就设置当前最大值(默认值是32000）
            if (getrlimit(RLIMIT_NOFILE, &rl) == 0 &&
                    rl.rlim_cur != RLIM_INFINITY)
            {
                nfiles = rl.rlim_cur - 1;
            }
            
            epfd = epoll_create(nfiles);
            FD_CLOSEONEXEC(epfd);//关闭execl这个描述符复制

            epollop = calloc(1, sizeof(struct epollop));
            epollop->fd = epfd;
            
            epollop->events = malloc(nfiles * sizeof(struct epoll_event));
            epollop->nevents = nfiles;

            epollop->fds = malloc(nfiles, sizeof(struct evepoll));
            epollop->nfds = nfiles;
            
            evsignal_init(base);
            {
                //创建两个相互连接的unix socket
                socketpair(AF_UNIX, SOCK_STREAM, 0 ,base->sig.ev_signal_pair);
                base->sig.sh_hold = NULL;
                base->sig.sh_hold_max = 0;
                base->sig.evsignal_caught = 0;
                memset(&base->sig.evsigcaught, 0, sizeof(sig_atomic_t)*NSIG);
                for(i=0; i < NSIG; i++)
                {
                    TAILQ_INIT(&base->sig.evsigevent[i]);
                }

                event_set(&base->sig.ev_signal, base->sig.ev_signal_pair[1], EV_READ|EV_PERSIST, evsignal_cb, &base->sig.ev_signal)
                {
                    base->sig.ev_signal.ev_base = current_base;
                    base->sig.ev_signal.ev_callback = evsignal_cb;
                    base->sig.ev_signal.ev_arg = base->sig.evsignal;
                    base->sig.ev_signal.ev_fd = base->sig.ev_signal_pair[1];
                    base->sig.ev_signal.ev_events = EV_READ | EV_PERSIST;
                    base->sig.ev_signal.ev_res = 0;
                    base->sig.ev_signal.ev_flags = EV_LIST_INIT;
                    base->sig.ev_signal.ncalls = 0;
                    base->sig.ev_signal.pncalls = NULL;
                    
                    min_heap_elem_init(ev)
                    {
                        base->sig.evsignal.min_heap_idx = -1;
                    }

                    if (current_base)
                        base->sig.evsignal.ev_pri = current_base->nactivequeues/2;

                }
                base->sig.ev_signal.ev_base = base;
                base->sig.ev_signal.ev_flags |= EVLIST_INTERNAL;
            }

        }
    }
    event_base_priority_init(base, 1)
    {
        base->nactivequeues = 1;
        base->activequques = calloc(base->nactivequques, 1*sizeof(struct event_list*));
        for (i =0; i<base->nactivequeues; i++)
        {
            base->activequeue[i] = malloc(sizeof(struct event_list));
            TAILQ_INIT(base->activequeue[i]);
        }
    }
}
