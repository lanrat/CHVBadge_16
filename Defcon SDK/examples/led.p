/******************************************************************************
 * Copyright 2016 Specialized Solutions LLC
 *
 * Title to the Materials (contents of this file) remain with Specialized
 * Solutions LLC.  The Materials are copyrighted and are protected by United
 * States copyright laws.  Copyright notices cannot be removed from the
 * Materials.
 *
 * See the file titled "Specialized Solutions LLC License Agreement.txt"
 * that has been distributed with this file for further licensing details.
 *
 * LED Example 
 *
 * Shows how to use LEDs
 *
 *****************************************************************************/
new Float:led_counter = 0.0

@timer0() 
{
	new Float:red_local
	new Float:green_local
	new Float:blue_local
	new color
	
	led_counter+=0.1
	
	red_local = 16.0 + floatsin(led_counter) * 16.0;
	green_local = 32.0 + floatsin(led_counter + 2.09439510) * 32.0;
	blue_local = 16.0 + floatsin(led_counter +  4.18879020) * 16.0;
	
	color = graphics_make_rgb_565_color(floatround(red_local, floatround_floor), floatround(green_local, floatround_floor), floatround(blue_local, floatround_floor))
	
	qcm_led_set(LED_FRONT_BOTTOM, color)
	qcm_led_set(LED_FRONT_TOP, color)
	qcm_led_set(LED_REAR_BOTTOM, color)
	qcm_led_set(LED_REAR_TOP, color)
}

main()
{
	sleep 1000
	
	qcm_timer_start(TIMER_0, 250, true)
}
