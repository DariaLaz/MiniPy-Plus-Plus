#include <iostream>
#include <string>

#include "Lexer.h"
#include "Parser.h"

void print_value(const Value &value)
{
    if (std::holds_alternative<int>(value))
    {
        std::cout << std::get<int>(value);
    }
    else if (std::holds_alternative<bool>(value))
    {
        std::cout
            << (std::get<bool>(value) ? "True" : "False");
    }
}

int main()
{
    std::string line;
    Environment env;

    while (true)
    {
        std::cout << ">>> ";

        if (!std::getline(std::cin, line) || line == "exit")
        {
            break;
        }

        if (line.empty())
        {
            continue;
        }

        try
        {
            Lexer lexer(line);
            std::vector<Token> tokens = lexer.tokenize();

            Parser parser(tokens);
            auto statement = parser.parse();

            auto result = statement->execute(env);

            if (result.has_value())
            {
                print_value(result.value());
                std::cout << '\n';
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}