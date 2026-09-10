#include <iostream>
#include <exception>

void run_lexer_tests();
void run_parser_tests();
void run_expression_tests();
void run_statement_tests();
void run_semantic_tests();
void run_builtin_tests();
void run_error_tests();

int main()
{
    try
    {
        std::cerr << "Lexer tests...\n";
        run_lexer_tests();

        std::cerr << "Parser tests...\n";
        run_parser_tests();

        std::cerr << "Expression tests...\n";
        run_expression_tests();

        std::cerr << "Statement tests...\n";
        run_statement_tests();

        std::cerr << "Semantic tests...\n";
        run_semantic_tests();

        std::cerr << "Builtin tests...\n";
        run_builtin_tests();

        std::cerr << "Error tests...\n";
        run_error_tests();

        std::cerr << "All tests passed!\n";
        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "TEST FAILED: " << error.what() << '\n';

        return 1;
    }
}