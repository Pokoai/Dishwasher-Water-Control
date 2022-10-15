/*
 * date: 2022-10-14
 * author: Guyue
 * describe: 
 */
 
#include "main.h"


bool key_on = false;  // 按键状态
bool flg = false;  	  // 继电器闭合标志位
u16 water_hight = 0;  // 水位


// 初始化
void init_all()
{
	led_off();     // led熄灭
	relay_off();   // 继电器断开	
	timer_init();  // 启动定时器
}


void main()
{	
	init_all();

	while (1) {
		if ( key_on ) {
			led_flashing();  // 灯闪烁
			relay_on();		 // 继电器闭合
			flg = true; 
		} 
	}
}


// 定时器0 中断服务程序
void timer0() interrupt 1
{
    TH0 = TH0_VAL;
    TL0 = TL0_VAL;

	water_hight = get_water_hight();  // 50ms更新一次水位数值

	if ( 2 == ++TIMER0_CNT ) {  // 100ms
		TIMER0_CNT = 0;
		key_on = duli_key();  // 更新按键状态
	}

	if ( flg ) {  // 继电器闭合后开始计时 10s
		if ( 200 == ++TIMER0_CNT2 ) {
			TIMER0_CNT2 = 0;
			flg = false;
			relay_off();   // 延时10s后断开继电器
		}
	}
	
}

