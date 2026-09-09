#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "AST/Environment.h"
#include "Builtins/utils.h"
#include "Semantic/SemanticAnalyzer.h"

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
        std::cerr << "Usage: " << argv[0] << " <source-file>\n";

        return 1;
    }

    try
    {
        std::string source = read_file(argv[1]);

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        std::unique_ptr<Statement> program = parser.parse();

        SemanticAnalyzer analyzer;
        analyzer.analyze(*program);

        auto env = std::make_shared<Environment>();
        install_builtins(*env);
        program->execute(*env);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}