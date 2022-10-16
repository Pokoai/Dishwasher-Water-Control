/*
 * date: 2022-10-14
 * author: Guyue
 * describe: 
 */
 
#include "main.h"


bool key_is_on = false;     // 按键按下标志位
bool relay_is_on = false;   // 继电器导通标志位
u16 water_hight = 0;  	    // 水位
u8 disp[4];  		  	    // water_hight拆分为4个数存入其中


// 数据处理
void data_pros()
{
	disp[0] = water_hight / 1000 + 48;  // 千位
	disp[1] = water_hight % 1000 / 100 + 48;  // 百位
	disp[2] = water_hight % 1000 % 100 / 10 + 48;  // 十位
	disp[3] = water_hight % 1000 % 100 % 10 + 48;  // 个位	
}


// 初始化
void init_all()
{
	led_off();     // led熄灭
	relay_off();   // 继电器断开	
	LCD_init();	   // LCD1602初始化
	timer_init();  // 启动定时器
}

// 主函数
void main()
{	
	init_all();
	while (1) {
		data_pros();  // 数据处理

		// 经测试，水位数值大于2000，则接触到水
		if ( water_hight > 1900 ) {
			relay_off();  // 继电器断开，常闭电磁阀关闭，即停止上水
			relay_is_on = false;  // 清空标志位

			LCD_write_str(0, 0, "Water FULL!");
			beep_on();		 // 蜂鸣器响报警
			led_flashing();  // 灯闪烁，水满报警
		} else {
			LCD_write_str(0, 0, "Water EMPTY!");  // 为了简便，只显示两种状态
		}

		// 显示实际水位数值
		LCD_write_str(0, 1, "Real Hight: ");
		LCD_write_str(12, 1, disp);
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
		key_is_on = duli_key();  // 更新按键状态

		if ( key_is_on ) {  // 按键按下
			relay_on();  // 打开电磁阀，开始上水
			relay_is_on = true;  // 继电器导通标志位置位
		} 
	}

	// 保险起见，继电器导通上水后开始计时30s，超过此时间则强制关闭电磁阀
	if ( relay_is_on ) {  
		if ( 800 == ++TIMER0_CNT2 ) {  // 经测试，30s需要800次计数
			TIMER0_CNT2 = 0;
			relay_off(); 
			relay_is_on = false;  // 清除标志位
		}
	}
	
}

