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
 * @brief LCD Management Routines
 *
 *****************************************************************************/

void display_reset(void)
{
    uint8_t cmd[1];
    uint8_t data[16];
    uint16_t i,j;

    /* turn on panel */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* configure inputs */
    GPIO_InitTypeDef gpio = {
            .GPIO_Mode  = GPIO_Mode_OUT,
            .GPIO_Speed = GPIO_Speed_2MHz,
            .GPIO_OType = GPIO_OType_PP,
            .GPIO_PuPd = GPIO_PuPd_NOPULL
    };

	/* configure reset line: PB2*/
    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_OType = GPIO_OType_PP;
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, 1);
    GPIO_Init(GPIOB, &gpio);

    delay_ms(100);
    GPIO_WriteBit(GPIOB, GPIO_Pin_2, 0);
    delay_ms(5);
    GPIO_WriteBit(GPIOB, GPIO_Pin_2, 1);
    delay_ms(10);

    //LCD Init For 1.44Inch LCD Panel with ST7735R.
    cmd[0] = 0x11;
    spi_lcd_transaction(cmd, 1, NULL, 0);
    delay_ms(120);

    //ST7735R Frame Rate
    data[0] = 0x01;
    data[1] = 0x2C;
    data[2] = 0x2D;
    cmd[0] = 0xB1;

    spi_lcd_transaction(cmd, 1, data, 3);

    cmd[0] = 0xB2;
    spi_lcd_transaction(cmd, 1, data, 3);


    data[0] = 0x01;
    data[1] = 0x2C;
    data[2] = 0x2D;
    data[3] = 0x01;
    data[4] = 0x2C;
    data[5] = 0x2D;
    cmd[0] = 0xB3;
    spi_lcd_transaction(cmd, 1, data, 6);

    data[0] = 0x07;
    cmd[0] = 0xB4;
    spi_lcd_transaction(cmd, 1, data, 1);

    //ST7735R Power Sequence
    data[0] = 0xA2;
    data[1] = 0x02;
    data[2] = 0x84;
    cmd[0] = 0xC0;
    spi_lcd_transaction(cmd, 1, data, 3);

    data[0] = 0xC5;
    cmd[0] = 0xC1;
    spi_lcd_transaction(cmd, 1, data, 1);

    data[0] = 0x0A;
    data[1] = 0x00;
    cmd[0] = 0xC2;
    spi_lcd_transaction(cmd, 1, data, 2);

    data[0] = 0x8A;
    data[1] = 0x2A;
    cmd[0] = 0xC3;
    spi_lcd_transaction(cmd, 1, data, 2);

    data[0] = 0x8A;
    data[1] = 0xEE;
    cmd[0] = 0xC4;
    spi_lcd_transaction(cmd, 1, data, 2);

    data[0] = 0x0E;
    cmd[0] = 0xC5;
    spi_lcd_transaction(cmd, 1, data, 1);

    data[0] = 0xC8;
    cmd[0] = 0x36;
    spi_lcd_transaction(cmd, 1, data, 1);

    //ST7735R Gamma Sequence
    data[0] = 0x0F;
    data[1] = 0x1A;
    data[2] = 0x18;
    data[3] = 0x2F;
    data[4] = 0x20;
    data[5] = 0x28;
    data[6] = 0x20;
    data[7] = 0x22;
    data[8] = 0x1F;
    data[9] = 0x1B;
    data[10] = 0x23;
    data[11] = 0x37;
    data[12] = 0x00;
    data[13] = 0x07;
    data[14] = 0x02;
    data[15] = 0x10;
    cmd[0] = 0xE0;
    spi_lcd_transaction(cmd, 1, data, 16);

    data[0] = 0x0F;
    data[1] = 0x1B;
    data[2] = 0x0F;
    data[3] = 0x17;
    data[4] = 0x33;
    data[5] = 0x2C;
    data[6] = 0x29;
    data[7] = 0x2E;
    data[8] = 0x30;
    data[9] = 0x30;
    data[10] = 0x39;
    data[11] = 0x3F;
    data[12] = 0x00;
    data[13] = 0x07;
    data[14] = 0x03;
    data[15] = 0x10;
    cmd[0] = 0xE1;
    spi_lcd_transaction(cmd, 1, data, 16);

    data[0] = 0x01;
    data[1] = 0xF6;
    data[2] = 0x00;
    cmd[0] = 0xF0;
    spi_lcd_transaction(cmd, 1, data, 3);

    data[0] = 0x05;
    cmd[0] = 0x3A;
    spi_lcd_transaction(cmd, 1, data, 1);

    cmd[0] = 0x29;
    spi_lcd_transaction(cmd, 1, NULL, 0);

    display_clear_to_color(0);

    /* setup the backlight on timer 5 channel 2 */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);

    /* Time Base configuration - PWM at 100 Hz*/
    /* Timer runs at 120e6 / 2 */
    TIM_TimeBaseStructure.TIM_Period = (120000000 / 2 / 6000 / 100);
    TIM_TimeBaseStructure.TIM_Prescaler = 6000 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    /* Channel 2 in PWM mode */
    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM_OC2Init(TIM5, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM5, ENABLE);
    TIM_CtrlPWMOutputs(TIM5, ENABLE);

    /* counter enable */
    TIM_Cmd(TIM5, ENABLE);

    display_backlight_duty(75);
}

void display_backlight_duty(uint8_t const duty)
{
    uint32_t TimerPeriod;
    uint16_t ChannelPulse;

	TimerPeriod = (120000000 / 2 / 6000 / 100);

	/* LED is active low, so need to invert duty) */
    ChannelPulse = (uint16_t) (((uint32_t) (100 - duty) * (TimerPeriod)) / 100);

    TIM_SetCompare2(TIM5, ChannelPulse);

}

void display_put_pixel(uint8_t const x, uint8_t const y, uint16_t const color)
{
    uint32_t i;
    uint8_t data[4];
    uint8_t cmd[1];

    if ((x > 127) || (y > 127))
    {
        return;
    }

    data[0] = (x + 2) >> 8;
    data[1] = x + 2;
    data[2] = (0x7F + 2) >> 8;
    data[3] = 0x7F+2;
    cmd[0] = 0x2A;
    spi_lcd_transaction(cmd, 1, data, 4);

    data[0] = (y + 3) >> 8;
    data[1] = y + 3;
    data[2] = (0x7F + 3) >> 8;
    data[3] = 0x7F + 3;
    cmd[0] = 0x2B;
    spi_lcd_transaction(cmd, 1, data, 4);

    cmd[0] = 0x2C;
    spi_lcd_transaction(cmd, 1, NULL, 0);

    spi_lcd_control_ss(FALSE);

    data[0] = color >> 8;
    data[1] = color;

    spi_lcd_write_raw(data, 2);

    spi_lcd_control_ss(TRUE);

}

void display_clear_to_color(uint16_t const color)
{
    uint32_t i;
    uint8_t data[4];
    uint8_t cmd[1];

    data[0] = 0x00;
    data[1] = 0x00 + 2;
    data[2] = (0x7F + 2) >> 8;
    data[3] = 0x7F + 2;
    cmd[0] = 0x2A;
    spi_lcd_transaction(cmd, 1, data, 4);

    data[0] = 0x00;
    data[1] = 0x00 + 3;
    data[2] = (0x7F + 3) >> 8;
    data[3] = 0x7F + 3;
    cmd[0] = 0x2B;
    spi_lcd_transaction(cmd, 1, data, 4);

    cmd[0] = 0x2C;
    spi_lcd_transaction(cmd, 1, NULL, 0);

    spi_lcd_control_ss(FALSE);

    for(i = 0; i < (128 * 128); i++)
    {
        data[0] = color >> 8;
        data[1] = color;

        spi_lcd_write_raw(data, 2);
    }

    spi_lcd_control_ss(TRUE);
}

void display_write_framebuffer(uint8_t const x_origin, uint8_t const y_origin,
							   uint8_t const clip_left_x, uint8_t const clip_lower_y,
							   uint16_t const * const content, uint16_t const content_size_pixels)
{
    uint32_t i;
    uint8_t data[4];
    uint8_t cmd[1];

    if ((x_origin > 127) || (y_origin > 127))
    {
        return;
    }

    data[0] = (x_origin + 2) >> 8;
    data[1] = (x_origin + 2);
    data[2] = (clip_left_x + 2) >> 8;
    data[3] = clip_left_x + 2;
    cmd[0] = 0x2A;
    spi_lcd_transaction(cmd, 1, data, 4);

    data[0] = (y_origin + 3) >> 8;
    data[1] = y_origin + 3;
    data[2] = (clip_lower_y + 3) >> 8;
    data[3] = clip_lower_y + 3;
    cmd[0] = 0x2B;
    spi_lcd_transaction(cmd, 1, data, 4);

    cmd[0] = 0x2C;
    spi_lcd_transaction(cmd, 1, NULL, 0);

    spi_lcd_control_ss(FALSE);

    for(i = 0; i < content_size_pixels; i++)
    {
        data[0] = content[i] >> 8;
		data[1] = content[i];

        spi_lcd_write_raw(data, 2);
    }

    spi_lcd_control_ss(TRUE);

}


