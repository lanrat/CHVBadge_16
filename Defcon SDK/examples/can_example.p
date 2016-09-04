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
 * QCM CAN Example
 *
 *****************************************************************************/

@can0_rx0(rx_msg[QCM_CAN_MSG])
{
	new i;

	printf("CAN0 Msg Received: %x, %d, %d : ", rx_msg.id, rx_msg.is_extended, rx_msg.dlc)

	for (i = 0; i < rx_msg.dlc; i++)
	{
		printf("%02x ", rx_msg.data[i])
	}

	printf("\r\n");

	rx_msg.id -= 1

	qcm_can_tx(CAN1, rx_msg);


}


@can1_rx0(rx_msg[QCM_CAN_MSG])
{
	new i;

	printf("CAN1 Msg Received: %x, %d, %d : ", rx_msg.id, rx_msg.is_extended, rx_msg.dlc)

	for (i = 0; i < rx_msg.dlc; i++)
	{
		printf("%02x ", rx_msg.data[i])
	}


	printf("\r\n");

	rx_msg.id -= 1

	qcm_can_tx(CAN0, rx_msg);

}

main()
{
	sleep 1000
    printf "Starting CAN Example\n"
	qcm_can_init(CAN0, 500000);
	qcm_can_init(CAN1, 500000);
	qcm_can_configure_rx_handler(CAN0, CAN_RX_HANDLER_0, 0x05, false);
	qcm_can_configure_rx_handler(CAN1, CAN_RX_HANDLER_1, 0x04, true);

}

