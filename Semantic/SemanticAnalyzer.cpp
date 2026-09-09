#include "Semantic/SemanticAnalyzer.h"

#include <stdexcept>
#include <unordered_set>

#include "AST/Statement/BlockStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/ForStatement.h"
#include "AST/Statement/FunctionStatement.h"
#include "AST/Statement/ReturnStatement.h"
#include "AST/Statement/BreakStatement.h"
#include "AST/Statement/ContinueStatement.h"

void SemanticAnalyzer::analyze(const Statement &program)
{
    loop_depth = 0;
    function_depth = 0;

    visit(program);
}

void SemanticAnalyzer::visit(const Statement &statement)
{
    if (auto block = dynamic_cast<const BlockStatement *>(&statement))
    {
        for (const auto &child : block->get_statements())
        {
            visit(*child);
        }
    }

    else if (auto if_statement = dynamic_cast<const IfStatement *>(&statement))
    {
        visit(if_statement->get_then());

        if (const Statement *else_branch = if_statement->get_else_branch())
        {
            visit(*else_branch);
        }
    }

    else if (auto while_statement = dynamic_cast<const WhileStatement *>(&statement))
    {
        loop_depth++;

        visit(while_statement->get_body());

        loop_depth--;
    }

    else if (auto for_statement = dynamic_cast<const ForStatement *>(&statement))
    {
        loop_depth++;

        visit(for_statement->get_body());

        loop_depth--;
    }

    else if (auto function = dynamic_cast<const FunctionStatement *>(&statement))
    {
        // Check duplicate parameters
        std::unordered_set<std::string> names;

        for (const std::string &parameter : function->get_parameters())
        {
            if (!names.insert(parameter).second)
            {
                throw std::runtime_error("Duplicate parameter: " + parameter);
            }
        }

        int old_loop_depth = loop_depth;

        loop_depth = 0;
        function_depth++;

        visit(function->get_body());

        function_depth--;
        loop_depth = old_loop_depth;
    }

    else if (dynamic_cast<const ReturnStatement *>(&statement))
    {
        if (function_depth == 0)
        {
            throw std::runtime_error("'return' outside function");
        }
    }

    else if (dynamic_cast<const BreakStatement *>(&statement))
    {
        if (loop_depth == 0)
        {
            throw std::runtime_error("'break' outside loop");
        }
    }

    else if (dynamic_cast<const ContinueStatement *>(&statement))
    {
        if (loop_depth == 0)
        {
            throw std::runtime_error("'continue' outside loop");
        }
    }
}