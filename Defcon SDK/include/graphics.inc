/******************************************************************************
 * Copyright 2016 Specialized Solutions LLC
 *
 * Title to the Materials (contents of this file) remain with Specialized
 * Solutions LLC.  The Materials are copyrighted and are protected by United
 * States copyright laws.  Copyright notices cannot be removed from the
 * Materials.
 *
 * @brief QCM Graphics Library for QCM-BDG-2016
 *
 *****************************************************************************/
#pragma library QCM_Graphics

stock graphics_line(x1, y1, x2, y2, line_color = 0xFFFF)
{
	new  deltaX, deltaY
    new   error, stepErrorLT, stepErrorGE
    new   stepX, stepY
    new   steep
    new   temp
	
	if(x1 == x2)
    {
        if(y1 > y2)
        {
            temp = y1
            y1 = y2
            y2 = temp
        }

        for(temp = y1; temp < y2 + 1; temp++)
        {
            qcm_display_set_pixel(x1, temp, line_color)
        }
    }
	else if(y1 == y2)
    {
        if(x1 > x2)
        {
            temp = x1
            x1 = x2
            x2 = temp
        }

        for(temp = x1; temp < x2 + 1; temp++)
        {
            qcm_display_set_pixel(temp, y1, line_color)
            
        }
    }
	else
	{
		stepX = 0
		deltaX = x2 - x1
		if(deltaX < 0)
		{
			deltaX = -deltaX
			--stepX
		}
		else
		{
			++stepX
		}

		stepY = 0
		deltaY = y2 - y1
		if(deltaY < 0)
		{
			deltaY = -deltaY
			--stepY
		}
		else
		{
			++stepY
		}

		steep = 0;
		if(deltaX < deltaY)
		{
			++steep
			temp = deltaX
			deltaX = deltaY
			deltaY = temp
			temp = x1
			x1 = y1
			y1 = temp
			temp = stepX
			stepX = stepY
			stepY = temp
			qcm_display_set_pixel(y1, x1, line_color)
		}
		else
		{
			qcm_display_set_pixel(x1, y1, line_color)
		}

		stepErrorGE = deltaX << 1

		stepErrorLT = deltaY << 1

		error = stepErrorLT - deltaX

		while(--deltaX >= 0)
		{
			if(error >= 0)
			{
				y1 += stepY
				error -= stepErrorGE
			}

			x1 += stepX
			error += stepErrorLT

			if(steep)
			{
				qcm_display_set_pixel(y1, x1, line_color)
			}
			else
			{
				qcm_display_set_pixel(x1, y1, line_color)
				
			}
		}   // end of while
	}

}

stock graphics_box(x_right, y_top, x_left, y_bottom, line_color)
{
	graphics_line(x_right, y_top, x_left, y_top, line_color)
	graphics_line(x_left, y_top, x_left, y_bottom, line_color)
	graphics_line(x_right, y_bottom, x_left, y_bottom, line_color)
	graphics_line(x_right, y_top, x_right, y_bottom, line_color)
}

/* colors are passed as 565 components
 * max red is 31
 * max green is 63
 * max blue is 31
 */
stock graphics_make_rgb_565_color(rgb_red, rgb_green, rgb_blue)
{
	return ((rgb_red & 0x1F) << 12) | ((rgb_green & 0x3F) << 5) | (rgb_blue & 0x1F)
}
