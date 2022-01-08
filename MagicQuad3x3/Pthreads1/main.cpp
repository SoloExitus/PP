
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <iostream>
#include <ctime>

pthread_mutex_t output;

const int N = 3;

const int Nth = 1;

const int SUM = N * (N*N + 1) / 2;

struct Data
{
	int start;
	int count;
	
	Data() :start(0), count(0){}
	Data(int s,int c):start(s),count(c){}
};

bool incorrectValue(int x)
{
	return x <= 0 || x > N* N;
}

void* thFunc(void* arg)
{
	Data* data = static_cast<Data*>(arg);

	for (int x00 = data->start; x00 < data->start + data->count; x00++)
	{
		for (int x01 = 1; x01 <= 9; x01++) 
		{
			if (x01 == x00) continue;
			int x02 = SUM - x00 - x01;
			if (x02 == x00 || x02 == x01 || incorrectValue(x02)) continue;
			for (int x10 = 1; x10 <= 9; x10++)
			{
				if (x10 == x00 || x10 == x01 || x10 == x02) continue;
				for (int x11 = 1; x11 <= 9; x11++)
				{
					if (x11 == x00 || x11 == x01 || x11 == x02 || x11 ==x10) continue;
					int x12 = SUM - x10 - x11;
					if (x12 == x00 || x12 == x01 || x12 == x02 || x12 == x10 || x12 == x11) continue;
					if (incorrectValue(x12)) continue;
					int x20 = SUM - x00 - x10;
					if (x20 == x00 || x20 == x01 || x20 == x02 || x20 == x10 || x20 == x11 || x20 == x12) continue;
					if (incorrectValue(x20)) continue;
					int x21 = SUM - x01 - x11;
					if (x21 == x00 || x21 == x01 || x21 == x02 || x21 == x10 || x21 == x11 || x21 == x12 || x21 == x20) continue;
					if (incorrectValue(x21)) continue;
					int x22 = SUM - x00 - x11;
					if (x22 == x00 || x22 == x01 || x22 == x02 || x22 == x10 || x22 == x11 || x22 == x12 || x22 == x20 || x22 == x21) continue;
					if (incorrectValue(x22)) continue;

					int r0 = x00 + x01 + x02;
					int r1 = x10 + x11 + x12;
					int r2 = x20 + x21 + x22;

					int c0 = x00 + x10 + x20;
					int c1 = x01 + x11 + x21;
					int c2 = x02 + x12 + x22;

					int d1 = x00 + x11 + x22;
					int d2 = x20 + x11 + x02;

					bool magic = r0 == r1 && r1 == r2 && r2 == c0 && c0 == c1 && c1 == c2 && c2 == d1 && d1 == d2;

					if (magic)
					{
						pthread_mutex_lock(&output);
						std::cout << x00 << x01 << x02 << std::endl;
						std::cout << x10 << x11 << x12 << std::endl;
						std::cout << x20 << x21 << x22 << std::endl;
						std::cout << "--------------------------------------" << std::endl;
						pthread_mutex_unlock(&output);
					}
				}
			}
		}
	}

	return nullptr;
}



int main()
{
	pthread_mutex_init(&output, nullptr);
	
	pthread_t Th[Nth];
	Data args[Nth];

	int AllWork = N * N;
	int distribWork = 0;
	
	unsigned int start_time = clock();
	for (int i = 0; i < Nth; i++)
	{
		int curWork = (AllWork - distribWork) / (Nth - i);


		args[i].start = distribWork + 1;
		args[i].count = curWork;

		pthread_create(&Th[i], nullptr, thFunc, (void*)&args[i]);
		distribWork += curWork;
	}

	for (int i = 0; i < Nth; i++)
	{
		pthread_join(Th[i], nullptr);
	}
	unsigned int end_time = clock();

	std::cout << end_time - start_time << std::endl;

	pthread_mutex_destroy(&output);
	system("pause");
	return 0;
}