#include "test.hpp"

int			main(void)
{
	try {
		Request		req(REQUEST);
		std::cout << req;
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
	}

	return 0;
}