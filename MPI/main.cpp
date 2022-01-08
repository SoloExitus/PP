#include <mpi.h> 
#include <stdio.h> 


int main(int argc, char** argv) {

	MPI_Init(&argc, &argv);
	// Получаем рейтинг процесса 
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// Распечатать сообщение 
	printf("Rank: %d , Size: %d \n", rank, size);
	// Завершаем среду MPI. 
	MPI_Finalize();
	return 0;
}
	