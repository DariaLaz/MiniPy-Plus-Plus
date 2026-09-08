#include "Builtins/PrintBuiltin.h"

#include <iostream>

std::string PrintBuiltin::name() const
{
    return "print";
}

Value PrintBuiltin::func(const std::vector<Value> &args) const
{
    for (std::size_t i = 0; i < args.size(); ++i)
    {
        std::cout << value_to_string(args[i]);

        if (i + 1 < args.size())
        {
            std::cout << " ";
        }
    }

    std::cout << '\n';

    return std::monostate{};
}