#include <string>

#include "TestUtils.h"

#include "Errors/TypeError.h"
#include "Errors/ValueError.h"

namespace
{
    void test_print_integer()
    {
        auto output = run_source("print(42)\n");

        expect_equal(output, "42\n", "print integer");
    }

    void test_print_string()
    {
        auto output = run_source("print(\"hello\")\n");

        expect_equal(output, "hello\n", "print string");
    }

    void test_print_bool_and_none()
    {
        auto output = run_source(
            "print(True)\n"
            "print(False)\n"
            "print(None)\n");

        expect_equal(
            output,
            "True\n"
            "False\n"
            "None\n",
            "print bool and None");
    }

    void test_print_list()
    {
        auto output = run_source("print([1, 2, 3])\n");

        expect_equal(output, "[1, 2, 3]\n", "print list");
    }

    void test_len_string()
    {
        auto output = run_source("print(len(\"hello\"))\n");

        expect_equal(output, "5\n", "len string");
    }

    void test_len_empty_string()
    {
        auto output = run_source("print(len(\"\"))\n");

        expect_equal(output, "0\n", "len empty string");
    }

    void test_len_list()
    {
        auto output = run_source("print(len([10, 20, 30]))\n");

        expect_equal(output, "3\n", "len list");
    }

    void test_len_empty_list()
    {
        auto output = run_source("print(len([]))\n");

        expect_equal(output, "0\n", "len empty list");
    }

    void test_len_dictionary()
    {
        auto output = run_source("x = {\"a\": 1, \"b\": 2}\n"
                                 "print(len(x))\n");

        expect_equal(output, "2\n", "len dictionary");
    }

    void test_len_wrong_type()
    {
        expect_throws<TypeError>("len(10)\n", "len wrong type");
    }

    void test_len_too_many_arguments()
    {
        expect_throws<TypeError>("len(\"a\", \"b\")\n", "len too many arguments");
    }

    void test_len_no_arguments()
    {
        expect_throws<TypeError>("len()\n", "len no arguments");
    }

    void test_range_one_argument()
    {
        auto output = run_source("print(range(5))\n");

        expect_equal(output, "[0, 1, 2, 3, 4]\n", "range one argument");
    }

    void test_range_two_arguments()
    {
        auto output = run_source("print(range(2, 6))\n");

        expect_equal(output, "[2, 3, 4, 5]\n", "range two arguments");
    }

    void test_range_positive_step()
    {
        auto output = run_source("print(range(1, 10, 2))\n");

        expect_equal(output, "[1, 3, 5, 7, 9]\n", "range positive step");
    }

    void test_range_negative_step()
    {
        auto output = run_source("print(range(5, 0, -1))\n");

        expect_equal(output, "[5, 4, 3, 2, 1]\n", "range negative step");
    }

    void test_range_empty()
    {
        auto output = run_source("print(range(0))\n");

        expect_equal(output, "[]\n", "empty range");
    }

    void test_range_zero_step()
    {
        expect_throws<ValueError>("range(1, 10, 0)\n", "range zero step");
    }

    void test_range_wrong_type()
    {
        expect_throws<TypeError>("range(\"5\")\n", "range wrong type");
    }

    void test_range_no_arguments()
    {
        expect_throws<TypeError>("range()\n", "range no arguments");
    }

    void test_range_too_many_arguments()
    {
        expect_throws<TypeError>("range(1, 2, 3, 4)\n", "range too many arguments");
    }

    void test_append()
    {
        auto output = run_source(
            "x = [1, 2]\n"
            "append(x, 3)\n"
            "print(x)\n");

        expect_equal(output, "[1, 2, 3]\n", "append");
    }

    void test_append_multiple_values()
    {
        auto output = run_source(
            "x = []\n"
            "append(x, 10)\n"
            "append(x, \"hello\")\n"
            "append(x, True)\n"
            "print(x)\n");

        expect_equal(output, "[10, hello, True]\n", "append multiple values");
    }

    void test_append_returns_none()
    {
        auto output = run_source(
            "x = []\n"
            "print(append(x, 10))\n");

        expect_equal(output, "None\n", "append returns None");
    }

    void test_append_wrong_target_type()
    {
        expect_throws<TypeError>("append(10, 20)\n", "append wrong target type");
    }

    void test_append_missing_argument()
    {
        expect_throws<TypeError>("append([])\n", "append missing argument");
    }

    void test_append_too_many_arguments()
    {
        expect_throws<TypeError>("append([], 1, 2)\n", "append too many arguments");
    }

    void test_str_integer()
    {
        auto output = run_source("print(str(123))\n");

        expect_equal(output, "123\n", "str integer");
    }

    void test_str_bool()
    {
        auto output = run_source(
            "print(str(True))\n"
            "print(str(False))\n");

        expect_equal(
            output,
            "True\n"
            "False\n",
            "str bool");
    }

    void test_str_none()
    {
        auto output = run_source("print(str(None))\n");

        expect_equal(output, "None\n", "str None");
    }

    void test_str_string()
    {
        auto output = run_source("print(str(\"hello\"))\n");

        expect_equal(output, "hello\n", "str string");
    }

    void test_str_wrong_argument_count()
    {
        expect_throws<TypeError>("str(1, 2)\n", "str wrong argument count");
    }

    void test_int_from_string()
    {
        auto output = run_source(
            "print(int(\"42\"))\n"
            "print(int(\"-15\"))\n");

        expect_equal(
            output,
            "42\n"
            "-15\n",
            "int from string");
    }

    void test_int_from_integer()
    {
        auto output = run_source("print(int(42))\n");

        expect_equal(output, "42\n", "int from integer");
    }

    void test_int_invalid_string()
    {
        expect_throws<ValueError>("int(\"hello\")\n", "int invalid string");
    }

    void test_int_partially_numeric_string()
    {
        expect_throws<ValueError>("int(\"12abc\")\n", "int partially numeric string");
    }

    void test_int_wrong_type()
    {
        expect_throws<TypeError>("int([1, 2])\n", "int wrong type");
    }

    void test_int_no_arguments()
    {
        expect_throws<TypeError>("int()\n", "int no arguments");
    }

    void test_int_too_many_arguments()
    {
        expect_throws<TypeError>("int(\"1\", \"2\")\n", "int too many arguments");
    }

    void test_builtin_stored_in_variable()
    {
        auto output = run_source(
            "f = len\n"
            "print(f(\"hello\"))\n");

        expect_equal(output, "5\n", "builtin stored in variable");
    }

}

void run_builtin_tests()
{
    test_print_integer();
    test_print_string();
    test_print_bool_and_none();
    test_print_list();

    test_len_string();
    test_len_empty_string();
    test_len_list();
    test_len_empty_list();
    test_len_dictionary();
    test_len_wrong_type();
    test_len_too_many_arguments();
    test_len_no_arguments();

    test_range_one_argument();
    test_range_two_arguments();
    test_range_positive_step();
    test_range_negative_step();
    test_range_empty();
    test_range_zero_step();
    test_range_wrong_type();
    test_range_no_arguments();
    test_range_too_many_arguments();

    test_append();
    test_append_multiple_values();
    test_append_returns_none();
    test_append_wrong_target_type();
    test_append_missing_argument();
    test_append_too_many_arguments();

    test_str_integer();
    test_str_bool();
    test_str_none();
    test_str_string();
    test_str_wrong_argument_count();

    test_int_from_string();
    test_int_from_integer();
    test_int_invalid_string();
    test_int_partially_numeric_string();
    test_int_wrong_type();
    test_int_no_arguments();
    test_int_too_many_arguments();

    test_builtin_stored_in_variable();
}