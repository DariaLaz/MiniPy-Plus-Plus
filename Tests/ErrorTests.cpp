#include <string>

#include "TestUtils.h"

#include "Errors/NameError.h"
#include "Errors/TypeError.h"
#include "Errors/IndexError.h"
#include "Errors/KeyError.h"
#include "Errors/ValueError.h"
#include "Errors/ZeroDivisionError.h"
#include "Errors/SyntaxError.h"

namespace
{
    void test_name_error_type()
    {
        expect_throws<NameError>("print(missing)\n", "undefined variable error type");
    }

    void test_name_error_location()
    {
        expect_error_contains<NameError>(
            "print(missing)\n",
            "NameError at 1:7",
            "undefined variable location");
    }

    void test_name_error_second_line()
    {
        expect_error_contains<NameError>(
            "x = 10\n"
            "print(y)\n",
            "NameError at 2:7",
            "undefined variable second line");
    }

    void test_zero_division_error_type()
    {
        expect_throws<ZeroDivisionError>("print(10 / 0)\n", "division by zero type");
    }

    void test_zero_division_error_location()
    {
        expect_error_contains<ZeroDivisionError>(
            "print(10 / 0)\n",
            "ZeroDivisionError at 1:10",
            "division by zero location");
    }

    void test_zero_division_nested_expression()
    {
        expect_error_contains<ZeroDivisionError>(
            "x = 100\n"
            "print(x + 20 / 0)\n",
            "ZeroDivisionError at 2:14",
            "nested division by zero location");
    }

    void test_add_type_error()
    {
        expect_throws<TypeError>("print(10 + \"hello\")\n", "addition type error");
    }

    void test_add_type_error_location()
    {
        expect_error_contains<TypeError>(
            "print(10 + \"hello\")\n",
            "TypeError at 1:10",
            "addition type error location");
    }

    void test_subtraction_type_error_location()
    {
        expect_error_contains<TypeError>(
            "print(\"hello\" - 10)\n",
            "TypeError at 1:15",
            "subtraction type error location");
    }

    void test_multiplication_type_error_location()
    {
        expect_error_contains<TypeError>(
            "print(\"hello\" * 10)\n",
            "TypeError at 1:15",
            "multiplication type error location");
    }

    void test_division_type_error_location()
    {
        expect_error_contains<TypeError>(
            "print(\"hello\" / 10)\n",
            "TypeError at 1:15",
            "division type error location");
    }

    void test_unary_minus_type_error()
    {
        expect_throws<TypeError>("print(-\"hello\")\n", "unary minus type error");
    }

    void test_unary_minus_location()
    {
        expect_error_contains<TypeError>(
            "print(-\"hello\")\n",
            "TypeError at 1:7",
            "unary minus location");
    }

    void test_list_index_error()
    {
        expect_throws<IndexError>(
            "x = [1, 2, 3]\n"
            "print(x[10])\n",
            "list index error");
    }

    void test_negative_list_index_error()
    {
        expect_throws<IndexError>(
            "x = [1, 2, 3]\n"
            "print(x[-10])\n",
            "negative list index error");
    }

    void test_string_index_error()
    {
        expect_throws<IndexError>(
            "x = \"hello\"\n"
            "print(x[100])\n",
            "string index error");
    }

    void test_list_index_wrong_type()
    {
        expect_throws<TypeError>(
            "x = [1, 2, 3]\n"
            "print(x[\"hello\"])\n",
            "list index wrong type");
    }

    void test_list_index_wrong_type_location()
    {
        expect_error_contains<TypeError>(
            "x = [1, 2, 3]\n"
            "print(x[\"hello\"])\n",
            "TypeError at 2:9",
            "list index wrong type location");
    }

    void test_dictionary_key_error()
    {
        expect_throws<KeyError>(
            "x = {\"name\": \"Daria\"}\n"
            "print(x[\"age\"])\n",
            "dictionary missing key");
    }

    void test_dictionary_key_error_location()
    {
        expect_error_contains<KeyError>(
            "x = {\"name\": \"Daria\"}\n"
            "print(x[\"age\"])\n",
            "KeyError at 2:9",
            "dictionary missing key location");
    }

    void test_dictionary_key_type_error()
    {
        expect_throws<TypeError>(
            "x = {\"name\": \"Daria\"}\n"
            "print(x[10])\n",
            "dictionary key wrong type");
    }

    void test_non_subscriptable()
    {
        expect_throws<TypeError>(
            "x = 10\n"
            "print(x[0])\n",
            "non-subscriptable object");
    }

    void test_non_subscriptable_location()
    {
        expect_error_contains<TypeError>(
            "x = 10\n"
            "print(x[0])\n",
            "TypeError at 2:8",
            "non-subscriptable location");
    }

    void test_non_callable()
    {
        expect_throws<TypeError>(
            "x = 10\n"
            "x()\n",
            "non-callable object");
    }

    void test_non_callable_location()
    {
        expect_error_contains<TypeError>(
            "x = 10\n"
            "x()\n",
            "TypeError at 2:2",
            "non-callable location");
    }

    void test_too_few_function_arguments()
    {
        expect_throws<TypeError>(
            "def add(a, b):\n"
            "    return a + b\n"
            "\n"
            "add(1)\n",
            "too few function arguments");
    }

    void test_too_many_function_arguments()
    {
        expect_throws<TypeError>(
            "def f(a):\n"
            "    return a\n"
            "\n"
            "f(1, 2)\n",
            "too many function arguments");
    }

    void test_function_argument_error_location()
    {
        expect_error_contains<TypeError>(
            "def f(a):\n"
            "    return a\n"
            "\n"
            "f(1, 2)\n",
            "TypeError at 4:2",
            "function argument error location");
    }

    void test_builtin_type_error()
    {
        expect_throws<TypeError>("len(10)\n", "builtin type error");
    }

    void test_builtin_type_error_location()
    {
        expect_error_contains<TypeError>(
            "len(10)\n",
            "TypeError at 1:4",
            "builtin error location");
    }

    void test_int_value_error()
    {
        expect_throws<ValueError>("int(\"hello\")\n", "int ValueError");
    }

    void test_int_value_error_location()
    {
        expect_error_contains<ValueError>("int(\"hello\")\n", "ValueError at 1:4", "int ValueError location");
    }

    void test_range_zero_step_value_error()
    {
        expect_throws<ValueError>("range(1, 10, 0)\n", "range zero step ValueError");
    }

    void test_range_zero_step_location()
    {
        expect_error_contains<ValueError>(
            "range(1, 10, 0)\n",
            "ValueError at 1:6",
            "range zero step location");
    }

    void test_unexpected_character()
    {
        expect_throws<SyntaxError>("x = @\n", "unexpected character");
    }

    void test_unexpected_character_location()
    {
        expect_error_contains<SyntaxError>(
            "x = @\n",
            "SyntaxError at 1:5",
            "unexpected character location");
    }

    void test_unterminated_string()
    {
        expect_throws<SyntaxError>(
            "x = \"hello\n",
            "unterminated string");
    }

    void test_missing_expression()
    {
        expect_throws<SyntaxError>("x = 10 +\n", "missing expression");
    }

    void test_missing_colon()
    {
        expect_throws<SyntaxError>(
            "if True\n"
            "    print(1)\n",
            "missing colon");
    }

    void test_break_outside_loop()
    {
        expect_throws<SyntaxError>("break\n", "break outside loop");
    }

    void test_break_location()
    {
        expect_error_contains<SyntaxError>(
            "break\n",
            "SyntaxError at 1:1",
            "break outside loop location");
    }

    void test_continue_outside_loop()
    {
        expect_throws<SyntaxError>("continue\n", "continue outside loop");
    }

    void test_return_outside_function()
    {
        expect_throws<SyntaxError>("return 10\n", "return outside function");
    }

    void test_duplicate_parameters()
    {
        expect_throws<SyntaxError>(
            "def f(x, x):\n"
            "    return x\n",
            "duplicate parameters");
    }

}

void run_error_tests()
{
    test_name_error_type();
    test_name_error_location();
    test_name_error_second_line();

    test_zero_division_error_type();
    test_zero_division_error_location();
    test_zero_division_nested_expression();

    test_add_type_error();
    test_add_type_error_location();
    test_subtraction_type_error_location();
    test_multiplication_type_error_location();
    test_division_type_error_location();

    test_unary_minus_type_error();
    test_unary_minus_location();

    test_list_index_error();
    test_negative_list_index_error();
    test_string_index_error();

    test_list_index_wrong_type();
    test_list_index_wrong_type_location();

    test_dictionary_key_error();
    test_dictionary_key_error_location();
    test_dictionary_key_type_error();

    test_non_subscriptable();
    test_non_subscriptable_location();

    test_non_callable();
    test_non_callable_location();

    test_too_few_function_arguments();
    test_too_many_function_arguments();
    test_function_argument_error_location();

    test_builtin_type_error();
    test_builtin_type_error_location();

    test_int_value_error();
    test_int_value_error_location();
    test_range_zero_step_value_error();
    test_range_zero_step_location();

    test_unexpected_character();
    test_unexpected_character_location();
    test_unterminated_string();

    test_missing_expression();
    test_missing_colon();

    test_break_outside_loop();
    test_break_location();
    test_continue_outside_loop();
    test_return_outside_function();
    test_duplicate_parameters();
}