/******************************************************************************
 * @file
 *
 * Copyright 2012-2016 Specialized Solutions LLC
 *
 * Title to the Materials (contents of this file) remain with Specialized
 * Solutions LLC.  The Materials are copyrighted and are protected by United
 * States copyright laws.  Copyright notices cannot be removed from the
 * Materials.
 *
 * See the file titled "Specialized Solutions LLC License Agreement.txt"
 * that has been distributed with this file for further licensing details.
 *
 * It will take a little bit of work to get this to compile - this is an example.
 *
 * @brief CANBUS Management Routines
 *
 *****************************************************************************/

/******************************************************************************
 * Macros and Constants
 *****************************************************************************/

#define FMR_FINIT         ((uint32_t)0x00000001) /* Filter init mode */

/******************************************************************************
 * Typedefs
 *****************************************************************************/

/******************************************************************************
 * Local Function Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/******************************************************************************
 * Global Variables
 *****************************************************************************/

/******************************************************************************
 * Functions
 *****************************************************************************/
static void configure_filters(void)
{
    int i;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;


    /* CAN1 uses filters 0-13,  CAN2 used 14-27 */
    CAN_SlaveStartBank(14);

    /* initialize all filters (except 13 and 27) to be 32-bit, ID-based */
    for (i = 0; i < 14; i++)
    {
        CAN_FilterInitStructure.CAN_FilterNumber = i;
        CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
        CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
        CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
        /* split the filters up between the FIFO's */
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
        CAN_FilterInitStructure.CAN_FilterActivation = DISABLE;
        CAN_FilterInit(&CAN_FilterInitStructure);
    }


    for (i = 14; i < 28; i++)
    {
        CAN_FilterInitStructure.CAN_FilterNumber = i;
        CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
        CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
        CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
        CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
        /* split the filters up between the FIFO's */
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
        CAN_FilterInitStructure.CAN_FilterActivation = DISABLE;
        CAN_FilterInit(&CAN_FilterInitStructure);
    }

}

/* 
 * Common values are:
 * 500000k:
 * sjw = 1;
 * bs1 = 8;
 * bs2 = 6;
 * tq = 4;
 *
 * 250000:
 * sjw = 1;
 * bs1 = 15;
 * bs2 = 8;
 * tq = 5;
 */
void can_init(uint16_t const tq_prescale, uint8_t const sjw,
              uint8_t const bs1, uint8_t const bs2)

{
	int i;

	CAN_InitTypeDef CAN_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

    /* CAN1 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1);

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	CAN_DeInit(CAN1);

	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = sjw - 1;

	/* CAN Baudrate = 1MBps (CAN clocked at 30 MHz) */
	CAN_InitStructure.CAN_BS1 = bs1 - 1;
	CAN_InitStructure.CAN_BS2 = bs2 - 1;
	CAN_InitStructure.CAN_Prescaler = tq_prescale;
	CAN_Init(CAN1, &CAN_InitStructure);

	/* turn on the xvr */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, 0);
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* CAN2 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	CAN_DeInit(CAN2);

	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = sjw - 1;

	/* CAN Baudrate = 1MBps (CAN clocked at 30 MHz) */
	CAN_InitStructure.CAN_BS1 = bs1 - 1;
	CAN_InitStructure.CAN_BS2 = bs2 - 1;
	CAN_InitStructure.CAN_Prescaler = tq_prescale;
	CAN_Init(CAN2, &CAN_InitStructure);

	configure_filters();

	/* turn on the xvr */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, 0);
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    }

}

void can_configure_id_filter(uint8_t channel, uint8_t const filter_id, uint32_t const can_id, BOOL const ide)
{
	uint32_t filter_number_bit_pos = 0;
	uint32_t filter_num = filter_id / 2;
    uint32_t internal_can_id;

    if (channel == 1)
    {
        filter_num += 14;
    }

    if (ide)
    {
        internal_can_id = 0x00000004;
        internal_can_id |= can_id << 3;
    }
    else
    {
        internal_can_id = can_id << 21;
    }

	filter_number_bit_pos = ((uint32_t)1) << (filter_num);

	/* Initialisation mode for the filter */
	CAN1->FMR |= FMR_FINIT;

	/* Filter Deactivation */
	CAN1->FA1R &= ~(uint32_t)filter_number_bit_pos;

	/* Filter Scale */

	/* 32-bit scale for the filter */
	CAN1->FS1R |= filter_number_bit_pos;
	CAN1->FM1R |= filter_number_bit_pos;

	/* 32-bit identifier or First 32-bit identifier */
	if (filter_id % 2)
	{
		CAN1->sFilterRegister[filter_num].FR2 = internal_can_id;
	}
	else
	{
		/* 32-bit mask or Second 32-bit identifier */
		CAN1->sFilterRegister[filter_num].FR1 = internal_can_id;
	}

	CAN1->FA1R |= filter_number_bit_pos;

	/* Leave the initialisation mode for the filter */
	CAN1->FMR &= ~FMR_FINIT;

}

BOOL can_send_message(uint8_t const channel, CanTxMsg const * const tx_message)
{
    CAN_TypeDef *can_channel;

	if (channel == 0)
	{
        can_channel = CAN1;
	}
	else
	{
        can_channel = CAN2;
	}

    CAN_Transmit(can_channel, tx_message);

}

BOOL can_receive_message(CanRxMsg * const rx_message)
{
	BOOL ret_val = FALSE;

    if (0 < CAN_MessagePending(CAN1, 0))
	{
		CAN_Receive(CAN1, 0, rx_message);
		ret_val = TRUE;
	}
	else if (0 < CAN_MessagePending(CAN2, 0))
	{
		CAN_Receive(CAN2, 0, rx_message);
		ret_val = TRUE;
	}

	return ret_val;

}
