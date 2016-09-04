/******************************************************************************
 * Copyright 2014-2016 Specialized Solutions LLC
 *
 * Title to the Materials (contents of this file) remain with Specialized
 * Solutions LLC.  The Materials are copyrighted and are protected by United
 * States copyright laws.  Copyright notices cannot be removed from the
 * Materials.
 *
 * See the file titled "Specialized Solutions LLC License Agreement.txt"
 * that has been distributed with this file for further licensing details.
 *
 * Hello World Example 
 *
 * Shows how to use pawn states, timers, printf, LEDs, and the LCD
 *
 *****************************************************************************/

@timer0() <red>
{
	qcm_display_bar(0,0,127,127,graphics_make_rgb_565_color(31,0,0))
	qcm_display_write_text(0,0,0xFFFF, LCD_FONT_MEDIUM, "Hello World")
	qcm_led_set(LED_FRONT_BOTTOM, graphics_make_rgb_565_color(31,0,0))
	qcm_led_set(LED_FRONT_TOP, graphics_make_rgb_565_color(31,0,0))
	qcm_led_set(LED_REAR_BOTTOM, graphics_make_rgb_565_color(31,0,0))
	qcm_led_set(LED_REAR_TOP, graphics_make_rgb_565_color(31,0,0))
	state green
}

@timer0() <green>
{
	qcm_display_bar(0,0,127,127,graphics_make_rgb_565_color(0,63,0))
	qcm_led_set(LED_FRONT_BOTTOM, graphics_make_rgb_565_color(0,63,0))
	qcm_led_set(LED_FRONT_TOP, graphics_make_rgb_565_color(0,63,0))
	qcm_led_set(LED_REAR_BOTTOM, graphics_make_rgb_565_color(0,63,0))
	qcm_led_set(LED_REAR_TOP, graphics_make_rgb_565_color(0,63,0))
	qcm_display_write_text(0,20,0xFFFF, LCD_FONT_MEDIUM, "Hello World")
	state blue
}

@timer0() <blue>
{
	qcm_display_bar(0,0,127,127,graphics_make_rgb_565_color(0,0,31))
	qcm_led_set(LED_FRONT_BOTTOM, graphics_make_rgb_565_color(0,0,31))
	qcm_led_set(LED_FRONT_TOP, graphics_make_rgb_565_color(0,0,31))
	qcm_led_set(LED_REAR_BOTTOM, graphics_make_rgb_565_color(0,0,31))
	qcm_led_set(LED_REAR_TOP, graphics_make_rgb_565_color(0,0,31))
	qcm_display_write_text(0,40,0xFFFF, LCD_FONT_MEDIUM, "Hello World")
	state white
}

@timer0() <white>
{
	qcm_display_bar(0,0,127,127,graphics_make_rgb_565_color(31,63,31))
	qcm_led_set(LED_FRONT_BOTTOM, graphics_make_rgb_565_color(31,63,31))
	qcm_led_set(LED_FRONT_TOP, graphics_make_rgb_565_color(31,63,31))
	qcm_led_set(LED_REAR_BOTTOM, graphics_make_rgb_565_color(31,63,31))
	qcm_led_set(LED_REAR_TOP, graphics_make_rgb_565_color(31,63,31))
	qcm_display_write_text(0,60, 0, LCD_FONT_MEDIUM, "Hello World")
	state black
}

@timer0() <black>
{
	qcm_display_bar(0,0,127,127,0)
	qcm_led_set(LED_FRONT_BOTTOM, 0)
	qcm_led_set(LED_FRONT_TOP, 0)
	qcm_led_set(LED_REAR_BOTTOM, 0)
	qcm_led_set(LED_REAR_TOP, 0)
	qcm_display_write_text(0,80, 0xFFFF, LCD_FONT_MEDIUM, "Hello World")
	state red
}

main()
{
	sleep 1000
    
	printf "Hello world\n"
	
	state red
	
	qcm_display_init()
	qcm_display_backlight(75)
	
	qcm_timer_start(TIMER_0, 1000, true)
}
