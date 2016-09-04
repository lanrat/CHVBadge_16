/******************************************************************************
 * @file
 *
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
 * It will take a little bit of work to get this to compile - this is an example.
 * @brief WS2812 management routines
 *
 *****************************************************************************/

/******************************************************************************
 * Macros and Constants
 *****************************************************************************/
#define CPU_DISABLE_INTERRUPTS()				__asm volatile 	( " cpsid i " )
#define CPU_ENABLE_INTERRUPTS()					__asm volatile 	( " cpsie i " )

/* For the badges with White WS2812's, these following values need to be doubled */
#define ZERO_CODE_HIGH_DELAY (1)
#define ZERO_CODE_LOW_DELAY (3)
#define ONE_CODE_HIGH_DELAY (6)
#define ONE_CODE_LOW_DELAY (1)

/******************************************************************************
 * Typedefs
 *****************************************************************************/

/******************************************************************************
 * Local Function Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/
static uint8_t led_settings[4][3];

/******************************************************************************
 * Global Variables
 *****************************************************************************/

/******************************************************************************
 * Functions
 *****************************************************************************/
void ws2812_init( void )
{
	memset(led_settings, 0xFF, sizeof(led_settings));

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* configure inputs */
    GPIO_InitTypeDef gpio = {
            .GPIO_Mode  = GPIO_Mode_OUT,
            .GPIO_Speed = GPIO_Speed_25MHz,
            .GPIO_OType = GPIO_OType_PP,
            .GPIO_PuPd = GPIO_PuPd_NOPULL
    };


	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, 0);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, 0);
    GPIO_Init(GPIOB, &gpio);

    ws2812_set_rgb(0, 0, 0, 0);
    ws2812_set_rgb(1, 0, 0, 0);
    ws2812_set_rgb(2, 0, 0, 0);
    ws2812_set_rgb(3, 0, 0, 0);

}

void ws2812_set_rgb(uint8_t const position, uint8_t const red, uint8_t const green, uint8_t const blue)
{
    volatile uint32_t chain_pin;
    volatile uint8_t rgb[3];
    volatile int i,j,k,l;
    volatile uint8_t start_index;

    if ((led_settings[position][0] == red) &&
        (led_settings[position][1] == green) &&
        (led_settings[position][2] == blue))
    {
        /* no change */
        return;
    }

    led_settings[position][0] = red;
    led_settings[position][1] = green;
    led_settings[position][2] = blue;

    if (position < 2)
    {
        chain_pin = GPIO_Pin_0;
        start_index = 0;
    }
    else
    {
        chain_pin = GPIO_Pin_1;
        start_index = 2;
    }

    CPU_DISABLE_INTERRUPTS();

    for (l = 0; l < 2; l++)
    {
        rgb[1] = ((uint16_t)0xFF * (uint16_t)led_settings[start_index + l][0]) / (uint16_t)100;
        rgb[0] = ((uint16_t)0xFF * (uint16_t)led_settings[start_index + l][1]) / (uint16_t)100;
        rgb[2] = ((uint16_t)0xFF * (uint16_t)led_settings[start_index + l][2]) / (uint16_t)100;
        i = 0;

        do
        {
            j = 8;
            do
            {
                if (rgb[i] & 0x80)
                {
                    GPIO_WriteBit(GPIOB, chain_pin, 1);

                    k = ONE_CODE_HIGH_DELAY;
                    do
                    {
                        k--;
                    }
                    while (k > 0);

                    GPIO_WriteBit(GPIOB, chain_pin, 0);
                    k = ONE_CODE_LOW_DELAY;
                    do
                    {
                        k--;
                    }
                    while (k > 0);

                }
                else
                {
                    GPIO_WriteBit(GPIOB, chain_pin, 1);
                    k = ZERO_CODE_HIGH_DELAY;
                    do
                    {
                        k--;
                    }
                    while (k > 0);

                    GPIO_WriteBit(GPIOB, chain_pin, 0);
                    k = ZERO_CODE_LOW_DELAY;
                    do
                    {
                        k--;
                    }
                    while (k > 0);
                }

                rgb[i] <<= 1;
                j--;
            }
            while (j > 0);
            i++;
        }
        while (i < 3);
    }

    /* wait 50 us */
    k = 500;
    do
    {
        k--;
    }
    while (k > 0);

    CPU_ENABLE_INTERRUPTS();

}


