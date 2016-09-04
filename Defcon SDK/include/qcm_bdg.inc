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
 * @brief QCM Native Functions for QCM-BDG-2016 (the DefCon 24 badge)
 *
 *****************************************************************************/
#pragma library QCM_BDG

/*****************************************************************************
 * Data structure definitions 
 ****************************************************************************/

/* 
 * Used to specify/receive CAN messages.
 * 
 * Members:
 * .id : CAN message ID
 * .is_extended : if true, message ID is 29-bit.  IF false, message id is 11-bit
 * .dlc : length of CAN message (0 through 8, inclusive)
 * .data : payload of CAN message (use .dlc to determine the valid data bytes)
 */ 
#define QCM_CAN_MSG[.id, bool:.is_extended, .dlc, .data[8]]

/*****************************************************************************
 * Tags (types) for use in calling the native routines 
 *****************************************************************************/

/* Input tags */
const GPIO_IN:GPIO_INPUT_SWITCH_LEFT = 0
const GPIO_IN:GPIO_INPUT_SWITCH_RIGHT = 1
const GPIO_IN:GPIO_INPUT_EXT_POWER = 2

/* Output tags */
const GPIO_OUT:GPIO_OUTPUT_EXHAUST = 0 /* be careful - this is a one shot deal. (unless you have some spare 100 ohm 0402 resistors :-).  Maybe put some vaping juice on the resistor first and PWM the output?) */

/* Input event tags */
const GPIO_EVENT:GPIO_EVENT_NONE = 0
const GPIO_EVENT:GPIO_EVENT_LOW = 1
const GPIO_EVENT:GPIO_EVENT_HIGH = 2
const GPIO_EVENT:GPIO_EVENT_ALL = 3

/* timer tags */
const TIMER:TIMER_0 = 0
const TIMER:TIMER_1 = 1
const TIMER:TIMER_2 = 2
const TIMER:TIMER_3 = 3
const TIMER:TIMER_4 = 4
const TIMER:TIMER_5 = 5

/* CAN receive handler tags */
const CAN_RX_HANDLER:CAN_RX_HANDLER_0 = 0
const CAN_RX_HANDLER:CAN_RX_HANDLER_1 = 1
const CAN_RX_HANDLER:CAN_RX_HANDLER_2 = 2
const CAN_RX_HANDLER:CAN_RX_HANDLER_3 = 3
const CAN_RX_HANDLER:CAN_RX_HANDLER_4 = 4
const CAN_RX_HANDLER:CAN_RX_HANDLER_5 = 5
const CAN_RX_HANDLER:CAN_RX_HANDLER_6 = 6
const CAN_RX_HANDLER:CAN_RX_HANDLER_7 = 7

/* CAN gateway mode tags */
const CAN_GWY_MODE:CAN_GWY_MODE_PASS = 0
const CAN_GWY_MODE:CAN_GWY_MODE_NO_PASS = 1

/* CAN channel tags */
const CAN_CHANNEL:CAN0 = 0
const CAN_CHANNEL:CAN1 = 1

/* LED identifiers */
const LED:LED_FRONT_BOTTOM = 0
const LED:LED_FRONT_TOP = 1
const LED:LED_REAR_BOTTOM = 2
const LED:LED_REAR_TOP = 3

/* LCD Font Types */
const LCD_FONT:LCD_FONT_MEDIUM = 0
const LCD_FONT:LCD_FONT_LARGE = 1

/********************************************************************************
 * System Native Routines 
 *******************************************************************************/

 /* returns number of milliseconds since system start */ 
native qcm_get_system_time() = -47

/********************************************************************************
 * Timer Routines
 *******************************************************************************/

/* 
 * Start a timer that will invoke associated event handler at expiration.  
 *
 * Arguments: 
 * timer_number : timer to start, must be of type TIMER (see above, for example TIMER_0)
 * timeout_ms : duration/period of timer, in milliseconds 
 * auto_reload : if true, then restart timer immediately on expiration, if false, do not restart timer on expiration
 *
 * Returns true if timer was started, false otherwise
 */
native bool:qcm_timer_start(TIMER:timer_number, timeout_ms, bool:auto_reload) = -48

/* 
 * Stop a previously started timer.
 *
 * Arguments: 
 * timer_number : timer to stop, must be of type TIMER (see above, for example TIMER_0)
 *
 * Returns true if timer was stopped, false otherwise
 */
native bool:qcm_timer_stop(TIMER:timer_number) = -49

/* 
 * Check to see if a timer is expired.
 *
 * Arguments: 
 * timer_number : timer to check, must be of type TIMER (see above, for example TIMER_0)
 *
 * Returns true if timer is expired, false if timer not expired, not started, or is set to auto_reload
 */
native bool:qcm_timer_expired(TIMER:timer_number) = -50

/********************************************************************************
 * CAN Routines
 *******************************************************************************/

/* 
 * Initialize CAN to the desired baud rate. 
 *
 * Arguments:
 * speed : Bit rate of CAN - can be 33333, 125000, 250000, 500000, or 1000000
 */
native qcm_can_init(CAN_CHANNEL:channel, speed) = -51

/*
 * Configures RX handler for specified ID
 *
 * Arguments:
 * channel : specifies the CAN channel to initialize, must be of type CAN_CHANNEL (see above, for example CAN0)
 * handler : specifies handler number for id, must be of type CAN_RX_HANDLER (see above, for example CAN_RX_HANDLER_0)
 * can_id : specifies ID of CAN message to invoke specified event handler upon reception. For example 0x7E8
 * is_extended : if true, can_id is a 29-bit identifier, if false it is an 11-bit identifier
 *
 * When specified message is received, then the associated event handler of the form @can_rx<n> will be invoked
 */
native qcm_can_configure_rx_handler(CAN_CHANNEL:channel, CAN_RX_HANDLER:handler, can_id, bool:is_extended) = -52

/* 
 * Transmit a CAN message
 *
 * Arguments:
 * channel : specifies the CAN channel to initialize, must be of type CAN_CHANNEL (see above, for example CAN0)
 * tx_msg : structure containing message to send.  See QCM_CAN_MSG above.
 */
native qcm_can_tx(CAN_CHANNEL:channel, tx_msg[QCM_CAN_MSG]) = -53

/* 
 * Sets the CAN channel gateway mode
 *
 * Arguments:
 * channel : specifies the CAN channel to initialize, must be of type CAN_CHANNEL (see above, for example CAN0)
 * mode : specifies the mode of the CAN gateway, must be of type CAN_GWY_MODE (see above, for example CAN_GWY_MODE_PASS)
 */
native qcm_can_gw_mode(CAN_CHANNEL:channel, CAN_GWY_MODE:mode) = -54

/********************************************************************************
 * GPIO Routines
 *******************************************************************************/

/*
 * Set an output to the specified state.
 *
 * Arguments:
 * channel : channel to set, must be of type GPIO_OUT.  For example, GPIO_OUTPUT_0
 * asserted : If true, specified channel is set to a logic 1 (3.3v).  
 *            If false, specified channel is set to a logic zero (0v)
 */
native qcm_gpio_set(GPIO_OUT:channel, bool:asserted) = -55
/*
 * Get the state of an input.
 *
 * Arguments:
 * channel : channel to get, must be of type GPIO_IN.  For example, GPIO_INPUT_0
 *
 * Returns true if input level is logic high, false otherwise
 */
native bool:qcm_gpio_get_input(GPIO_IN:channel) = -56

/*
 * Get the state of an output.
 *
 * Arguments:
 * channel : channel to get, must be of type GPIO_OUT.  For example, GPIO_OUTPUT_0
 *
 * Returns true if output level is logic high, false otherwise
 */
native bool:qcm_gpio_get_output(GPIO_OUT:channel) = -57

/* 
 * Registers event handler on an input
 *
 * Arguments:
 * channel : channel to monitor, must be of type GPIO_IN.  For example, GPIO_INPUT_0
 * event : input change condition to monitor.  Must be of type GPIO_EVENT.
 *
 * When specified event on specified channel occurs, event handler of the form @gpio_input<n> will be invoked.
 */
native qcm_gpio_configure_handler(GPIO_IN:channel, GPIO_EVENT:event) = -58

/********************************************************************************
 * LED Routines
 *******************************************************************************/
/* 
 * Sets an LED to a specified color.
 *
 * Arguments:
 * led : Specifies what LED to address, must be of type LED (for example, 
 *       LED_FRONT_TOP
 * color : a 16-bit color value in 5-6-5 format (i.e. rrrrrggggggbbbbb, 100% green is
 * 0b0000011111100000 or 0x07E0, white is 0b1111111111111111 or 0xFFFF)
 *
 * Also see graphics.inc for RGB565 color utilities
 *
 */
native qcm_led_set(LED:led_address, color) = -59

/********************************************************************************
 * Display Routines
 *
 * The LCD is a 128x128 display that uses 16-bit color in RGB565 format.
 *
 * The top left of the display is coordinate 0,0
 * X coordinates increase to 127 as pixels move to the right.
 * Y coordinates increase to 127 as pixes move down.
 *
 * Also see graphics.inc for PAWN-based graphics routines (line and 
 * color utilities).
 *
 *******************************************************************************/
/* 
 * Initializes LCD display, must be called before any other display routines are
 * called.
 *
 * Arguments:
 * None.
 */
native qcm_display_init() = -60
 
/* 
 * Sets PWM duty cycle of LCD backlight
 *
 * Arguments:
 * duty : value between 0 and 100, inclusive
 */
native qcm_display_backlight(duty) = -61

/* 
 * Writes a packed frame buffer to the LCD
 *
 * Each element of the passed array contains two 16-bit pixel values,
 * with the first pixel occupying the top 16-bits of the 32-bit cell.
 *
 * The origin specifies the top left corner of the content.
 *
 * Arguments:
 * x_origin : x location between 0 and 127, inclusive, to start placing the passed content
 * y_origin : y location between 0 and 127, inclusive, to start placing the passed content
 * clip_right_x : location between 0 and 127, inclusive, to bound the buffer being written
 * clip_lower_y : location between 0 and 127, inclusive, to bound the buffer being written
 * content : array containing packed 5-6-5 color data (2 pixels per cell)
 * content_size_pixels : the number of pixels passed in content, note that this can be
 * 		an odd value.
 */
native qcm_display_write_buffer(x_origin, y_origin, clip_right_x, clip_lower_y, 
	content[], content_size_pixels) = -62

/* 
 * The following routine is the same as the one above - it just accepts constant
 * bitmap content - PAWN needs this version to avoid compiler errors.  And this method 
 * is also needed to avoid exceeding the PAWN stack limit for large bitmap images (they are 
 * defined as constant data in global scope (see ImageCreate.pl in the tools subdirectory of the
 * SDK)- this keeps the data constant and it is not copied).
 */
native qcm_display_write_const_buffer(x_origin, y_origin, clip_right_x, clip_lower_y, 
	const content[], content_size_pixels) = -62

/* 
 * Sets region specified to specified color.
 *
 * Can be used to clear the display if coordinates of 0,0,127,127 are passed
 *
 * Arguments:
 * x_left : x location between 0 and 127, inclusive, for left of bar
 * y_top : x location between 0 and 127, inclusive, for top of bar
 * x_right : x location between 0 and 127, inclusive, for right of bar
 * y_bottom : x location between 0 and 127, inclusive, for bottom of bar
 * color : a 16-bit color value in 5-6-5 format (i.e. rrrrrggggggbbbbb, 100% green is
 * 0b0000011111100000 or 0x07E0, white is 0b1111111111111111 or 0xFFFF)
 */	
native qcm_display_bar(x_left, y_top, x_right, y_bottom, color) = -63

/* 
 * Sets a single pixel on the LCD
 *
 * note that it is much more efficient to call qcm_display_write_buffer() to send
 * an entire buffer than to set each pixel individually.  
 *
 * Arguments:
 * x : x location between 0 and 127, inclusive, to set pixel
 * y : y location between 0 and 127, inclusive, to set pixel
 * color : a 16-bit color value in 5-6-5 format (i.e. rrrrrggggggbbbbb, 100% green is
 * 0b0000011111100000 or 0x07E0, white is 0b1111111111111111 or 0xFFFF)
 */
native qcm_display_set_pixel(x, y, color) = -64

/* 
 * Write a text string to the LCD
 *
 * x and y specify the top left corner of the text to write.
 *
 * Medium font is 16 pixels high
 * Large font is 33 pixels high
 *
 * Arguments:
 * x : x location between 0 and 127, inclusive, left corner of text
 * y : y location between 0 and 127, inclusive, top corner of text
 * color : a 16-bit color value in 5-6-5 format (i.e. rrrrrggggggbbbbb, 100% green is
 * 0b0000011111100000 or 0x07E0, white is 0b1111111111111111 or 0xFFFF)
 * font_size : font to use, must be of type LCD_FONT (for example LCD_FONT_MEDIUM)
 * text : string to write, can be either packed or unpacked
 */
native qcm_display_write_text(x, y, color, LCD_FONT:font_type, const text[]) = -65


/********************************************************************************
 * Host USB routines 
 *******************************************************************************/
/* 
 * Write binary data to the host USB endpoint.  
 *
 * All data will be sent using the debug command ID 0x01.  See the USB interface
 * specification for more information.  Maximum send size is 128 bytes.  Multiple
 * USB frames will be generated for any size over 63 bytes.
 *
 * Arguments:
 * data : array of data to be written
 * data_width: specifies number of bits used for data in each cell of data array,
 * can be 8, 16, 24, or 32.  Data will be written most significant byte first for
 * sizes greater than 8.
 * data_length: number of elements to write from data array (note - not number of 
 * bytes).
 */
native qcm_host_send(data[], data_width, data_length = sizeof data) = -66

/******************************************************************************
 * Forward declarations for event handlers 
 *****************************************************************************/

/* timer event handlers */
forward @timer0()
forward @timer1()
forward @timer2()
forward @timer3()
forward @timer4()
forward @timer5()

/* 
 * If present in the script, then all messages not matched by any handlers registered with 
 * qcm_can_configure_rx_handler() are passed to this event handler.  Registered event handlers
 * will continue to get the ID's that they are registered for.  This can be thought of as a handler
 * to catch "whatever ID's are left over" after the individual handlers are registered.
 */
forward @can0_rx_all(rx_msg[QCM_CAN_MSG])
forward @can1_rx_all(rx_msg[QCM_CAN_MSG])

/* CAN message ID event handlers for channel CAN0 */
forward @can0_rx0(rx_msg[QCM_CAN_MSG])
forward @can0_rx1(rx_msg[QCM_CAN_MSG])
forward @can0_rx2(rx_msg[QCM_CAN_MSG])
forward @can0_rx3(rx_msg[QCM_CAN_MSG])
forward @can0_rx4(rx_msg[QCM_CAN_MSG])
forward @can0_rx5(rx_msg[QCM_CAN_MSG])
forward @can0_rx6(rx_msg[QCM_CAN_MSG])
forward @can0_rx7(rx_msg[QCM_CAN_MSG])

/* CAN message ID event handlers for channel CAN1 */
forward @can1_rx0(rx_msg[QCM_CAN_MSG])
forward @can1_rx1(rx_msg[QCM_CAN_MSG])
forward @can1_rx2(rx_msg[QCM_CAN_MSG])
forward @can1_rx3(rx_msg[QCM_CAN_MSG])
forward @can1_rx4(rx_msg[QCM_CAN_MSG])
forward @can1_rx5(rx_msg[QCM_CAN_MSG])
forward @can1_rx6(rx_msg[QCM_CAN_MSG])
forward @can1_rx7(rx_msg[QCM_CAN_MSG])

/* switch status change event handlers */
forward @gpio_input_switch_left()
forward @gpio_input_switch_right()

/* external power change event handler */
forward @gpio_input_ext_power()

/* 
 * This event handler is invoked with the data received on the host 
 * USB endpoint.  Passed data is one byte per cell and not packed.
 * Note that the USB command ID of 0x01 is not passed (see document 
 * titled "USB Protocol.txt" in doc subfolder of SDK).
 *
 */
forward @host_rx(data[], data_size);

