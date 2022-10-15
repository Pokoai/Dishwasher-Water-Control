#include "led.h"

// led¡¡
void led_on()
{
	GPIO_LED_1 = 1;
}

// led√
void led_off()
{
	GPIO_LED_1 = 0;
}
