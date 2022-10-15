#ifndef __MAIN_H__
#define __MAIN_H__

/*
 * module：main
 * function: 类型别名定义、项目所用外设头文件声明
 * author：Guyue
 * 
 */


// 系统头文件
#include <reg52.h>     
#include <intrins.h>
#include <string.h>
#include "config.h"  // 外设配置


// 类型定义
typedef  unsigned char  uchar;  
typedef  unsigned int   uint;
typedef  unsigned long  ulong;

typedef  uchar    u8;
typedef  uint     u16;
typedef  ulong    u32;
typedef  char     int8;
typedef  int      int16;
typedef  long     int32;

typedef  char      bool;       
#define  true       1
#define  false      0


// 外设头文件（根据实际项目添加）
// 注：必须放在类型定义后面
#include "key.h"
#include "led.h"
#include "timer.h"
#include "relay.h"
#include "xpt2046.h"
#include "lcd1602.h"


#endif