#define _CRT_SECURE_NO_WARNINGS

#define HAVE_STRUCT_TIMESPEC
#include <string.h>
#include <pthread.h>
#include <ctime>
#include <openssl/md5.h>
#include <iostream>

const char* alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
const int Nth = 4;
const char* HASHPASSWORD;
int PASSWORDLEN = 5;

struct Data
{
	int alohabetStart;
	int Count;
};

void* thFunc(void* arg)
{
	Data* data = static_cast<Data*>(arg);

	unsigned char password[6];
	password[5] = '\0';

	unsigned char md5digest[MD5_DIGEST_LENGTH];
	char md5chars[2 * MD5_DIGEST_LENGTH + 1];

	for (int s0 = data->alohabetStart; s0 < data->alohabetStart + data->Count; s0++)
	{
		password[0] = alphabet[s0];
		for (int s1 = 0; s1 < 36; s1++)
		{
			password[1] = alphabet[s1];
			for (int s2 = 0; s2 < 36; s2++)
			{
				password[2] = alphabet[s2];
				for (int s3 = 0; s3 < 36; s3++)
				{
					password[3] = alphabet[s3];
					for (int s4 = 0; s4 < 36; s4++)
					{
						password[4] = alphabet[s4];
						MD5(password,5, md5digest);
						for (int a = 0; a < MD5_DIGEST_LENGTH; a++)
						{
							
							sprintf_s(&md5chars[a * 2], 3, "%02x", md5digest[a]);
						}
						
						if (strcmp(HASHPASSWORD, md5chars) == 0)
						{
							printf("PASSWORD = %s\n", password);
							return nullptr;
						}
					}
				}
			}
		}
	}

	return nullptr;
}

int main(int* argc, char** argv)
{
	HASHPASSWORD = argv[1];

	std::cout << "Entered HASH:" << HASHPASSWORD << std::endl;

	pthread_t Th[Nth];
	Data args[Nth];

	unsigned int start_time = clock();

	for (int distribWork = 0, curWork = 0, executorNum = 0; distribWork < 36; executorNum++)
	{
		curWork = (36 - distribWork) / (Nth - executorNum);

		args[executorNum].alohabetStart = distribWork;
		args[executorNum].Count = curWork;

		pthread_create(&Th[executorNum], nullptr, thFunc, (void*)&args[executorNum]);
		distribWork += curWork;
	}

	for (int i = 0; i < Nth; i++)
	{
		pthread_join(Th[i], nullptr);
	}
	unsigned int end_time = clock();

	std::cout << end_time - start_time << std::endl;
	return 0;
}