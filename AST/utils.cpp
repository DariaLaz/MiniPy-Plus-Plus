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
