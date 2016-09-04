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
 * Shows how to make a simple CAN debugging tool
 *
 *****************************************************************************/
new c0tx
new c0rx
new c1tx
new c1rx

@host_rx(data[], data_size)
{
    new tx_msg[QCM_CAN_MSG]
    new i
    
    tx_msg.id = data[2]
    tx_msg.id += data[3] << 8
    tx_msg.id += data[4] << 16
    tx_msg.id += (data[5] << 24) & 0x1F
    
    tx_msg.dlc = data[6]


    if (data[5] & 0x80)
    {
        tx_msg.is_extended = true
    }
    else
    {
        tx_msg.is_extended = false
    }
    
    for (i = 0; i < 8; i++)
    {
        if (i < tx_msg.dlc)
        {
            tx_msg.data[i] = data[i + 10]
        }
        else
        {
            tx_msg.data[i] = 0
        }
    }
    /* This SHOULD be data[0], but for some reason data[0] == 17 */
    if (data[1] == 0)
    {
        c0tx++
        qcm_can_tx(CAN0, tx_msg)
    }
    else
    {
        c1tx++
        qcm_can_tx(CAN1, tx_msg)
    }
}

@timer0()
{
    new str[16]

    qcm_display_bar(0,0,127,127,graphics_make_rgb_565_color(31,63,31))

    qcm_display_write_text(0,0, 0, LCD_FONT_MEDIUM, "can0:")  
    strformat(str, sizeof(str), true, "  tx: %08d", c0tx)
    qcm_display_write_text(0,10, 0, LCD_FONT_MEDIUM, str)  
    strformat(str, sizeof(str), true, "  rx: %08d", c0rx)
    qcm_display_write_text(0,20, 0, LCD_FONT_MEDIUM, str)

    qcm_display_write_text(0,40, 0, LCD_FONT_MEDIUM, "can1:")  
    strformat(str, sizeof(str), true, "  tx: %08d", c1tx)
    qcm_display_write_text(0,50, 0, LCD_FONT_MEDIUM, str)  
    strformat(str, sizeof(str), true, "  rx: %08d", c1rx)
    qcm_display_write_text(0,60, 0, LCD_FONT_MEDIUM, str)

}

send_can_msg_to_host(rx_msg[QCM_CAN_MSG], channel)
{
    new rx_message[17]
	new i
    
	rx_message[0] = channel
	rx_message[1] = rx_msg.id;
    rx_message[2] = rx_msg.id >> 8;
    rx_message[3] = rx_msg.id >> 16;
    rx_message[4] = rx_msg.id >> 24;
    rx_message[5] = rx_msg.dlc;
    rx_message[6] = 0
    rx_message[7] = 0
    rx_message[8] = 0
    
	if (rx_msg.is_extended)
	{
		rx_message[4] |= 0x80
	}
    
    for (i = 0; i < 8; i++)
    {
        if (i < rx_msg.dlc)
        {
            rx_message[i + 9] = rx_msg.data[i]
        }
        else
        {
            rx_message[i + 9] = 0
        }
    }
	
    qcm_host_send(rx_message, 8)
}

@can0_rx_all(rx_msg[QCM_CAN_MSG])
{
    c0rx++
    send_can_msg_to_host(rx_msg, 0)	
}

@can1_rx_all(rx_msg[QCM_CAN_MSG])
{
    c1rx++
    send_can_msg_to_host(rx_msg, 1)
}

main()
{
    c0tx = 0
    c0rx = 0
    c1tx = 0
    c1rx = 0

    qcm_can_init(CAN0, 500000)
    qcm_can_init(CAN1, 500000)

    qcm_display_init()
    qcm_display_backlight(75)

    qcm_timer_start(TIMER_0,200,true)

}
