/*
 * =====================================================================================
 *
 *       Filename:  event_set.cpp
 *
 *    Description:  event set 函数的分析过程
 *
 *        Version:  1.0
 *        Created:  2015年03月04日 10时38分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liyankun (), liyankun@baidu.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

void event_set(struct event *ev, int fd, short events, void (*callback)(int , short, void*), void* args)
{
    ev->ev_base = current_base;     //设置默认的ev_base为current_base，其中current_base为最后创建的event_base的对象指针
    ev->ev_callback = callback;     //设置回调函数
    ev->ev_arg = arg;               //设置回调函数的参数
    ev->ev_fd = fd;                 //设置回调函数传递的fd
    ev->ev_events = events;         //设置当前事件需要调用的事件类型
    /*
     * EV_TIMEOUT       0x01
     * EV_READ          0x02
     * EV_WRITE         0x04
     * EV_SIGNAL        0x08
     * EV_PERSIST       0x10
     */
    ev->ev_res = 0;                 //记录了当前激活事件的类型
    ev->ev_flag = EVLIST_INIT;       //设置事件类型，用于表示当前的事件处于什么阶段
    /*                    
    * EVLIST_TIMEOUT   0x01     //代表event在time堆中
    * EVLIST_INSERTED  0x02     //代表event在已注册时间链表中
    * EVLIST_SIGNAL    0x04     //未见使用
    * EVLIST_ACTIVE    0x08     //代表event在激活链表中
    * EVLIST_INTERNAL  0x10     //内部使用标记
    * EVLIST_INIT      0x80     //代表event已经被初始化
    */ 
    ev->ev_ncalls = 0;          //代表callback被执行多少次
    ev->ev_pncalls = NULL;      //指向ev_ncalls or NULL
    
    min_heap_elem_init(ev);     //初始化时间堆的索引值
    if (current_base)
        ev->ev_pri = current_base->nactivequeues/2;//设置当前事件被激活之后，插入的权限事件队列索引值

}
