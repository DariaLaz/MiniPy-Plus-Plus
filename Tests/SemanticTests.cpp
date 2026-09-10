#include <stdexcept>
#include <string>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Semantic/SemanticAnalyzer.h"
#include "Errors/SyntaxError.h"

namespace
{

    void expect_semantic_success(const std::string &source, const std::string &test_name)
    {
        try
        {
            Lexer lexer(source);
            auto tokens = lexer.tokenize();

            Parser parser(tokens);
            auto program = parser.parse();

            SemanticAnalyzer analyzer;
            analyzer.analyze(*program);
        }
        catch (const std::exception &error)
        {
            throw std::runtime_error(
                "Expected semantic analysis to succeed in '" +
                test_name +
                "', but got: " +
                error.what());
        }
    }

    void expect_semantic_error(const std::string &source, const std::string &test_name)
    {
        try
        {
            Lexer lexer(source);
            auto tokens = lexer.tokenize();

            Parser parser(tokens);
            auto program = parser.parse();

            SemanticAnalyzer analyzer;
            analyzer.analyze(*program);
        }
        catch (const SyntaxError &)
        {
            return;
        }
        catch (const std::exception &error)
        {
            throw std::runtime_error(
                "Expected SyntaxError in '" +
                test_name +
                "', but got another exception: " +
                error.what());
        }

        throw std::runtime_error(
            "Expected SyntaxError in '" +
            test_name +
            "', but semantic analysis succeeded");
    }

    void test_break_outside_loop()
    {
        expect_semantic_error("break\n", "break outside loop");
    }

    void test_break_inside_while()
    {
        expect_semantic_success(
            "while True:\n"
            "    break\n",
            "break inside while");
    }

    void test_break_inside_for()
    {
        expect_semantic_success(
            "for i in range(10):\n"
            "    break\n",
            "break inside for");
    }

    void test_break_inside_nested_loop()
    {
        expect_semantic_success(
            "while True:\n"
            "    for i in range(10):\n"
            "        break\n"
            "    break\n",
            "break inside nested loop");
    }

    void test_continue_outside_loop()
    {
        expect_semantic_error("continue\n", "continue outside loop");
    }

    void test_continue_inside_while()
    {
        expect_semantic_success(
            "while True:\n"
            "    continue\n",
            "continue inside while");
    }

    void test_continue_inside_for()
    {
        expect_semantic_success(
            "for i in range(10):\n"
            "    continue\n",
            "continue inside for");
    }

    void test_return_outside_function()
    {
        expect_semantic_error("return 10\n", "return outside function");
    }

    void test_return_inside_function()
    {
        expect_semantic_success(
            "def f():\n"
            "    return 10\n",
            "return inside function");
    }

    void test_return_without_value()
    {
        expect_semantic_success(
            "def f():\n"
            "    return\n",
            "return without value");
    }

    void test_return_inside_if_inside_function()
    {
        expect_semantic_success(
            "def f(x):\n"
            "    if x > 0:\n"
            "        return x\n"
            "    return 0\n",
            "return inside if inside function");
    }

    void test_duplicate_parameters()
    {
        expect_semantic_error(
            "def f(x, x):\n"
            "    return x\n",
            "duplicate parameters");
    }

    void test_multiple_unique_parameters()
    {
        expect_semantic_success(
            "def f(a, b, c):\n"
            "    return a\n",
            "unique parameters");
    }

    void test_duplicate_parameter_not_confused_with_function_name()
    {
        expect_semantic_success(
            "def x(x):\n"
            "    return x\n",
            "parameter may match function name");
    }

    void test_break_in_function_inside_loop_is_invalid()
    {
        expect_semantic_error(
            "while True:\n"
            "    def f():\n"
            "        break\n"
            "    break\n",
            "break in function inside outer loop");
    }

    void test_continue_in_function_inside_loop_is_invalid()
    {
        expect_semantic_error(
            "while True:\n"
            "    def f():\n"
            "        continue\n"
            "    break\n",
            "continue in function inside outer loop");
    }

    void test_break_in_loop_inside_function_is_valid()
    {
        expect_semantic_success(
            "def f():\n"
            "    while True:\n"
            "        break\n"
            "    return 10\n",
            "break in loop inside function");
    }

    void test_continue_in_loop_inside_function_is_valid()
    {
        expect_semantic_success(
            "def f():\n"
            "    for i in range(10):\n"
            "        continue\n"
            "    return 10\n",
            "continue in loop inside function");
    }

    void test_return_inside_nested_function()
    {
        expect_semantic_success(
            "def outer():\n"
            "    def inner():\n"
            "        return 10\n"
            "    return inner\n",
            "return inside nested function");
    }

    void test_duplicate_parameters_in_nested_function()
    {
        expect_semantic_error(
            "def outer():\n"
            "    def inner(x, x):\n"
            "        return x\n"
            "    return inner\n",
            "duplicate parameters in nested function");
    }

    void test_valid_complex_control_flow()
    {
        expect_semantic_success(
            "def search():\n"
            "    for i in range(10):\n"
            "        if i == 2:\n"
            "            continue\n"
            "        if i == 5:\n"
            "            break\n"
            "    return i\n",
            "valid complex control flow");
    }

}

void run_semantic_tests()
{
    test_break_outside_loop();
    test_break_inside_while();
    test_break_inside_for();
    test_break_inside_nested_loop();

    test_continue_outside_loop();
    test_continue_inside_while();
    test_continue_inside_for();

    test_return_outside_function();
    test_return_inside_function();
    test_return_without_value();
    test_return_inside_if_inside_function();

    test_duplicate_parameters();
    test_multiple_unique_parameters();
    test_duplicate_parameter_not_confused_with_function_name();

    test_break_in_function_inside_loop_is_invalid();
    test_continue_in_function_inside_loop_is_invalid();

    test_break_in_loop_inside_function_is_valid();
    test_continue_in_loop_inside_function_is_valid();

    test_return_inside_nested_function();
    test_duplicate_parameters_in_nested_function();

    test_valid_complex_control_flow();
}