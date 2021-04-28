#include "ConfigUtil.hpp"

// HELPFUL FUNCTIONS
bool is_digits(const std::string &str)
{
	return str.find_first_not_of("0123456789") == std::string::npos;
}

unsigned int	str_to_ip(std::string str_ip)
{
	size_t			sep = 0;
	unsigned int	n;
	unsigned char	uchar_ip[4];
	size_t			start = 0;
	if (str_ip == "localhost")
		str_ip = "127.0.0.1";
	for (unsigned int i = 3 ; i != std::numeric_limits<uint32_t>::max(); i--)
	{
		sep = str_ip.find_first_of('.', sep);
		std::string str = str_ip.substr(start, sep);
		n = std::atoi(str.c_str());
		uchar_ip[i] = static_cast<unsigned char>(n);
		sep++;
		start = sep;
	}
	unsigned final = *(reinterpret_cast<unsigned int *>(uchar_ip));
	return final;
}

std::string	remove_adjacent_slashes(const std::string &str)
{
	std::string	ret;
	bool		last_is_slash = false;

	for (std::string::size_type i = 0; i < str.length(); i++)
	{
		if (str[i] == '/')
		{
			if (!last_is_slash)
				ret.push_back(str[i]);
			last_is_slash = true;
		}
		else
		{
			last_is_slash = false;
			ret.push_back(str[i]);
		}
	}
	return ret;
}
