#include <stdexcept>
#include <string>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Errors/SyntaxError.h"

namespace
{

    void expect_parses(
        const std::string &source,
        const std::string &test_name)
    {
        try
        {
            Lexer lexer(source);
            auto tokens = lexer.tokenize();

            Parser parser(tokens);
            auto program = parser.parse();

            if (!program)
            {
                throw std::runtime_error("Parser returned null program: " + test_name);
            }
        }
        catch (const std::exception &error)
        {
            throw std::runtime_error(
                "Expected valid syntax in test '" +
                test_name +
                "', but got: " +
                error.what());
        }
    }

    void expect_syntax_error(const std::string &source, const std::string &test_name)
    {
        try
        {
            Lexer lexer(source);
            auto tokens = lexer.tokenize();

            Parser parser(tokens);
            parser.parse();
        }
        catch (const SyntaxError &)
        {
            return;
        }
        catch (const std::exception &error)
        {
            throw std::runtime_error(
                "Expected SyntaxError in test '" +
                test_name +
                "', but got another exception: " +
                error.what());
        }

        throw std::runtime_error(
            "Expected SyntaxError in test '" +
            test_name +
            "', but parsing succeeded");
    }

    void test_number_expression()
    {
        expect_parses("10\n", "number expression");
    }

    void test_arithmetic_expression()
    {
        expect_parses("2 + 3 * 4\n", "arithmetic expression");
    }

    void test_parenthesized_expression()
    {
        expect_parses("(2 + 3) * 4\n", "parenthesized expression");
    }

    void test_comparison_expression()
    {
        expect_parses("x >= 10\n", "comparison expression");
    }

    void test_logical_expression()
    {
        expect_parses("True and False or not False\n", "logical expression");
    }

    void test_assignment()
    {
        expect_parses("x = 10\n", "assignment");
    }

    void test_index_assignment()
    {
        expect_parses("x[0] = 10\n", "index assignment");
    }

    void test_list_literal()
    {
        expect_parses("x = [1, 2, 3]\n", "list literal");
    }

    void test_dictionary_literal()
    {
        expect_parses("x = {\"name\": \"Daria\", \"age\": 20}\n", "dictionary literal");
    }

    void test_index_expression()
    {
        expect_parses("x[0]\n", "index expression");
    }

    void test_slice_expression()
    {
        expect_parses(
            "x[1:3]\n"
            "x[:3]\n"
            "x[2:]\n"
            "x[:]\n",
            "slice expressions");
    }

    void test_function_call()
    {
        expect_parses("add(1, 2)\n", "function call");
    }

    void test_nested_function_call()
    {
        expect_parses("print(add(1, multiply(2, 3)))\n", "nested function call");
    }

    void test_if_statement()
    {
        expect_parses(
            "if x > 0:\n"
            "    print(x)\n",
            "if statement");
    }

    void test_if_else_statement()
    {
        expect_parses(
            "if x > 0:\n"
            "    print(\"positive\")\n"
            "else:\n"
            "    print(\"other\")\n",
            "if else statement");
    }

    void test_if_elif_else_statement()
    {
        expect_parses(
            "if x > 0:\n"
            "    print(\"positive\")\n"
            "elif x == 0:\n"
            "    print(\"zero\")\n"
            "else:\n"
            "    print(\"negative\")\n",
            "if elif else statement");
    }

    void test_while_statement()
    {
        expect_parses(
            "while x < 10:\n"
            "    x = x + 1\n",
            "while statement");
    }

    void test_for_statement()
    {
        expect_parses(
            "for i in range(5):\n"
            "    print(i)\n",
            "for statement");
    }

    void test_nested_blocks()
    {
        expect_parses(
            "while x < 10:\n"
            "    if x == 5:\n"
            "        break\n"
            "    x = x + 1\n",
            "nested blocks");
    }

    void test_function_definition()
    {
        expect_parses(
            "def add(a, b):\n"
            "    return a + b\n",
            "function definition");
    }

    void test_function_without_parameters()
    {
        expect_parses(
            "def hello():\n"
            "    print(\"hello\")\n",
            "function without parameters");
    }

    void test_nested_function()
    {
        expect_parses(
            "def outer():\n"
            "    def inner():\n"
            "        return 10\n"
            "    return inner\n",
            "nested function");
    }

    void test_multiline_list()
    {
        expect_parses(
            "x = [\n"
            "    1,\n"
            "    2,\n"
            "    3\n"
            "]\n",
            "multiline list");
    }

    void test_multiline_call()
    {
        expect_parses(
            "print(\n"
            "    1 + 2\n"
            ")\n",
            "multiline function call");
    }

    void test_multiline_dictionary()
    {
        expect_parses(
            "x = {\n"
            "    \"a\": 1,\n"
            "    \"b\": 2\n"
            "}\n",
            "multiline dictionary");
    }

    void test_missing_if_colon()
    {
        expect_syntax_error(
            "if True\n"
            "    print(1)\n",
            "missing if colon");
    }

    void test_missing_while_colon()
    {
        expect_syntax_error(
            "while True\n"
            "    print(1)\n",
            "missing while colon");
    }

    void test_missing_function_colon()
    {
        expect_syntax_error(
            "def f()\n"
            "    return 1\n",
            "missing function colon");
    }

    void test_missing_closing_parenthesis()
    {
        expect_syntax_error("print(1 + 2\n", "missing closing parenthesis");
    }

    void test_missing_closing_bracket()
    {
        expect_syntax_error("x = [1, 2, 3\n", "missing closing bracket");
    }

    void test_missing_expression_after_operator()
    {
        expect_syntax_error("x = 10 +\n", "missing expression after operator");
    }

    void test_assignment_without_value()
    {
        expect_syntax_error("x =\n", "assignment without value");
    }

    void test_invalid_function_parameters()
    {
        expect_syntax_error(
            "def f(a,):\n"
            "    return a\n",
            "invalid function parameters");
    }

}

void run_parser_tests()
{
    test_number_expression();
    test_arithmetic_expression();
    test_parenthesized_expression();
    test_comparison_expression();
    test_logical_expression();

    test_assignment();
    test_index_assignment();

    test_list_literal();
    test_dictionary_literal();
    test_index_expression();
    test_slice_expression();

    test_function_call();
    test_nested_function_call();

    test_if_statement();
    test_if_else_statement();
    test_if_elif_else_statement();

    test_while_statement();
    test_for_statement();
    test_nested_blocks();

    test_function_definition();
    test_function_without_parameters();
    test_nested_function();

    test_multiline_list();
    test_multiline_call();
    test_multiline_dictionary();

    test_missing_if_colon();
    test_missing_while_colon();
    test_missing_function_colon();
    test_missing_closing_parenthesis();
    test_missing_closing_bracket();
    test_missing_expression_after_operator();
    test_assignment_without_value();
    test_invalid_function_parameters();
}