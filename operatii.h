#pragma once
#include "Complex.h"
#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;


Mat FireDetect(Mat inMat, double th) {

	unsigned int height = inMat.rows;
	unsigned int width = inMat.cols;
	Mat outMat(inMat.rows, inMat.cols, inMat.type());

	for (unsigned int y = 0; y < height; y++)
		for (unsigned int x = 0; x < width; x++) {
			unsigned char r, g, b;
			float R, G, B, H, S, V, max, min, delta;

			//Aflam valorile HSV ale pixelului aplicand formulele de conversie RGB to HSV astfel: 

			// citim valorile RGB ale pixelului
			b = inMat.at<Vec3b>(y, x)[0];
			g = inMat.at<Vec3b>(y, x)[1];
			r = inMat.at<Vec3b>(y, x)[2];

			// Impartim valorile RGB la 255 fiecare pentru a obtine valori ale acestora in intervalul [0,1]  
			R = r / 255.0f;
			G = g / 255.0f;
			B = b / 255.0f;

			//Aflam valoarea minima si maxima dintre R,G,B ale pixelului 
			max = MAX(R, MAX(G, B));
			min = MIN(R, MIN(G, B));
			delta = max - min;

			if (delta == 0) {
				H = 0;
				S = 0;
			}
			else {
				if (max == R)
					H = 60.0f * ((G - B) / delta);
				if (max == G)
					H = 60.0f * (2.0f + (B - R) / delta);
				if (max == B)
					H = 60.0f * (4.0f + (R - G) / delta);
				S = (max == 0) ? 0 : 100.0f * delta / max;
			}
			if (H < 0)
			{
				H += 360.0F;
			}

			V = max * 100;
			// Daca pixelul are nuanta de culoare specifica focului sub 60% cu saturatia mai mare decat jumatate 
			//si se respecta regula r>g>b in care culoarea rosie are valoarea mai mare decat valoarea thresholdului
			// atunci consideram acel pixel ca facand parte din pixelii de foc si il evidentiam cu rosu
			if ((r >= th) && (r >= g) && (g >= b) && (H <=60) && (S>=50)) {
				outMat.at<Vec3b>(y, x)[0] = 0;    
				outMat.at<Vec3b>(y, x)[1] = 0;    
				outMat.at<Vec3b>(y, x)[2] = 255;  
			}
			// Daca pixelul nu respecta regulile de mai sus il vom considera un pixel non-foc si ii vom atribui culoarea
			// verde reprezentand background-ul imaginii
			else {
				outMat.at<Vec3b>(y, x)[0] = 0;
				outMat.at<Vec3b>(y, x)[1] = 45; 
				outMat.at<Vec3b>(y, x)[2] = 0;
			}
		}
	return outMat;
}

bool FireOnRect(Mat frame1, unsigned int X, unsigned int Y)
{
	for (unsigned int y = Y; y < Y + frame1.rows/8; y ++)
		for (unsigned int x = X; x < X + frame1.cols / 16; x++)
		{
			if (frame1.at<Vec3b>(y, x)[0] == 0 && frame1.at<Vec3b>(y, x)[1] == 0 && frame1.at<Vec3b>(y, x)[2] == 255)
			{
				return true;
			}
		}
	return false;
}


