/******************************************************************************
 * Copyright 2014-2015 Specialized Solutions LLC
 *
 * Title to the Materials (contents of this file) remain with Specialized
 * Solutions LLC.  The Materials are copyrighted and are protected by United
 * States copyright laws.  Copyright notices cannot be removed from the
 * Materials.
 *
 * See the file titled "Specialized Solutions LLC License Agreement.txt"
 * that has been distributed with this file for further licensing details.
 *
 * QCM GPIO Example
 *
 *****************************************************************************/
new counter = 0

print_gpio_notice(gpio, bool:io_state)
{
    printf("GPIO %d event - state %d\n", gpio, io_state)
}

@gpio_input_switch_left()
{
    printf("Left switch state change to %d\n", qcm_gpio_get_input(GPIO_INPUT_SWITCH_LEFT))
    
}

@gpio_input_switch_right()
{
    printf("Right switch state change to %d\n", qcm_gpio_get_input(GPIO_INPUT_SWITCH_RIGHT))
    
}

main()
{
    sleep 1000

    printf "Starting GPIO Example\n"
    
    qcm_gpio_configure_handler(GPIO_INPUT_SWITCH_LEFT, GPIO_EVENT_HIGH)
	qcm_gpio_configure_handler(GPIO_INPUT_SWITCH_RIGHT, GPIO_EVENT_HIGH)
    
}