#include <fstream>
#include <iostream>
#include <sstream>
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

std::string read_file(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Wrong arguments, please provide a source file\n";
        return 1;
    }

    try
    {
        std::string source = read_file(argv[1]);

        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto program = parser.parse();

        Environment env;

        auto result = program->execute(env);

        if (result.has_value())
        {
            print_value(*result);
            std::cout << '\n';
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}