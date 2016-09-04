/******************************************************************************
 * Copyright 2015-2016 Specialized Solutions LLC
 *
 * Title to the Materials (contents of this file) remain with Specialized
 * Solutions LLC.  The Materials are copyrighted and are protected by United
 * States copyright laws.  Copyright notices cannot be removed from the
 * Materials.
 *
 * See the file titled "Specialized Solutions LLC License Agreement.txt"
 * that has been distributed with this file for further licensing details.
 *
 * Pawn built-in state machine example
 *
 *****************************************************************************/

print_state() <initial>
{
	printf("In the initial state\n")
}

print_state() <state1>
{
	printf("In the state1 state\n")
}

print_state() <state2>
{
	printf("In the state2 state\n")
}

print_state() <state3>
{
	printf("In the state3 state\n")
}

print_state() <state4>
{
	printf("In the state4 state\n")
}

print_state() <>
{
	printf("In an unknown state\n")
}

@gpio_input_switch_left() <initial>
{
	print_state()
	state state1
}

@gpio_input_switch_left() <state1>
{
	print_state()
	state state2

}

@gpio_input_switch_left() <state2>
{
	print_state()
	state state3

}

@gpio_input_switch_left() <state3>
{
	print_state()
	state state4

}

@gpio_input_switch_left() <state4>
{
	print_state()
	state state1
}

@gpio_input_switch_left() <>
{
	print_state()
	state initial
}

main()
{
	sleep 1000
    printf "Starting state machine example\n"
    state initial
    qcm_gpio_configure_handler(GPIO_INPUT_SWITCH_LEFT, GPIO_EVENT_HIGH)
}

