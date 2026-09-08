#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

class Statement;
class Environment;

struct ListValue;
struct FunctionValue;

using Value = std::variant<
    int,
    bool,
    std::string,
    std::shared_ptr<ListValue>,
    std::shared_ptr<FunctionValue>,
    std::monostate>;

struct ListValue
{
    std::vector<Value> elements;
};

struct FunctionValue
{
    std::vector<std::string> parameters;

    const Statement *body;

    Environment *closure;
};

void print_value(const Value &value);

bool is_truthy(const Value &value);

Value add(const Value &left, const Value &right);
