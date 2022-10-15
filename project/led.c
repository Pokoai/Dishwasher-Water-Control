#include "led.h"

// led亮
void led_on()
{
	GPIO_LED_1 = 1;
}

// led灭
void led_off()
{
	GPIO_LED_1 = 0;
}