#ifndef __51CONFIG_H__
#define __51CONFIG_H__

/*
 * module: config
 * function: 外设配置及端口分配
 * author: Guyue
 *
 */
 

// 键盘
sbit GPIO_KEY_1 = P1^1;         // 独立按键
#define GPIO_KEY P2             // 矩阵键盘

// LED 
sbit GPIO_LED_1 = P1^0;

// LCD 1062
sbit LCD_E   = P2^7;            // 使能信号
sbit LCD_RS  = P2^6;            // 数据/命令选择端  对应  1/0
sbit LCD_RW  = P2^5;            // 读/写选择端      对应  1/0
#define LCD_DATA_BUS P0         // 数据总线

// 继电器
sbit RELAY = P1^2;

// I2C
sbit SCL = P0^0;
sbit SDA = P0^1;

// PCF8591
#define AddWr 0x90   // 写数据地址
#define AddRd 0x91   // 读数据地址


/* ------------------------- timer options --------------------------*/
#define CONFIG_EA   1    // 总中断开关

#define TMOD_VAL  0x11   // T0、T1均工作在方式1，16位定时器

#define CONFIG_TR0  1    // 定时器0开关
#define CONFIG_ET0  1    // 中断允许
#define TH0_VAL  (65535-50000) / 256;   // T0: 50ms
#define TL0_VAL  (65535-50000) % 256;

#define CONFIG_TR1  1    // 定时器1开关
#define CONFIG_ET1  1    // 中断允许
/* ------------------------- timer options --------------------------*/



/* ------------------------- uart option --------------------------*/
#define UART_BLOCK      // 以阻塞方式读取数据

#define CONFIG_EA   1   // 总中断开关
#define CONFIG_ES   1   // 串口中断开关
#define CONFIG_TR1  1   // 定时器1开关

//#define CONFIG_11_0592MHz_BAUDRATE_4800  // 通信波特率选择
#define CONFIG_11_0592MHz_BAUDRATE_9600
//#define CONFIG_11_0592MHz_BAUDRATE_57600
//#define CONFIG_12MHz_BAUDRATE_4800

#if defined   (CONFIG_11_0592MHz_BAUDRATE_4800)
#define TMOD_VAL   0x20
#define SCON_VAL   0x50
#define TH1_VAL    0xF4
#define TL1_VAL    0xF4
#define PCON_VAL   0x80
#elif defined (CONFIG_11_0592MHz_BAUDRATE_9600)
//#define TMOD_VAL   0x20  // 已在 timer options 定义
#define SCON_VAL   0x50
#define TH1_VAL    0xFD
#define TL1_VAL    0xFD
#define PCON_VAL   0x00
#elif defined (CONFIG_11_0592MHz_BAUDRATE_57600)
#define TMOD_VAL   0x20
#define SCON_VAL   0x50
#define TH1_VAL    0xFF
#define TL1_VAL    0xFF
#define PCON_VAL   0x80
#elif defined (CONFIG_12MHz_BAUDRATE_4800)
#define TMOD_VAL   0x20
#define SCON_VAL   0x50
#define TH1_VAL    0xF3
#define TL1_VAL    0xF3
#define PCON_VAL   0x80
#endif
/* ------------------------- uart option --------------------------*/



#endif