#include "WS2812Matrix.h"

void WS2812Matrix::fadeAbs(float value)
{
	if(value > 1.0 || value < 0.0)
	{
		return;
	}
	cRGB rgbValue;
	for (int i = 0; i < ledCount; i++)
	{
		rgbValue = pixel.get_crgb_at(i);
		rgbValue.r = 255 * value;
		rgbValue.g *= 255 * value;
		rgbValue.b *= 255 * value;
		pixel.set_crgb_at(i, rgbValue);
	}
	pixel.sync();
}

void WS2812Matrix::fadeRel(float value)
{
	if (value > 1.0 || value < 0.0)
	{
		return;
	}
	cRGB rgbValue;
	for (int i = 0; i < ledCount; i++)
	{
		rgbValue = pixel.get_crgb_at(i);
		rgbValue.r *= value;
		rgbValue.g *= value;
		rgbValue.b *= value;
		pixel.set_crgb_at(i, rgbValue);
	}
	pixel.sync();
}

WS2812Matrix::WS2812Matrix(int ledCount, int outputPin)
	: ledCount(ledCount),
	pixel(WS2812(ledCount))
{
	pixel.setOutput(outputPin);

	pixel.setColorOrderGRB(); // for sk6812 rgbw pixel

							  // Setting the Pixel to 0,0,0;
	cRGB rgbValue;
	rgbValue.r = 00;
	rgbValue.g = 00;
	rgbValue.b = 00;
	for (int i = 0; i < ledCount; i++)
	{
		pixel.set_crgb_at(i, rgbValue);
	}
}

void WS2812Matrix::setFrame(Frame* frame)
{
	RGBColor colorMatrix[8][6];
	cRGB rgbValue;
	frame->getSnailedFrameMatrix(colorMatrix);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			rgbValue.r = colorMatrix[i][j].getR();
			rgbValue.g = colorMatrix[i][j].getG();
			rgbValue.b = colorMatrix[i][j].getB();

			/*rgbValue.r = i*i;
			rgbValue.g = j*j;
			rgbValue.b = i*j + 20;*/

			pixel.set_crgb_at((i*6)+j, rgbValue);
		}
	}
	pixel.sync();
}
