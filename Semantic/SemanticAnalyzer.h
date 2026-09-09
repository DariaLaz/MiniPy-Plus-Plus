#pragma once

#include "AST/Statement/Statement.h"

class SemanticAnalyzer
{
public:
    void analyze(const Statement &program);

private:
    int loop_depth = 0;
    int function_depth = 0;

    void visit(const Statement &statement);
};