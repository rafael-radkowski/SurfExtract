#include "RandomNumbers.h"



vector<float> RandomNumbers::GetNumbers(int N, float min, float max)
{
	vector<float> numbers;
	numbers.reserve(N);

	std::random_device rd;

    //
    // Engines 
    //
    std::mt19937 engine(rd());
    //std::knuth_b e2(rd());
    //std::default_random_engine e2(rd()) ;

    //
    // Distribtuions
    //
    std::uniform_real_distribution<float> dist(min, max);
    //std::normal_distribution<> dist(2, 2);
    //std::student_t_distribution<> dist(5);
    //std::poisson_distribution<> dist(2);
    //std::extreme_value_distribution<> dist(0,2);


	for (int i = 0; i < N; i++) {

		numbers.push_back(dist(engine));
	}

	return numbers;

}


//static 
vector<int> RandomNumbers::GetNumbersInt(int N, int min, int max)
{
	vector<int> numbers;
	numbers.reserve(N);

	std::random_device rd;

    //
    // Engines 
    //
    std::mt19937 engine(rd());
    //std::knuth_b e2(rd());
    //std::default_random_engine e2(rd()) ;

    //
    // Distribtuions
    //
    std::uniform_int<int> dist(min, max);
    //std::normal_distribution<> dist(2, 2);
    //std::student_t_distribution<> dist(5);
    //std::poisson_distribution<> dist(2);
    //std::extreme_value_distribution<> dist(0,2);


	for (int i = 0; i < N; i++) {

		numbers.push_back(dist(engine));
	}

	return numbers;

}