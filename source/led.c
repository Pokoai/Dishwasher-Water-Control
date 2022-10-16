#include "led.h"


// 1ms ��ʱ����
static void delay_ms(u16 n)
{
	u8 n1 = 23;
	
	n *= n1;
	while ( n-- );
}


// led��
void led_on()
{
	GPIO_LED_1 = 1;
}

// led��
void led_off()
{
	GPIO_LED_1 = 0;
}

// led��˸
// ռ��cpuʱ�䣬����������
void led_flashing()
{	
	int i;
	for ( i = 0; i < 6; i++ ) {
		led_on();
		delay_ms(500);
		led_off();
		delay_ms(500);
	}

	led_off();  // ��˸������Ϩ��
}