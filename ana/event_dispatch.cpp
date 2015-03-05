/*
 * =====================================================================================
 *
 *       Filename:  event_dispatch.cpp
 *
 *    Description:  事件循环
 *
 *        Version:  1.0
 *        Created:  2014年10月14日 09时44分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liyankun (), liyankun@baidu.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

int event_base_loop(struct event_base* base, int flags)
{
    base->tv_cache.tv_sec = 0;/* 清除缓存时间 */

    if (base->sig.ev_signal_added)//如果说信号已经添加了，那么，就把evsignal_base指向base
        evsignal_base = base;
    
    done = 0;
    while(!done)
    {
        if (base->event_gotterm)
        {
            base->event_gotterm = 0;
            break;//如果说设置了event_gotterm的话，就直接跳出事件循环
        }
        if (base->event_break)
        {
            base->event_break = 0;
            break;//如果说设置了event_break的话，就直接跳出事件循环
        }

        timeout_correct(base, &tv);
        {
            //如果当前时间大于base->event_tv的话，那么就更新base->event_tv为最新时间
            //如果当前时间小于base->event_tv的话，说明用户修改了时间，当然肯定不是monitor时间啦
            //小于进行如下步骤
            evutil_timersub(&base->event_tv, tv, &off);/* 这个函数是获取event_tv和tv之间的差值，保存到off这变量里面 */
            //然后把定时器的最小堆中的ev_timeout减掉差值

        }

        tv_p = &tv;
        //如果说当前没有机会事件，上次已经处理完了，并且没有设置非阻塞标志
        if (!base->event_count_active && !(flags & EVLOOP_NONBLOCK))
        {
            timeout_next(base, &tv_p);
            {
                if (ev = min_heap_top(&base->timeheap) == NULL)
                {
                    *tv_p = NULL;//如果说最小堆没有节点，也就是说没有定时任务的话，会吧tv_p指向NULL
                    return 0;
                }

                if (gettime(base, &now) == -1)
                {
                    return -1;
                }

                if (evutil_timecmp(&ev->ev_timeout, &now, <=))
                {
                    //如果发现有定时任务，并且定时任务的最小堆根节点超时时间小于当前时间，就是已经有事件超时了
                    //就把tv_p的时间给清空了,表示不用epoll_wait等待时间，就会有定时事件处理
                }
                //如果定时事件还没有超时，那么就把当前时间和最小堆的根节点之差赋值给tv_p
            }
        }
        else
        {
            //如果说设置为非阻塞的or还有激活事件，那么把epoll_wait的时间清空
            evutil_timeclear(&tv);
        }

        if (event_haveevents(base))
        {
            //如果没有注册时间，那么就直接退出循环
            return 1;
        }

        gettime(base, &base->event_tv);//设置base->event_tv为
        base->tv_cache.tv_sec = 0;


        res = evsel->dispatch(base, evbase, tv_p);//这个实际上就是调用epoll_wait函数
        res = epoll_dispath(base, evbase, tv_p);
        {
            struct epollop* epollop = evbase;
            struct epoll_event* events = epollop->events;

            if (tv_p != NULL)
                timeout = tv_p->tv_sec * 1000 + (tv_p->tv_usec + 999)/1000;//因为epoll_wait等待时间单位是毫秒，微秒取floor

            if (timeout > 47)//对epoll_wait内核有最大等待时间，大概好似47s左右
            {
                timeout = MAX_EPOLL_TIMEOUT_MSEC;
            }

            res = epoll_wait(epollop->epfd, events, epollop->nevents, timeout);
            if (res = -1)
            {
                if (errno != EINTR)//如果epoll_wait返回出错，那么如果不是信号中断的，那么就退出epoll_wait
                    return -1;
                evsignal_process(base)
                {
                    struct evsignal_info* sig = &base->sig;
                    struct event* ev, *next_ev;

                    base->sig.evsignal_caught = 0;
                    for (i = 1; i<NSIG;i++)
                    {
                        //遍历每一个信号event队列，如果发现队列中存在节点，那么就把这个节点加入到激活队列中去
                    }
                }

                return 0;
            }

            for(i = 0; i<res; i++)
            {
                //根据返回的事件类型来决定加入到那个激活队列中去
                int what = events[i].events;
                struct event* evread = NULL, *evwrite = NULL;
                
                evep = (struct evepoll*) event[i].data.ptr;
                if (what & EPOLLHUP|EPOLLERR)
                {
                    evread = evep->evread;
                    evwrite = evep->evwrite;
                }
                else
                {
                    if (what & EPOLLIN)
                    {
                        evread = evep->evread;
                    }
                    if (what & EPOLLOUT)
                    {
                        evwrite = evep->evwrite;
                    }
                }

                if (evread != NULL)
                    event_active(evread, EV_READ, 1);
                if (evwrite != NULL)
                    event_active(evwrite,EV_WRITE, 1);
            }
        }

        gettime(base, &base->tv_cache);
        timeout_process(base)
        {
            //获取队列超时节点，加入到队列中去
        }
        event_process_active(base)
        {
            //回调函数进行处理
        }
    }
}
