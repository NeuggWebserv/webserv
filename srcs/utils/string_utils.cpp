#include "webserv.hpp"

bool is_suffix(const std::string& src, const std::string& suffix)
{
    size_t src_len = src.size();
    size_t suffix_len = suffix.size();

    while (suffix_len > 0)
    {
		--suffix_len;
        --src_len;
        if (src_len < 0 || src[src_len] != suffix[suffix_len])
            return false;
    }
    return true;
}
