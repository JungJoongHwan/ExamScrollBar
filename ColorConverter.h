#pragma once

#include "Gdiplus.h"
using namespace Gdiplus;

class CColorConverter
{
public:
	CColorConverter()	{};
	~CColorConverter()	{};

	static Color GetConvertedColor(Color clOrg)
	{
		return RGBToHSV((double)clOrg.GetRed(), (double)clOrg.GetGreen(), (double)clOrg.GetBlue());
	}
		
	static Color RGBToHSV(double dR, double dG, double dB)
	{
		double var_R = (dR / 255);						//RGB from 0 to 255
		double var_G = (dG / 255);
		double var_B = (dB / 255);

		double var_Min = min(min(var_R, var_G), var_B);	//Min. value of RGB
		double var_Max = max(max(var_R, var_G), var_B);	//Max. value of RGB
		double del_Max = var_Max - var_Min;					//Delta RGB value 

		double V = var_Max;

		double H, S;
		if (del_Max == 0)								//This is a gray, no chroma...
		{
			H = 0;                                //HSV results from 0 to 1
			S = 0;
		}
		else                                    //Chromatic data...
		{
			S = del_Max / var_Max;

			double del_R = (((var_Max - var_R) / 6) + (del_Max / 2)) / del_Max;
			double del_G = (((var_Max - var_G) / 6) + (del_Max / 2)) / del_Max;
			double del_B = (((var_Max - var_B) / 6) + (del_Max / 2)) / del_Max;

			if (var_R == var_Max) H = del_B - del_G;
			else if (var_G == var_Max) H = (1.0F / 3) + del_R - del_B;
			else if (var_B == var_Max) H = (2.0F / 3) + del_G - del_R;

			if (H < 0) H += 1;
			if (H > 1) H -= 1;
		}

		return HSVToRGB(H, S, V - 0.5);
	}


	static Color HSVToRGB(double H, double S, double V)
	{
		double R, G, B;
		if (S == 0)								//HSV from 0 to 1
		{
			R = V * 255;
			G = V * 255;
			B = V * 255;
		}
		else
		{
			double var_h = H * 6;
			if (var_h == 6) var_h = 0;				//H must be < 1
			int var_i = int(var_h);				//Or ... var_i = floor( var_h )
			double var_1 = V * (1 - S);
			double var_2 = V * (1 - S * (var_h - var_i));
			double var_3 = V * (1 - S * (1 - (var_h - var_i)));

			double var_r, var_g, var_b;
			if (var_i == 0) { var_r = V; var_g = var_3; var_b = var_1; }
			else if (var_i == 1) { var_r = var_2; var_g = V; var_b = var_1; }
			else if (var_i == 2) { var_r = var_1; var_g = V; var_b = var_3; }
			else if (var_i == 3) { var_r = var_1; var_g = var_2; var_b = V; }
			else if (var_i == 4) { var_r = var_3; var_g = var_1; var_b = V; }
			else                   { var_r = V; var_g = var_1; var_b = var_2; }

			R = var_r * 255;							//RGB results from 0 to 255
			G = var_g * 255;
			B = var_b * 255;
		}

		return Color((BYTE)R, (BYTE)G, (BYTE)B);
	}
};
