#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <ctime>
#include <random>
#include <string>

#include <windows.h>

pthread_mutex_t LOG;
pthread_mutex_t FORK[5];

void WriteLog(const int num, const std::string& message) {

	pthread_mutex_lock(&LOG);
	std::cout << "Philosopher[" << num << "]:" << message << std::endl;
	pthread_mutex_unlock(&LOG);
}

void* thFunc(void* arg)
{
	int num = (int)arg;
	srand(num);
	int Count = 5;

	time_t unixtime;
	timespec ts;

	int res;

	while (Count > 0)
	{
		if (num % 2 == 0)
		{
			WriteLog(num, "I am waiting right fork!");
			std::time(&unixtime);
			ts.tv_sec = unixtime + 20;
			res = pthread_mutex_timedlock(&FORK[num],&ts);
			if (res == 0)
			{
				WriteLog(num, "I take right fork!");

				WriteLog(num, "I am waiting left fork!");
				std::time(&unixtime);
				ts.tv_sec = unixtime + 20;
				res = pthread_mutex_timedlock(&FORK[(num + 1) % 5], &ts);
				if (res == 0)
				{
					WriteLog(num, "I take left fork!");

					WriteLog(num, "I am eating!");
					int time = rand() % 10 + 5;
					Sleep(time * 1000);

					WriteLog(num, "I put left fork!");
					pthread_mutex_unlock(&FORK[(num + 1) % 5]);

					WriteLog(num, "I put right fork!");
					pthread_mutex_unlock(&FORK[num]);
				}
				else
				{
					WriteLog(num, "I can't take left forck!");
					WriteLog(num, "I put right forck!");
					pthread_mutex_unlock(&FORK[num]);
					continue;
				}
			}
			else
			{
				WriteLog(num, "I can't take right forck!");
				continue;
			}


		}
		else
		{
			WriteLog(num, "I am waiting left fork!");
			std::time(&unixtime);
			ts.tv_sec = unixtime + 20;
			res = pthread_mutex_timedlock(&FORK[(num + 1) % 5], &ts);
			if (res == 0)
			{
				WriteLog(num, "I take left fork!");

				WriteLog(num, "I am wating right fork!");
				std::time(&unixtime);
				ts.tv_sec = unixtime + 20;
				res = pthread_mutex_timedlock(&FORK[num], &ts);
				if (res == 0)
				{
					WriteLog(num, "I take right fork!");

					WriteLog(num, "I am eating!");
					int time = rand() % 10 + 5;
					Sleep(time * 1000);

					WriteLog(num, "I put right fork!");
					pthread_mutex_unlock(&FORK[num]);

					WriteLog(num, "I put left fork!");
					pthread_mutex_unlock(&FORK[(num + 1) % 5]);
				}
				else
				{
					WriteLog(num, "I can't take right forck!");
					WriteLog(num, "I put left forck!");
					pthread_mutex_unlock(&FORK[(num + 1)% 5]);
				}
				
			}
			else
			{
				WriteLog(num, "I can't take left forck!");
				continue;
			}
			
		}

		WriteLog(num, "I am Philosopher!");
		int time = rand() % 30 + 15;
		Sleep(time * 1000);
		Count--;
	}
	
	WriteLog(num, "I go home!");
	return nullptr;
}

int main()
{	
	pthread_mutex_init(&LOG, nullptr);

	pthread_t Th[5];

	for (int i = 0; i < 5; i++)
	{
		pthread_mutex_init(&FORK[i], nullptr);
	}

	unsigned int start_time = clock();

	for (int i = 0; i < 5; i++)
	{
		pthread_create(&Th[i], nullptr, thFunc, (void*)i);
	}

	for (int i = 0; i < 5; i++)
	{
		pthread_join(Th[i], nullptr);
	}
	unsigned int end_time = clock();

	std::cout << std::endl << "------------------------------" << std::endl;
	std::cout << end_time - start_time << std::endl;

	pthread_mutex_destroy(&LOG);

	for (int i = 0; i < 5; i++)
	{
		pthread_mutex_destroy(&FORK[i]);
	}

	system("pause");
	return 0;
}