/*
 * =====================================================================================
 *
 *       Filename:  event_add.cpp
 *
 *    Description:  event add 分析数据流
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 16时26分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liyankun (), liyankun@baidu.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
int ev_signal_add(ev)
{
    //获取到当前的event_base
    struct event_base* base = ev->ev_base;
    //获取event_base相应的信号
    struct evsignal_info* sig = &ev->ev_base->sig;

    if (ev->ev_events & (EV_READ|EV_WRITE))
        //err
    
    evsignal = ev->ev_fd;//把信号number保存在，同时信号number不能超过NSIG
    
    if (TAILQ_EMPTY(&sig->evsigevents[evsignal]))//如果队列为空的话 
    {
        //信号回调函数evsignal_handler主要功能是统计信号处理多少次，同时发送一个字符"a" 到socketpair去
        _evsignal_set_handler(base, evsignal, evsignal_handler) 
        {
            //如果base->sig.sh_old不够evsignal的话，就重新分配base->sig.sh_hold二维数组
            if (evsignal >= base->sig.sh_old_max)
            {
                base->sig.sh_old_max = evsignal+1;
                base->sig.sh_old = p;//新分配的内存数组
            }
            
            sig->sh_old[evsignal] = malloc(sizeof(*sig->sh_old[evsignal]));
            sigaction(evsignal, &sa, sig->sh_old[evsignal]);//注册信号处理函数，然后把老的函数调用保存在sig->sh_old[evsignal]里去 
            
        }

    }
    //
    TAILQ_INSERT_TAIL(&sig->evsigevents[evsignal], ev, ev_signal_next);//插入到队列尾部
}

int event_add(struct event* ev, const struct timeval* tv)
{
    struct event_base* base = ev->ev_base;
    const struct eventop* evsel = base->evsel;
    void* evbase = base->evbase;

    if (tv != NULL && !(ev->ev_flags & EVLIST_TIMEOUT))
    {
        //如果时间不为空，并且没有设置EVLIST_ TIMEOUT标志
        if (min_heap_reserve(&base->timeheap, 1+min_heap_size(&base->timeheap)))
            return -1;
    }

    //如果设置了事件的可读、可写、信号等，并且这个事件不是激活or插入状态，就加入到epoll中去
    if ((ev->ev_events & (EV_READ|EV_WRITE|EV_SIGNAL)) &&
            !(ev->ev_flags & (EVLIST_INSERTED | EVLIST_ACTIVE)))
    {
        res = evsel->add(evbase, ev)//实际上调用的是epoll_add
        {
            if (ev->ev_events & EV_SIGNAL)
                return ev_signal_add(ev)
                {
                    //查看ev_signal_add函数,见上面函数的分析
                }
            //后面就根绝设置的events事件选项，加入到evbase中的epfd中去了。
        }

        if (res != -1)//添加成功的话
        {
            event_queue_insert(base, ev, EVLIST_INSERTED);
            {
                //check,两次插入操作的话，会插入失败 
                switch (queue)
                {
                    case EVLIST_INSERTED:
                        TAILQ_INSERT_TAIL(&base->eventqueue, ev, ev_next);//插入到eventquque里面去
                        break;
                    case EVLIST_ACTIVE:
                        base->event_count_active++;
                        TAILQ_INSERT_TAIL(base->activequeues[ev->ev_pri], ev, ev_active_next);
                        break;
                    case EVLIST_TIMEOUT:
                        min_heap_push(&base->timeheap,ev);
                        {
                            //最小堆算法
                            //就是从最后的一个叶子节点，按照父节点的路径一步步比较大小
                            //如果父节点比插入叶子节点大，就会把父节点和叶子节点交换
                            //一直到root节点，直到父节点比插入节点小，就跳出循环
                            //然后赋值到这个节点
                            //p是event数组，a是数组大小，n是当前使用大小
                        }
                        break;
                }
            }
        }
    }
    
}
