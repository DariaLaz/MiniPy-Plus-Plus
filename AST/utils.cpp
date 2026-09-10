#include "AST/utils.h"

int normalize_int_index(Value index_value, std::size_t size, const SourceLocation &location)
{
    int index = std::get<int>(index_value);

    if (index < 0)
    {
        index += static_cast<int>(size);
    }

    if (index < 0 || index >= static_cast<int>(size))
    {
        throw IndexError("Index out of range", location);
    }

    return index;
}