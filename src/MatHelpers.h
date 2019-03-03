#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>

// opencv
#include <opencv2/opencv.hpp>


class MatHelpers
{

public:

	// https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
	static string Type2str(int type) {
		  string r;

		  uchar depth = type & CV_MAT_DEPTH_MASK;
		  uchar chans = 1 + (type >> CV_CN_SHIFT);

		  switch ( depth ) {
			case CV_8U:  r = "8U"; break;
			case CV_8S:  r = "8S"; break;
			case CV_16U: r = "16U"; break;
			case CV_16S: r = "16S"; break;
			case CV_32S: r = "32S"; break;
			case CV_32F: r = "32F"; break;
			case CV_64F: r = "64F"; break;
			default:     r = "User"; break;
		  }

		  r += "C";
		  r += (chans+'0');

		  return r;
	}
};