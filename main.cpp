#include <iostream>
#include <string>

#include "Lexer.h"
#include "Parser.h"

int main()
{
    std::string line;

    while (true)
    {
        std::cout << ">>> ";

        if (line.empty())
        {
            continue;
        }
        if (line == "exit")
        {
            break;
        }

        try
        {
            Lexer lexer(line);
            std::vector<Token> tokens = lexer.tokenize();

            Parser parser(tokens);
            std::unique_ptr<Expression> expression = parser.parse();

            std::cout << expression->evaluate() << '\n';
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}