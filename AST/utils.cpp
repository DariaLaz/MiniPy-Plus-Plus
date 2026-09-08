#include "AST/utils.h"

int normalize_int_index(Value index, int size)
{
    int i = std::get<int>(index);
    if (i < 0)
    {
        i += size;
    }

    return i;
}

inline void validate_index(std::size_t min, std::size_t max, int val, std::string message = "Index out of range")
{
    if (val < min || val >= max)
    {
        throw std::runtime_error(message);
    }
}
