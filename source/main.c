/*
 * date: 2022-10-14
 * author: Guyue
 * describe: 
 */
 
#include "main.h"


bool key_on = false;


void main()
{	
	timer_init();  // 启动定时器

	while (1) {
		if ( key_on ) {
			led_flashing();
		}
	}
}


// 定时器0 中断服务程序
void timer0() interrupt 1
{
    TH0 = TH0_VAL;
    TL0 = TL0_VAL;

	if ( 2 == ++TIMER0_CNT ) {  // 100ms
		TIMER0_CNT = 0;
		key_on = duli_key();  // 更新按键状态
	}
}

