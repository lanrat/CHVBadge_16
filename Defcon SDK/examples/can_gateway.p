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
 * QCM GWY CAN test
 *
 * ID 0x222 sent on CAN0 will show a message on the host console
 * ID 0x333 sent on CAN1 will show a message on the host console
 * Anything else sent on CAN1 will gateway to CAN0
 *
 *****************************************************************************/

@can0_rx0(rx_msg[QCM_CAN_MSG])
{
    printf("CAN0 Received 0x222\n")
    
}

@can1_rx0(rx_msg[QCM_CAN_MSG])
{
    printf("CAN1 Received 0x333\n")
    
}


main()
{
    sleep 1000
	
    printf "Starting CAN GWY Test\n"
    printf("Initialized\n");
    
	qcm_can_init(CAN0, 500000)
    qcm_can_init(CAN1, 500000)
    qcm_can_configure_rx_handler(CAN0, CAN_RX_HANDLER_0, 0x222, false)
    qcm_can_configure_rx_handler(CAN1, CAN_RX_HANDLER_0, 0x333, false)
    qcm_can_gw_mode(CAN1, CAN_GWY_MODE_PASS)
}
