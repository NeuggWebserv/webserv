#include "test.hpp"
#include "ConfigReader.hpp"

int		main(void)
{
	ConfigReader		reader;
	fileVector			file;

	file = reader.readFile("test.conf");
	for (unsigned int i = 0; i < file.size(); i++)
	{
		std::cout << file[i] << "\n";
	}
	std::cout << std::endl;
	return 0;
}