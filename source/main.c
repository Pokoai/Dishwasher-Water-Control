/*
 * date: 2022-10-14
 * author: Guyue
 * describe: 
 */

#include "head.h"


bool key_is_on = false;      // 按键按下标志位
bool relay_is_on = false;    // 继电器导通标志位
bool water_is_full = false;  // 水满标志位
u16 water_hight = 0;  	     // 水位
u8 disp[4];  		  	     // water_hight拆分为4个数存入其中，送到lcd显示
u8 *cmd;					 // 串口接收到的命令
u8 ir_cmd = 0;					 // 红外线接收到的命令
u8 ir_disp[4];


// 数据处理
void data_pros()
{
	disp[0] = water_hight / 1000 + 48;  // 千位
	disp[1] = water_hight % 1000 / 100 + 48;  // 百位
	disp[2] = water_hight % 1000 % 100 / 10 + 48;  // 十位
	disp[3] = water_hight % 1000 % 100 % 10 + 48;  // 个位	

	ir_disp[0] = ir_cmd / 16;  // 16进展前4位
	ir_disp[1] = ir_cmd % 16;  // 16进展后4位
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

		// 经测试，水位数值大于2000，则接触到水
		if ( water_hight > 1800 ) {
			relay_off();  // 继电器断开，常闭电磁阀关闭，即停止上水
			relay_is_on = false;  // 改变继电器标志位
			water_is_full = true;  // 水满标志位置位

			LCD_write_str(0, 0, "Water FULL!");
			beep_on();		 // 蜂鸣器响报警
			led_flashing();  // 灯闪烁，水满报警
		} else if ( relay_is_on ) {  // 打开继电器了
			LCD_write_str(0, 0, "Valve ON... "); 
		} else {
			LCD_write_str(0, 0, "Water EMPTY!"); 
			water_is_full = false;  // 平时水满标志位清空
		}

		// 显示实际水位数值
		LCD_write_str(0, 1, "Real Hight: ");
		LCD_write_str(12, 1, disp);

		// 将水位值通过串口发送至上位机
		uart_write(disp);  

		// 读取串口接收到的命令
		// 若命令为 OPEN#$，则打开电磁阀
		cmd = uart_read(cmd);
		if ( !strcmp(cmd, "OPEN#") && !water_is_full ) {
			relay_on();  // 打开电磁阀，开始上水
			relay_is_on = true;  // 继电器导通标志位置位
			cmd[0] = '\0';  // 清空cmd字符串
		} else if ( !strcmp(cmd, "CLOSE#") ) {
			relay_off();  // 关闭电磁阀
			relay_is_on = false;
			cmd[0] = '\0';  // 清空cmd字符串
		}
		// LCD_write_str(12, 0, "    ");  // 第一行最后四位清屏，但会导致字符闪烁
		// LCD_write_str(12, 0, cmd); 

		LCD_write_str(14, 0, ir_disp);  // 显示红外线值 
	}
}


// 定时器0 中断服务程序
void timer0() interrupt 1
{
    TH0 = TH0_VAL;
    TL0 = TL0_VAL; 

	// 50ms更新一次水位数值、红外线命令
	water_hight = get_water_hight();  
	ir_cmd = Ir_read();
	
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

	// 保险起见，继电器导通上水后开始计时20s，超过此时间则强制关闭电磁阀
	if ( relay_is_on ) {  
		if ( 600 == ++TIMER0_CNT2 ) {  // 经测试，30s需要800次计数
			TIMER0_CNT2 = 0;
			relay_off(); 
			relay_is_on = false;  // 清除标志位
		}
	}
}

