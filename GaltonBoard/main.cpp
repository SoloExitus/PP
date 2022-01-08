#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <ctime>
#include <random>

pthread_mutex_t COLLECTIONACCESS;

const int L = 10;
const int BALLS = 10000;
const int Nth = 4;

int collection[L + 1] = {};

struct Data 
{
	int levelsCount;
	int ballsCount;

	Data(int levelsCount, int ballsCount): levelsCount(levelsCount), ballsCount(ballsCount){}
	Data() : levelsCount(0), ballsCount(0) {}

};

void* thFunc(void* arg)
{
	srand(static_cast<unsigned int>(time(0)));

	Data* data = static_cast<Data*>(arg);

	for (int ball = 0; ball < data->ballsCount; ball++)
	{
		float position = (float)(data->levelsCount) / 2;
		for (int pin = 0; pin < data->levelsCount; pin++)
		{
			bool luck = rand() % 2;
			if (luck)
				position += 0.5;
			else
				position -= 0.5;
		}

		pthread_mutex_lock(&COLLECTIONACCESS);
		collection[(int)position] += 1;
		pthread_mutex_unlock(&COLLECTIONACCESS);
	}
	return nullptr;
}

void gistogramma()
{
	int max = 0;

	for (int i = 0; i < L + 1; i++)
	{
		if (collection[i] > max)
			max = collection[i];
	}

	for (int i = max; i > 0; i-=100)
	{
		for (int j = 0; j < L + 1; j++)
		{
			if (collection[j] >= i)
				std::cout << "*** ";
			else
				std::cout << "    ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	pthread_mutex_init(&COLLECTIONACCESS, nullptr);

	pthread_t Th[Nth];
	Data args[Nth];

	unsigned int start_time = clock();

	for (int distribWork = 0, curWork=0, executorNum = 0; distribWork < BALLS; executorNum++)
	{
		curWork = (BALLS - distribWork) / (Nth - executorNum);

		args[executorNum].ballsCount = curWork;
		args[executorNum].levelsCount = L;

		pthread_create(&Th[executorNum], nullptr, thFunc, (void*)&args[executorNum]);
		distribWork += curWork;
	}

	for (int i = 0; i < Nth; i++)
	{
		pthread_join(Th[i], nullptr);
	}
	unsigned int end_time = clock();


	gistogramma();

	for (int i = 0; i < L + 1; i++)
	{
		std::cout << collection[i] << " ";
	}
	std::cout << std::endl << "------------------------------" << std::endl;
	std::cout << end_time - start_time << std::endl;

	pthread_mutex_destroy(&COLLECTIONACCESS);
	system("pause");
	return 0;
}