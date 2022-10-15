#ifndef __TIMER_H__
#define __TIMER_H__

/*
 * module：timer
 * author：Guyue
 
 * api：
 * void init_timer()
 */


#include "main.h"


static u16 TIMER0_CNT = 0;     // 定时器0计数器1
static u16 TIMER0_CNT2 = 0;     // 定时器0计数器2

extern void timer_init();


#endif