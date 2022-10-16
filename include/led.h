#ifndef __LED_H__
#define __LED_H__

/*
 * module：单个led亮灭控制
 * author：Guyue
 * 
 * api：
 * void led_on()    点亮led
 * void led_off()   熄灭led
 */


#include "main.h"

extern void led_on();
extern void led_off();
extern void led_flashing();


#endif
