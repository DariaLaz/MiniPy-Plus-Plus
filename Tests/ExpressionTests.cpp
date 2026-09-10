#include <string>

#include "TestUtils.h"

#include "Errors/TypeError.h"
#include "Errors/NameError.h"
#include "Errors/IndexError.h"
#include "Errors/KeyError.h"
#include "Errors/ZeroDivisionError.h"

namespace
{
    void test_integer_arithmetic()
    {
        auto output = run_source(
            "print(10 + 5)\n"
            "print(10 - 5)\n"
            "print(10 * 5)\n"
            "print(10 / 5)\n");

        expect_equal(
            output,
            "15\n"
            "5\n"
            "50\n"
            "2\n",
            "integer arithmetic");
    }

    void test_operator_precedence()
    {
        auto output = run_source(
            "print(2 + 3 * 4)\n"
            "print((2 + 3) * 4)\n"
            "print(20 / 2 + 3)\n");

        expect_equal(
            output,
            "14\n"
            "20\n"
            "13\n",
            "operator precedence");
    }

    void test_unary_operators()
    {
        auto output = run_source(
            "print(-5)\n"
            "print(+5)\n"
            "print(-(-10))\n");

        expect_equal(
            output,
            "-5\n"
            "5\n"
            "10\n",
            "unary operators");
    }

    void test_string_concatenation()
    {
        auto output = run_source("print(\"hello\" + \" world\")\n");

        expect_equal(
            output,
            "hello world\n",
            "string concatenation");
    }

    void test_integer_comparisons()
    {
        auto output = run_source(
            "print(5 < 10)\n"
            "print(5 <= 5)\n"
            "print(10 > 5)\n"
            "print(10 >= 10)\n"
            "print(5 == 5)\n"
            "print(5 != 10)\n");

        expect_equal(
            output,
            "True\n"
            "True\n"
            "True\n"
            "True\n"
            "True\n"
            "True\n",
            "integer comparisons");
    }

    void test_equality()
    {
        auto output = run_source(
            "print(10 == 10)\n"
            "print(10 == 20)\n"
            "print(\"abc\" == \"abc\")\n"
            "print(\"abc\" != \"xyz\")\n"
            "print(True == True)\n"
            "print(False != True)\n");

        expect_equal(
            output,
            "True\n"
            "False\n"
            "True\n"
            "True\n"
            "True\n"
            "True\n",
            "equality expressions");
    }

    void test_logical_not()
    {
        auto output = run_source(
            "print(not True)\n"
            "print(not False)\n"
            "print(not 0)\n"
            "print(not 1)\n");

        expect_equal(
            output,
            "False\n"
            "True\n"
            "True\n"
            "False\n",
            "logical not");
    }

    void test_logical_and_or()
    {
        auto output = run_source(
            "print(True and True)\n"
            "print(True and False)\n"
            "print(False or True)\n"
            "print(False or False)\n");

        expect_equal(
            output,
            "True\n"
            "False\n"
            "True\n"
            "False\n",
            "logical and or");
    }

    void test_and_short_circuit()
    {
        auto output = run_source("print(False and undefined_variable)\n");

        expect_equal(output, "False\n", "and short circuit");
    }

    void test_or_short_circuit()
    {
        auto output = run_source("print(True or undefined_variable)\n");

        expect_equal(output, "True\n", "or short circuit");
    }

    void test_variable_expression()
    {
        auto output = run_source(
            "x = 42\n"
            "print(x)\n");

        expect_equal(output, "42\n", "variable expression");
    }

    void test_undefined_variable()
    {
        expect_throws<NameError>("print(missing)\n", "undefined variable");
    }

    void test_list_literal()
    {
        auto output = run_source("print([1, 2, 3])\n");

        expect_equal(output, "[1, 2, 3]\n", "list literal");
    }

    void test_list_indexing()
    {
        auto output = run_source(
            "x = [10, 20, 30]\n"
            "print(x[0])\n"
            "print(x[1])\n"
            "print(x[2])\n");

        expect_equal(
            output,
            "10\n"
            "20\n"
            "30\n",
            "list indexing");
    }

    void test_negative_list_indexing()
    {
        auto output = run_source(
            "x = [10, 20, 30]\n"
            "print(x[-1])\n"
            "print(x[-2])\n"
            "print(x[-3])\n");

        expect_equal(
            output,
            "30\n"
            "20\n"
            "10\n",
            "negative list indexing");
    }

    void test_list_index_out_of_range()
    {
        expect_throws<IndexError>(
            "x = [1, 2, 3]\n"
            "print(x[10])\n",
            "list index out of range");
    }

    void test_list_index_wrong_type()
    {
        expect_throws<TypeError>(
            "x = [1, 2, 3]\n"
            "print(x[\"hello\"])\n",
            "list index wrong type");
    }

    void test_string_indexing()
    {
        auto output = run_source(
            "x = \"hello\"\n"
            "print(x[0])\n"
            "print(x[1])\n"
            "print(x[-1])\n");

        expect_equal(
            output,
            "h\n"
            "e\n"
            "o\n",
            "string indexing");
    }

    void test_string_index_out_of_range()
    {
        expect_throws<IndexError>(
            "x = \"hello\"\n"
            "print(x[100])\n",
            "string index out of range");
    }

    void test_list_slicing()
    {
        auto output = run_source(
            "x = [1, 2, 3, 4, 5]\n"
            "print(x[1:4])\n"
            "print(x[:3])\n"
            "print(x[2:])\n"
            "print(x[:])\n");

        expect_equal(
            output,
            "[2, 3, 4]\n"
            "[1, 2, 3]\n"
            "[3, 4, 5]\n"
            "[1, 2, 3, 4, 5]\n",
            "list slicing");
    }

    void test_string_slicing()
    {
        auto output = run_source(
            "x = \"hello\"\n"
            "print(x[1:4])\n"
            "print(x[:2])\n"
            "print(x[2:])\n"
            "print(x[:])\n");

        expect_equal(
            output,
            "ell\n"
            "he\n"
            "llo\n"
            "hello\n",
            "string slicing");
    }

    void test_slice_bounds_clamped()
    {
        auto output = run_source(
            "x = [1, 2, 3]\n"
            "print(x[100:200])\n"
            "print(x[-100:100])\n");

        expect_equal(
            output,
            "[]\n"
            "[1, 2, 3]\n",
            "slice bounds clamped");
    }

    void test_slice_wrong_index_type()
    {
        expect_throws<TypeError>(
            "x = [1, 2, 3]\n"
            "print(x[\"a\":2])\n",
            "slice wrong index type");
    }

    void test_dictionary_indexing()
    {
        auto output = run_source(
            "x = {\"name\": \"Daria\", \"age\": 20}\n"
            "print(x[\"name\"])\n"
            "print(x[\"age\"])\n");

        expect_equal(
            output,
            "Daria\n"
            "20\n",
            "dictionary indexing");
    }

    void test_dictionary_missing_key()
    {
        expect_throws<KeyError>(
            "x = {\"name\": \"Daria\"}\n"
            "print(x[\"age\"])\n",
            "dictionary missing key");
    }

    void test_dictionary_wrong_key_type()
    {
        expect_throws<TypeError>(
            "x = {\"name\": \"Daria\"}\n"
            "print(x[1])\n",
            "dictionary wrong key type");
    }

    void test_invalid_addition()
    {
        expect_throws<TypeError>("print(10 + \"hello\")\n", "invalid addition");
    }

    void test_invalid_subtraction()
    {
        expect_throws<TypeError>("print(\"hello\" - 1)\n", "invalid subtraction");
    }

    void test_invalid_multiplication()
    {
        expect_throws<TypeError>("print(\"hello\" * 2)\n", "invalid multiplication");
    }

    void test_invalid_division()
    {
        expect_throws<TypeError>("print(\"hello\" / 2)\n", "invalid division");
    }

    void test_invalid_comparison()
    {
        expect_throws<TypeError>("print(\"hello\" < 10)\n", "invalid comparison");
    }

    void test_unary_minus_wrong_type()
    {
        expect_throws<TypeError>("print(-\"hello\")\n", "unary minus wrong type");
    }

    void test_division_by_zero()
    {
        expect_throws<ZeroDivisionError>("print(10 / 0)\n", "division by zero");
    }

    void test_non_subscriptable_value()
    {
        expect_throws<TypeError>(
            "x = 10\n"
            "print(x[0])\n",
            "non subscriptable value");
    }

    void test_none_expression()
    {
        auto output = run_source("print(None)\n");

        expect_equal(output, "None\n", "None expression");
    }

}

void run_expression_tests()
{
    test_integer_arithmetic();
    test_operator_precedence();
    test_unary_operators();

    test_string_concatenation();

    test_integer_comparisons();
    test_equality();

    test_logical_not();
    test_logical_and_or();
    test_and_short_circuit();
    test_or_short_circuit();

    test_variable_expression();
    test_undefined_variable();

    test_list_literal();
    test_list_indexing();
    test_negative_list_indexing();
    test_list_index_out_of_range();
    test_list_index_wrong_type();

    test_string_indexing();
    test_string_index_out_of_range();

    test_list_slicing();
    test_string_slicing();
    test_slice_bounds_clamped();
    test_slice_wrong_index_type();

    test_dictionary_indexing();
    test_dictionary_missing_key();
    test_dictionary_wrong_key_type();

    test_invalid_addition();
    test_invalid_subtraction();
    test_invalid_multiplication();
    test_invalid_division();
    test_invalid_comparison();
    test_unary_minus_wrong_type();

    test_division_by_zero();
    test_non_subscriptable_value();

    test_none_expression();
}