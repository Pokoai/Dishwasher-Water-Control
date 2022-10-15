/*
 * date: 2022-10-14
 * author: Guyue
 * describe: 
 */
 
#include "main.h"

void main()
{	
	while (1) {
		bool key = duli_key();
		
		if ( key ) {
			led_off();
		} else {
			led_on();
		}
	}
}