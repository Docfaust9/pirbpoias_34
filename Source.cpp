#include<iostream>
#include<fstream>
#include<string>
#include <sstream>
#include <openssl/sha.h>
#include <iomanip>
#include<thread>
#include<mutex>
#include<ctime>
#include<vector>

#pragma warning(disable : 4996)

using namespace std;


string sha256(const string& input) 
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, input.c_str(), input.size());
	SHA256_Final(hash, &sha256);

	stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
	}

	return ss.str();
}

string first;
string second;
string third;
const string alphabet = "abcdefghijklmnopqrstuvwxyz";

void unhash(string hash, unsigned short int start_char, unsigned short int end_char, string* result)
{
	
	char data[5];
	for (int i = start_char; i < 26; i++)
	{
		data[0] = alphabet[i];
		for (int j = 0; j < 26; j++)
		{
			data[1] = alphabet[j];
			for (int k = 0; k < 26; k++)
			{
				data[2] = alphabet[k];
				for (int l = 0; l < 26; l++)
				{
					data[3] = alphabet[l];
					for (int r = 0; r < 26; r++)
					{
						data[4] = alphabet[r];
						string str = string(data);
						cout << str << endl;
						if (sha256(str) == hash)
						{
							*result = str;
							return;
						}
						
					}
				}
			}
		}
	}
	
}


string create_threads(string hash, unsigned short int number_of_threads)
{
	unsigned short int delta = 26 / number_of_threads;
	vector <thread> threads;
	string result;
	unsigned int start_time = clock();

	if (26 % number_of_threads == 0)
	{
		
		for (unsigned short int i = 0; i < number_of_threads; i++)
		{
			threads.push_back(thread(unhash, hash, i*delta, (i+1)*delta, &result));
		}

		for (unsigned short int i = 0; i < number_of_threads; i++)
		{
			threads[i].join();
		}
	}
	else
	{
		for (unsigned short int i = 0; i < number_of_threads-1; i++)
		{
			threads.push_back(thread(unhash, hash, i * delta, (i + 1) * delta, &result));
		}
		threads.push_back(thread(unhash, hash, number_of_threads * delta, 26, &result));

		for (unsigned short int i = 0; i < number_of_threads; i++)
		{
			threads[i].join();
		}
	}
	unsigned int end_time = clock();
	cout << "Алгоритм отработал за " << (end_time - start_time) / CLOCKS_PER_SEC << " секунд" << endl;
	return result;

}


void get_lines()
{
	ifstream in;
	in.open("source.txt");
	if (in.is_open())
	{
		in.seekg(0);
		getline(in, first);
		getline(in, second);
		getline(in, third);
	}
	in.close();
}

int main()

{
	setlocale(LC_ALL, "ru");
	unsigned short int number_threads;
	get_lines();
	cout << "Выберите, значение какой хэш суммы необходимо подобрать?" << endl << "1. " << first << endl << "2. " << second << endl<<\
		"3. " << third << endl;
	string choose;

	cin >> choose;
	while (choose != "1" && choose != "2" && choose != "3")
	{
		cout << "Значение неккоректно, повторите ввод: ";
		cin >> choose;
	}

	cout << "Введите количество потоков  для подбора значения хэш функции (до 6): ";
	cin >> number_threads;

	

	string result;
	if (choose == "1")
	{
		result = create_threads(first, number_threads);
	}
	else {
		if (choose == "2")
		{
			result = create_threads(second, number_threads);
		}
		else {
			if (choose == "3")
			{
				result = create_threads(third, number_threads);
			}
		}
	}

	cout << "Значение для хэш суммы: " << result << endl;


	return 0;
}