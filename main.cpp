#include <iostream>
#include <string>

#include "Lexer.h"
#include "Parser.h"

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
                std::cout << result.value() << '\n';
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}