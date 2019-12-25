#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <random>


using namespace std;


class RandomNumbers
{
public:

	static vector<float> GetNumbers(int N, float min, float max);


	static vector<int> GetNumbersInt(int N, int min, int max);


};