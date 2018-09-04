#include <iostream>

#include "data_reader.h"

int main(int argc, char const *argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cout << "Usage:\nbulk <num>\nParameters: \n<num>: number of packed commands\n";
			return 1;
		}

		int num_commands = atoi(argv[1]);

		data_reader reader(num_commands);
		reader.Perform(std::cin);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
