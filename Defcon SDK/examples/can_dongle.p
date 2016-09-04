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
@can0_rx_all(rx_msg[QCM_CAN_MSG])
{
	new rx_message[5]
	
	rx_message[0] = qcm_get_system_time()
	rx_message[1] = rx_msg.id;
	rx_message[2] = rx_msg.dlc;
	if (rx_message.is_extended)
	{
		rx_message[2] |= 0x100
	}
	
	rx_message[3] = rx_message.data[0] << 24
	rx_message[3] |= rx_message.data[1] << 16
	rx_message[3] |= rx_message.data[2] << 8
	rx_message[3] |= rx_message.data[3]
	rx_message[4] = rx_message.data[4] << 24
	rx_message[4] |= rx_message.data[5] << 16
	rx_message[4] |= rx_message.data[6] << 8
	rx_message[4] |= rx_message.data[7]
	
	host_send(rx_message, 32)
	
}

@can1_rx_all(rx_msg[QCM_CAN_MSG])
{
	new rx_message[5]
	
	rx_message[0] = qcm_get_system_time()
	rx_message[1] = rx_msg.id;
	rx_message[2] = rx_msg.dlc;
	if (rx_message.is_extended)
	{
		rx_message[2] |= 0x100
	}
	
	rx_message[3] = rx_message.data[0] << 24
	rx_message[3] |= rx_message.data[1] << 16
	rx_message[3] |= rx_message.data[2] << 8
	rx_message[3] |= rx_message.data[3]
	rx_message[4] = rx_message.data[4] << 24
	rx_message[4] |= rx_message.data[5] << 16
	rx_message[4] |= rx_message.data[6] << 8
	rx_message[4] |= rx_message.data[7]
	
	host_send(rx_message, 32)
}
main()
{
	qcm_can_init(CAN0, 500000)
    qcm_can_init(CAN1, 500000)
}
