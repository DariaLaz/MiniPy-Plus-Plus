#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Semantic/SemanticAnalyzer.h"
#include "Builtins/utils.h"
#include "Runtime/Environment.h"

inline std::string run_source(const std::string &source)
{
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    SemanticAnalyzer analyzer;
    analyzer.analyze(*program);

    auto env = std::make_shared<Environment>();
    install_builtins(*env);

    std::ostringstream output;

    std::streambuf *old_buffer =
        std::cout.rdbuf(output.rdbuf());

    try
    {
        program->execute(*env);
    }
    catch (...)
    {
        std::cout.rdbuf(old_buffer);
        throw;
    }

    std::cout.rdbuf(old_buffer);

    return output.str();
}

inline void expect_equal(const std::string &actual, const std::string &expected, const std::string &test_name)
{
    if (actual != expected)
    {
        throw std::runtime_error(
            "FAILED: " + test_name +
            "\nExpected:\n" + expected +
            "\nActual:\n" + actual);
    }
}

template <typename ErrorType>
void expect_throws(const std::string &source, const std::string &test_name)
{
    try
    {
        run_source(source);
    }
    catch (const ErrorType &)
    {
        return;
    }
    catch (const std::exception &error)
    {
        throw std::runtime_error(
            "FAILED: " + test_name +
            "\nExpected a different error type."
            "\nActual error: " +
            std::string(error.what()));
    }

    throw std::runtime_error(
        "FAILED: " + test_name +
        "\nExpected an exception, but none was thrown.");
}

template <typename ErrorType>
void expect_error_contains(const std::string &source, const std::string &expected_text, const std::string &test_name)
{
    try
    {
        run_source(source);
    }
    catch (const ErrorType &error)
    {
        std::string actual = error.what();

        if (actual.find(expected_text) == std::string::npos)
        {
            throw std::runtime_error(
                "FAILED: " + test_name +
                "\nExpected error to contain:\n" +
                expected_text +
                "\nActual error:\n" +
                actual);
        }

        return;
    }
    catch (const std::exception &error)
    {
        throw std::runtime_error(
            "FAILED: " + test_name +
            "\nExpected another error type."
            "\nActual error: " +
            std::string(error.what()));
    }

    throw std::runtime_error(
        "FAILED: " + test_name +
        "\nExpected an exception, but none was thrown.");
}