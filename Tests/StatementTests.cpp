#include <string>

#include "TestUtils.h"

namespace
{
    void test_assignment()
    {
        auto output = run_source(
            "x = 10\n"
            "print(x)\n");

        expect_equal(output, "10\n", "assignment");
    }

    void test_reassignment()
    {
        auto output = run_source(
            "x = 10\n"
            "x = 20\n"
            "print(x)\n");

        expect_equal(output, "20\n", "reassignment");
    }

    void test_assignment_from_expression()
    {
        auto output = run_source(
            "x = 10\n"
            "y = x * 2 + 5\n"
            "print(y)\n");

        expect_equal(output, "25\n", "assignment from expression");
    }

    void test_list_index_assignment()
    {
        auto output = run_source(
            "x = [10, 20, 30]\n"
            "x[1] = 99\n"
            "print(x)\n");

        expect_equal(output, "[10, 99, 30]\n", "list index assignment");
    }

    void test_negative_list_index_assignment()
    {
        auto output = run_source(
            "x = [10, 20, 30]\n"
            "x[-1] = 99\n"
            "print(x)\n");

        expect_equal(output, "[10, 20, 99]\n", "negative list index assignment");
    }

    void test_dictionary_existing_key_assignment()
    {
        auto output = run_source(
            "x = {\"name\": \"Daria\", \"age\": 20}\n"
            "x[\"age\"] = 21\n"
            "print(x[\"age\"])\n");

        expect_equal(output, "21\n", "dictionary existing key assignment");
    }

    void test_dictionary_new_key_assignment()
    {
        auto output = run_source(
            "x = {\"name\": \"Daria\"}\n"
            "x[\"city\"] = \"Sofia\"\n"
            "print(x[\"city\"])\n");

        expect_equal(output, "Sofia\n", "dictionary new key assignment");
    }

    void test_if_true()
    {
        auto output = run_source(
            "x = 10\n"
            "if x > 5:\n"
            "    print(\"yes\")\n");

        expect_equal(output, "yes\n", "if true");
    }

    void test_if_false()
    {
        auto output = run_source(
            "x = 1\n"
            "if x > 5:\n"
            "    print(\"yes\")\n"
            "print(\"done\")\n");

        expect_equal(output, "done\n", "if false");
    }

    void test_if_else()
    {
        auto output = run_source(
            "x = 2\n"
            "if x > 5:\n"
            "    print(\"large\")\n"
            "else:\n"
            "    print(\"small\")\n");

        expect_equal(output, "small\n", "if else");
    }

    void test_if_elif_else()
    {
        auto output = run_source(
            "x = 10\n"
            "if x < 10:\n"
            "    print(\"small\")\n"
            "elif x == 10:\n"
            "    print(\"ten\")\n"
            "else:\n"
            "    print(\"large\")\n");

        expect_equal(output, "ten\n", "if elif else");
    }

    void test_while_loop()
    {
        auto output = run_source(
            "x = 0\n"
            "while x < 3:\n"
            "    print(x)\n"
            "    x = x + 1\n");

        expect_equal(
            output,
            "0\n"
            "1\n"
            "2\n",
            "while loop");
    }

    void test_while_break()
    {
        auto output = run_source(
            "x = 0\n"
            "while x < 10:\n"
            "    if x == 3:\n"
            "        break\n"
            "    print(x)\n"
            "    x = x + 1\n");

        expect_equal(
            output,
            "0\n"
            "1\n"
            "2\n",
            "while break");
    }

    void test_while_continue()
    {
        auto output = run_source(
            "x = 0\n"
            "while x < 5:\n"
            "    x = x + 1\n"
            "    if x == 3:\n"
            "        continue\n"
            "    print(x)\n");

        expect_equal(
            output,
            "1\n"
            "2\n"
            "4\n"
            "5\n",
            "while continue");
    }

    void test_for_loop()
    {
        auto output = run_source(
            "for i in range(5):\n"
            "    print(i)\n");

        expect_equal(
            output,
            "0\n"
            "1\n"
            "2\n"
            "3\n"
            "4\n",
            "for loop");
    }

    void test_for_break()
    {
        auto output = run_source(
            "for i in range(10):\n"
            "    if i == 3:\n"
            "        break\n"
            "    print(i)\n");

        expect_equal(
            output,
            "0\n"
            "1\n"
            "2\n",
            "for break");
    }

    void test_for_continue()
    {
        auto output = run_source(
            "for i in range(5):\n"
            "    if i == 2:\n"
            "        continue\n"
            "    print(i)\n");

        expect_equal(
            output,
            "0\n"
            "1\n"
            "3\n"
            "4\n",
            "for continue");
    }

    void test_break_only_exits_inner_loop()
    {
        auto output = run_source(
            "for i in range(3):\n"
            "    for j in range(3):\n"
            "        if j == 1:\n"
            "            break\n"
            "        print(i)\n");

        expect_equal(
            output,
            "0\n"
            "1\n"
            "2\n",
            "break exits nearest loop");
    }

    void test_function_call()
    {
        auto output = run_source(
            "def add(a, b):\n"
            "    return a + b\n"
            "\n"
            "print(add(10, 20))\n");

        expect_equal(output, "30\n", "function call");
    }

    void test_function_local_variable()
    {
        auto output = run_source(
            "def f():\n"
            "    x = 42\n"
            "    return x\n"
            "\n"
            "print(f())\n");

        expect_equal(output, "42\n", "function local variable");
    }

    void test_function_implicit_none()
    {
        auto output = run_source(
            "def f():\n"
            "    x = 10\n"
            "\n"
            "print(f())\n");

        expect_equal(output, "None\n", "function implicit None");
    }

    void test_early_return()
    {
        auto output = run_source(
            "def f():\n"
            "    print(1)\n"
            "    return 10\n"
            "    print(2)\n"
            "\n"
            "print(f())\n");

        expect_equal(
            output,
            "1\n"
            "10\n",
            "early return");
    }

    void test_recursion()
    {
        auto output = run_source(
            "def factorial(n):\n"
            "    if n <= 1:\n"
            "        return 1\n"
            "    return n * factorial(n - 1)\n"
            "\n"
            "print(factorial(5))\n");

        expect_equal(output, "120\n", "recursion");
    }

    void test_local_variable_shadows_global()
    {
        auto output = run_source(
            "x = 10\n"
            "\n"
            "def f():\n"
            "    x = 20\n"
            "    print(x)\n"
            "\n"
            "f()\n"
            "print(x)\n");

        expect_equal(
            output,
            "20\n"
            "10\n",
            "local variable shadows global");
    }

    void test_lexical_scope()
    {
        auto output = run_source(
            "x = 1\n"
            "\n"
            "def f():\n"
            "    return x\n"
            "\n"
            "def g():\n"
            "    x = 999\n"
            "    return f()\n"
            "\n"
            "print(g())\n");

        expect_equal(output, "1\n", "lexical scope");
    }

    void test_closure()
    {
        auto output = run_source(
            "def outer():\n"
            "    x = 10\n"
            "\n"
            "    def inner():\n"
            "        return x\n"
            "\n"
            "    return inner\n"
            "\n"
            "f = outer()\n"
            "print(f())\n");

        expect_equal(output, "10\n", "closure");
    }

    void test_closure_survives_outer_function()
    {
        auto output = run_source(
            "def make_value():\n"
            "    x = 42\n"
            "\n"
            "    def get_value():\n"
            "        return x\n"
            "\n"
            "    return get_value\n"
            "\n"
            "f = make_value()\n"
            "print(f())\n"
            "print(f())\n");

        expect_equal(
            output,
            "42\n"
            "42\n",
            "closure survives outer function");
    }

}

void run_statement_tests()
{
    test_assignment();
    test_reassignment();
    test_assignment_from_expression();

    test_list_index_assignment();
    test_negative_list_index_assignment();
    test_dictionary_existing_key_assignment();
    test_dictionary_new_key_assignment();

    test_if_true();
    test_if_false();
    test_if_else();
    test_if_elif_else();

    test_while_loop();
    test_while_break();
    test_while_continue();

    test_for_loop();
    test_for_break();
    test_for_continue();

    test_break_only_exits_inner_loop();

    test_function_call();
    test_function_local_variable();
    test_function_implicit_none();
    test_early_return();

    test_recursion();

    test_local_variable_shadows_global();
    test_lexical_scope();

    test_closure();
    test_closure_survives_outer_function();
}