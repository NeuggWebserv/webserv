#include "webserv.hpp"

bool is_suffix(const std::string& src, const std::string& suffix)
{
    size_t src_len = src.size();
    size_t suffix_len = suffix.size();

    if (src_len > suffix_len)
        return false;
    while (--suffix_len > 0)
    {
        --src_len;
        if (src[src_len] != suffix[suffix_len])
            return false;
    }
    return true;
}