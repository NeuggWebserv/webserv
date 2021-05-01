#include "webserv.hpp"

std::string&					pop_str_tail(std::string& str)
{
	if (str.size())
		str.resize(str.size() - 1);
	return str;
}
