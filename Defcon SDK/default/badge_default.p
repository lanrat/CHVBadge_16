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
 * Default Badge Script for DefCon 24
 *
 * This demo shows off many features of the PAWN language and the QCM
 * subsystem.
 *
 *****************************************************************************/

#include "Defcon.p"
#include "Rapid7.p"
#include "CHV.p"

new selected_item = 0
 
/******************************************************************************
 * Menu handling code 
 *****************************************************************************/
new const menu_items[4]{} = [ "Demo", "Julia", "Cube", "Settings"]

show_menu()
{
	new i
	
    state menu
    
	qcm_display_bar(0,0,127,127,0)
	graphics_box(0,0,127,127,0xFFFF)

	for (i = 0; i < 4; i++)
	{
		if (i == selected_item)
		{
			qcm_display_bar(1,i * 20,126,i * 20 + 20, 0xFFFF)
			qcm_display_write_text(1,i * 20, 0x0000, LCD_FONT_MEDIUM,menu_items[i])
		}
		else
		{
			qcm_display_write_text(1,i * 20,0xFFFF,LCD_FONT_MEDIUM,menu_items[i])
		}
	}
}

input_switch_left_debounce() <menu>
{
	selected_item++;
	
	if (selected_item > 3)
	{
		selected_item = 0;
	}
	
	show_menu()
	
	printf("Changed %d\n", selected_item);
	
}

input_switch_right_debounce() <menu>
{
	if (selected_item == 0)
	{
		start_demo()
	}
    else if (selected_item == 1)
	{
		selected_item = 0
		plot_julia()
	}
	else if (selected_item == 2)
	{
		selected_item = 0
		cube_run()
	}
	else if (selected_item == 3)
	{
		state settings
		selected_item = 0
		show_settings()
	}
	else
	{
		selected_item = 0
		show_menu()
	}
}

/******************************************************************************
 * Settings handling code 
 *****************************************************************************/
new settings_items[6]{} = [ "Backlight", 
                          "Lo Frnt LED", 
						  "Hi Frnt LED",
						  "Lo Rear LED",
						  "Hi Rear LED",
						  "Exit"]
new selected_led;

show_settings()
{
	new i
	
	qcm_display_bar(0,0,127,127,0)
	graphics_box(0,0,127,127,0xFFFF)

	for (i = 0; i < 6; i++)
	{
		if (i == selected_item)
		{
			qcm_display_bar(1,i * 20,126,i * 20 + 20, 0xFFFF)
			qcm_display_write_text(1,i * 20,0,LCD_FONT_MEDIUM,settings_items[i])
		}
		else
		{
			qcm_display_write_text(1,i * 20,0xFFFF,LCD_FONT_MEDIUM,settings_items[i])
		}
	}
}

input_switch_left_debounce() <settings>
{
	selected_item++;
	
	if (selected_item > 5)
	{
		selected_item = 0
	}
	
	show_settings()
}

input_switch_right_debounce() <settings>
{
	if (selected_item == 0)
	{
		state backlight
		adjust_backlight()
	}
	else if ((selected_item > 0) &&
			 (selected_item < 5))
	{
		state led_adjust
		selected_led = selected_item - 1;
		selected_item = 0
		show_led_settings()
	}
	else if (selected_item == 5)
	{
		state menu
		selected_item = 0
		show_menu()
	}
	else
	{
		selected_item = 0
		show_settings()
	}
}

/******************************************************************************
 * Backlight handling code 
 *****************************************************************************/
new duty = 75

adjust_backlight()
{
	new i
	new item_text{25}
	
	qcm_display_bar(0,0,127,127,0)
	graphics_box(0,0,127,127,0xFFFF)

	for (i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			strformat(item_text, sizeof item_text, true, "Duty: %d", duty)
		}
		else
		{
			item_text = "Exit"
		}
		
		if (i == selected_item)
		{
			qcm_display_bar(1,i * 20,126,i * 20 + 20, 0xFFFF)
			qcm_display_write_text(1,i*20,0,LCD_FONT_MEDIUM,item_text)
		}
		else
		{
			qcm_display_write_text(1,i*20,0xFFFF,LCD_FONT_MEDIUM,item_text)
		}
	}
}

input_switch_left_debounce() <backlight>
{
	selected_item++;
	
	if (selected_item > 1)
	{
		selected_item = 0;
	}
	
	adjust_backlight()
}

input_switch_right_debounce() <backlight>
{
	if (selected_item == 0)
	{
		duty++
		if (duty > 100)
		{
			duty = 0
		}
		qcm_display_backlight(duty)
		adjust_backlight()
	}
	else if (selected_item == 1)
	{
		selected_item = 0
		state settings
		show_settings()
	}
	else
	{
		selected_item = 0
		adjust_backlight()
	}
}


/******************************************************************************
 * LED handling code 
 *****************************************************************************/
new red[4]
new green[4]
new blue[4]

show_led_settings()
{
	new i
	new item_text{20}
	
	qcm_display_bar(0,0,127,127,0)
	graphics_box(0,0,127,127,0xFFFF)

	for (i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			strformat(item_text, sizeof item_text, true, "Red: %d", red[selected_led])
		}
		else if (i == 1)
		{
			strformat(item_text, sizeof item_text, true, "Green: %d", green[selected_led])
		}
		else if (i == 2)
		{
			strformat(item_text, sizeof item_text, true, "Blue: %d", blue[selected_led])
		}
		else
		{
			item_text = "Exit"
		}
		
		if (i == selected_item)
		{
			qcm_display_bar(1,i * 20,126,i * 20 + 20, 0xFFFF)
			qcm_display_write_text(1,i * 20,0,LCD_FONT_MEDIUM,item_text)
		}
		else
		{
			qcm_display_write_text(1,i * 20,0xFFFF,LCD_FONT_MEDIUM,item_text)
		}
	}
}

input_switch_left_debounce() <led_adjust>
{
	selected_item++;
	
	if (selected_item > 3)
	{
		selected_item = 0;
	}
	
	show_led_settings()
}

input_switch_right_debounce() <led_adjust>
{
	new color
	if (selected_item == 0)
	{
		red[selected_led]++
		
		if (red[selected_led] > 31)
		{
			red[selected_led] = 0;
		}
		color = graphics_make_rgb_565_color(red[selected_led], green[selected_led], blue[selected_led])
		
		qcm_led_set(LED:selected_led, color)
		show_led_settings()
	}
	else if (selected_item == 1)
	{
		green[selected_led]++
		
		if (green[selected_led] > 63)
		{
			green[selected_led] = 0;
		}
		
		color = graphics_make_rgb_565_color(red[selected_led], green[selected_led], blue[selected_led])
		
		qcm_led_set(LED:selected_led, color)
		
		show_led_settings()
	}
	else if (selected_item == 2)
	{
		blue[selected_led]++
		
		if (blue[selected_led] > 31)
		{
			blue[selected_led] = 0;
		}
		
		color = graphics_make_rgb_565_color(red[selected_led], green[selected_led], blue[selected_led])
		
		qcm_led_set(LED:selected_led, color)
		
		show_led_settings()
	}
	else if (selected_item == 3)
	{
		selected_item = 0
		state settings
		show_settings()
	}
	else
	{
		selected_item = 0
		show_led_settings()
	}
}

/******************************************************************************
 * Demo1 code 
 *****************************************************************************/
new Float:led_counter

start_demo()
{
	qcm_timer_start(TIMER_0,250,true)
	qcm_timer_start(TIMER_1,3000,true)
    qcm_display_bar(0,0,127,127,0)
	display_Defcon()
	state demo_2
}

@timer0()
{
	/* walk through colors with LEDs */
	new Float:red_local
	new Float:green_local
	new Float:blue_local
	new color
	
	led_counter += 0.1
	
	red_local = 16.0 + floatsin(Float:led_counter) * 16.0
	green_local = 32.0 + floatsin(Float:led_counter + 3.1416 * 2.0 / 3.0) * 32.0
	blue_local = 16.0 + floatsin(Float:led_counter + 3.1416 * 4.0 / 3.0) * 16.0	
	color = graphics_make_rgb_565_color(floatround(red_local, floatround_floor) >> 1, floatround(green_local, floatround_floor) >> 1, floatround(blue_local, floatround_floor) >> 1)
		
	qcm_led_set(LED_FRONT_BOTTOM, color)
	qcm_led_set(LED_FRONT_TOP, color)
	qcm_led_set(LED_REAR_BOTTOM, color)
	qcm_led_set(LED_REAR_TOP, color)
}


@timer1() <demo_1>
{
	/* show sponsor 1 logo */
	qcm_display_bar(0,0,127,127,0)
	display_Defcon()
	state demo_2
}

@timer1() <demo_2>
{
	/* show sponsor 1 logo */
	qcm_display_bar(0,0,127,127,0xFFFF)
	display_CHV()
	state demo_3
}


@timer1() <demo_3>
{
	/* show specsol logo */
	qcm_display_bar(0,0,127,127,0xFFFF)
	display_Rapid7()
	state demo_1
}


stop_demo()
{
	qcm_timer_stop(TIMER_0)
	qcm_timer_stop(TIMER_1)
	qcm_led_set(LED_FRONT_BOTTOM, 0)
	qcm_led_set(LED_FRONT_TOP, 0)
	qcm_led_set(LED_REAR_BOTTOM, 0)
	qcm_led_set(LED_REAR_TOP, 0)
	selected_item = 0
	state menu
	show_menu()
}

input_switch_left_debounce() <demo_1>
{
	stop_demo()
}

input_switch_right_debounce() <demo_1>
{
	stop_demo()
}

input_switch_left_debounce() <demo_2>
{
	stop_demo()
}

input_switch_right_debounce() <demo_2>
{
	stop_demo()
}

input_switch_left_debounce() <demo_3>
{
	stop_demo()
}

input_switch_right_debounce() <demo_3>
{
	stop_demo()
}

/******************************************************************************
 * Julia code 
 *****************************************************************************/
julia_iteration(Float:Zx, Float:Zy, Float:Cx, Float:Cy, IterationMax, Float:ER2)
{
    new Float:Zx2
    new Float:Zy2
    new i=0

    Zx2 = Zx * Zx
    Zy2 = Zy * Zy

    while ( i < IterationMax && (Zx2+Zy2 < ER2) )
    {
        Zy = 2*Zx*Zy + Cy;
        Zx = Zx2-Zy2 +Cx;
        Zx2 = Zx*Zx;
        Zy2 = Zy*Zy;
        i += 1;
    }
        
    return i;
}

plot_julia()
{
	new x,y
    new Float:xform_x
    new Float:xform_y
    new color;
    
    state julia
    
    qcm_display_bar(0,0,127,127,0)
	
    x = 0;
    y = 0;
    
    for (x = 0; x < 128; x++)
    {
        for (y = 0; y < 128; y++)
        {
           xform_x = float(x) / 64.0 - 1.0;
           xform_y = float(y) / 64.0 - 1.0;
           color = julia_iteration(xform_x, xform_y, 0.36, -0.36, 64, 2.0)
           
           color = graphics_make_rgb_565_color(color & 0x1F, color & 0x3F, color & 0x1F)
           
           qcm_display_set_pixel(x, y, color)
        }
    }
}

input_switch_left_debounce() <julia>
{
	selected_item = 0;
	show_menu()
}

input_switch_right_debounce() <julia>
{
	selected_item = 0;
	show_menu()
}


/******************************************************************************
 * Cube code 
 *****************************************************************************/
#define ORG (-10)

new face1[5][2] = [
			[ 28, 28 ],
			[ 100, 28 ],
			[ 100, 100 ],
			[ 28, 100 ],
			[ 28, 28 ]
			   ];
 
new face2[5][2] = [
			    [ 28-ORG, 28+ORG ],
			    [ 100-ORG, 28+ORG ],
			    [ 100-ORG, 100+ORG ],
			    [ 28-ORG, 100+ORG ],
			    [ 28-ORG, 28+ORG ]
		           ];

#define COS_ANGLE (0.996194698)
#define SIN_ANGLE (0.087155742)

new midx1, midy1, midx2, midy2

cube_rotate()
{
    new i
	new xnew, ynew
	
    for (i=0; i<5; i++)
    {
	 
		xnew = floatround(float(midx1) + float(face1[i][0] - midx1) * COS_ANGLE -
						  float(face1[i][1] - midy1) * SIN_ANGLE)
		ynew = floatround(float(midy1) + float(face1[i][0] - midx1) * SIN_ANGLE +
						  float(face1[i][1] - midy1) * COS_ANGLE)
	 
		face1[i][0] = xnew
		face1[i][1] = ynew
	 
		xnew = floatround(float(midx2) + float(face2[i][0] - midx2) * COS_ANGLE -
						  float(face2[i][1] - midy2) * SIN_ANGLE)
		ynew = floatround(float(midy2) + float(face2[i][0] - midx2) * SIN_ANGLE +
						  float(face2[i][1] - midy2) * COS_ANGLE)
	 
		face2[i][0] = xnew
		face2[i][1] = ynew
    }
 
	
} 

cube_display()
{
	new i
	
	/* clear the display */
    qcm_display_bar(0,0,127,127,0)
 
    for (i=0; i<4; i++)
    {
		graphics_line(face1[i][0], face1[i][1], face1[i+1][0], face1[i+1][1], 0b1111100000000000)
		graphics_line(face2[i][0], face2[i][1], face2[i+1][0], face2[i+1][1], 0b0000011111100000)
		graphics_line(face1[i][0], face1[i][1], face2[i][0], face2[i][1], 0b1111111111100000)
    }
	
}

cube_run()
{
	state cube
	
	midx1 = (face1[0][0] + face1[1][0]) / 2;
    midy1 = (face1[1][1] + face1[2][1]) / 2;
    midx2 = (face2[0][0] + face2[1][0]) / 2;
    midy2 = (face2[1][1] + face2[2][1]) / 2;
	
	cube_display()
	
	qcm_timer_start(TIMER_1,250,true)
}

@timer1() <cube>
{
	cube_rotate()
	cube_display()
}

input_switch_left_debounce() <cube>
{
	qcm_timer_stop(TIMER_1)
	selected_item = 0;
	show_menu()
}

input_switch_right_debounce() <cube>
{
	qcm_timer_stop(TIMER_1)
	selected_item = 0;
	show_menu()
}

/******************************************************************************
 * Default state handlers
 *****************************************************************************/

input_switch_right_debounce() <>
{

}

input_switch_left_debounce() <>
{

}

@timer1() <>
{

}


/******************************************************************************
 * Switch debounce routines
 *****************************************************************************/
@gpio_input_switch_right()
{
    /* start debounce timer */
    qcm_timer_start(TIMER_2,50,false)
}

@gpio_input_switch_left()
{
    /* start debounce timer */
    qcm_timer_start(TIMER_3,50,false)
}

@timer2()
{
    if (qcm_gpio_get_input(GPIO_INPUT_SWITCH_RIGHT))
    {
        /* button has been released - call new function to handle debounced input */
        input_switch_right_debounce()
    }
}

@timer3()
{
    if (qcm_gpio_get_input(GPIO_INPUT_SWITCH_LEFT))
    {
        /* button has been released - call new function to handle debounced input */
        input_switch_left_debounce()
    }
}

/******************************************************************************
 * Main routine (initilization)
 *****************************************************************************/
main()
{
	sleep 1000

	state menu
	
	qcm_display_init()
	qcm_display_backlight(75)
	qcm_gpio_configure_handler(GPIO_INPUT_SWITCH_LEFT, GPIO_EVENT_HIGH)
	qcm_gpio_configure_handler(GPIO_INPUT_SWITCH_RIGHT, GPIO_EVENT_HIGH)

	show_menu()
	
}
