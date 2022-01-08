#include <iostream>
#include <bitset>
#include <omp.h>

#define itemCount 10

const int MaxCopacity = 165;

const int weight[itemCount] = { 23, 31, 29, 44, 53, 38, 63, 85, 89, 82 };

const int profit[itemCount] = { 92, 57, 49, 68, 60, 43, 67, 84, 87, 72 };

#define Th 4

struct Res {
	int profit = -1;
	int weight = 0;
	int selection = -1;
};

int main(){

	omp_set_num_threads(Th);

	Res res[Th];

	long int N = 1 << itemCount;

	#pragma omp parallel for
		for (int i = 0; i < N; i++)
		{
			int thIndex = omp_get_thread_num();

			int currentProfit = 0;
			int currentWeight = 0;

			for (int shift = 0; shift < itemCount; shift++)
			{
				int mask = 1 << shift;
				bool take = i & mask;

				if (take) {
					currentProfit += profit[shift];
					currentWeight += weight[shift];
				}
			}

			if (currentWeight <= MaxCopacity && currentProfit > res[thIndex].profit)
			{
				
				res[thIndex].profit = currentProfit;
				res[thIndex].weight = currentWeight;
				res[thIndex].selection = i;
			}
		}

	int optimal = 0;
	for (int r = 1; r < Th; r++)
	{
		if (res[r].profit > res[optimal].profit)
			optimal = r;
	}

	std::bitset<itemCount> selbin(res[optimal].selection);
	std::cout <<"optimal selection= " << res[optimal].selection <<"  bin= "<< selbin << " with profit= "<< res[optimal].profit<< " and weight= "<< res[optimal].weight << std::endl;
	system("pause");
	return 0;
}