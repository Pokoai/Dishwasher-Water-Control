/*
 * date: 2022-10-14
 * author: Guyue
 * describe: 
 */

#include "head.h"


bool key_is_on = false;      // 按键按下标志位
bool relay_is_on = false;    // 继电器导通标志位
// bool alarm_is_on = false;    // 报警标志位    暂时没想到如何使用
bool water_is_full = false;  // 水满标志位
u16 water_hight = 0;  	     // 水位
u8 disp[4];  		  	     // water_hight拆分为4个数存入其中，送到lcd显示
u8 *uart_cmd;				 // 串口接收到的命令
u8 *ir_cmd;			     // 红外线接收到的命令
u8 ir_disp[4];


// 数据处理
void data_pros()
{
	disp[0] = water_hight / 1000 + 48;  // 千位
	disp[1] = water_hight % 1000 / 100 + 48;  // 百位
	disp[2] = water_hight % 1000 % 100 / 10 + 48;  // 十位
	disp[3] = water_hight % 1000 % 100 % 10 + 48;  // 个位	

	ir_disp[0] = *ir_cmd / 16;  // 16进展前4位
	ir_disp[1] = *ir_cmd % 16;  // 16进展后4位
	ir_disp[2] = '\H';

	// 转换为ACSII码
	if ( ir_disp[0] < 10 ) {  // 是数字
		ir_disp[0] += 48;
	} else {
		ir_disp[0] += 55;  // 大写字母
	}
	if ( ir_disp[1] < 10 ) {  // 是数字
		ir_disp[1] += 48;
	} else {
		ir_disp[1] += 55;  // 大写字母
	}

	
}


// 初始化
void init_all()
{
	led_off();     // led熄灭
	relay_off();   // 继电器断开	
	LCD_init();	   // LCD1602初始化
	timer_init();  // 定时器初始化
	uart_init();   // 串口初始化
	Ir_init(); 	   // 红外线接收初始化
}

// 主函数
void main()
{	
	init_all();

	while (1) {
		data_pros();  // 数据处理

		// 经测试，水位数值大于2100，则接触到水
		if ( water_hight > 2100 ) {
			water_is_full = true;  // 水满标志位置位
			// relay_off();  // 继电器断开，常闭电磁阀关闭，即停止上水
			// relay_is_on = false;  // 改变继电器标志位
			// water_hight > 2100 后不立马关闭电磁阀，而是视现实情况，延时一段时间后才关闭

			LCD_write_str(0, 0, "Water FULL!");

			if ( water_hight > 2500 ) {  // 暂时用此方法限制报警
				beep_on();		 // 蜂鸣器响报警
				led_flashing();  // 灯闪烁，水满报警
			}
			
			
		} else if ( relay_is_on ) {  // 打开继电器了
			LCD_write_str(0, 0, "Valve ON... "); 
			water_is_full = false;  // 平时水满标志位清空
		} else {
			LCD_write_str(0, 0, "Water EMPTY!"); 
			water_is_full = false;  // 平时水满标志位清空
		}

		// 显示实际水位数值
		LCD_write_str(0, 1, "Real Hight: ");
		LCD_write_str(12, 1, disp);

		// 将水位值通过串口发送至上位机
		uart_write(disp);  

		// 读取红外线值，若为47H（10进制71），则打开电磁阀；若为45H（10进制69），则打开电磁阀
		// 读取串口接收到的命令，若命令为 OPEN$，则打开电磁阀；若命令为 CLOSE$，则关闭电磁阀
		ir_cmd = Ir_read();
		uart_cmd = uart_read(uart_cmd);  // 统一放到定时器0中断服务函数中

		if ( ( *ir_cmd == 71 || !strcmp(uart_cmd, "OPEN") ) && !water_is_full ) {
			relay_on();  		 // 打开电磁阀，开始上水
			relay_is_on = true;  // 继电器导通标志位置位
			*ir_cmd = 0;  		 // 清空Ir命令
			uart_cmd[0] = '\0';  // 清空串口命令
		} else if ( *ir_cmd == 69 || !strcmp(uart_cmd, "CLOSE") ) {
			relay_off();  		 // 关闭电磁阀
			relay_is_on = false; // 继电器导通标志位清0
			*ir_cmd = 0;  		 // 清空Ir命令
			uart_cmd[0] = '\0';  // 清空串口命令
			TIMER0_CNT2 = 0;     // 只要关闭电磁阀，20s计时位则清0
		}
		// LCD_write_str(12, 0, "    ");  // 第一行最后四位清屏，但会导致字符闪烁
		// LCD_write_str(12, 0, cmd); 

		LCD_write_str(13, 0, ir_disp);  // 显示红外线值 
	}
}


// 定时器0 中断服务程序
void timer0() interrupt 1
{
    TH0 = TH0_VAL;
    TL0 = TL0_VAL; 

	// 50ms更新一次水位数值、红外线命令、串口命令
	water_hight = get_water_hight();  
	// ir_cmd = Ir_read();
	// uart_cmd = uart_read(uart_cmd);
	
	// 100ms更新一次按键状态
	if ( 2 == ++TIMER0_CNT ) {  // 100ms
		TIMER0_CNT = 0;

		// 更新按键状态
		key_is_on = duli_key();  
		if ( key_is_on && !water_is_full ) {  // 按键按下且水未满
			relay_on();  // 打开电磁阀，开始上水
			relay_is_on = true;  // 继电器导通标志位置位
			
			// LCD_write_str(0, 0, "Valve ON...");  // 主函数中有该函数，中断服务程序中尽量不要再使用同样函数，以免发出冲突
			// 可借助relay_is_on标志位，在主函数中调用LCD_write_str()
		} 

		// 读取串口接收到的数据
		// cmd = uart_read(cmd);
		// LCD_clear();
		// LCD_write_str(0, 0, cmd); 
	}

	// 液位传感器检测到水后，延时2s才关闭电磁阀
	if ( water_is_full ) {
		if ( 40 == ++TIMER0_CNT3 ) {
			TIMER0_CNT3 = 0;
			relay_off();
			relay_is_on = false;
		}
	}

	// 保险起见，继电器导通上水后开始计时2min，超过此时间则强制关闭电磁阀
	// 实际上水时间：1min33s
	if ( relay_is_on ) {  
		if ( 2500 == ++TIMER0_CNT2 ) {  // 经测试，2min需要2500次计数
			TIMER0_CNT2 = 0;
			relay_off(); 
			relay_is_on = false;  // 清除标志位
		}
	}


}

