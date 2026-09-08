#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <functional>

class Statement;
class Environment;

struct ListValue;
struct FunctionValue;
struct BuildinFunctionValue;

using Value = std::variant<
    int,
    bool,
    std::string,
    std::shared_ptr<ListValue>,
    std::shared_ptr<FunctionValue>,
    std::shared_ptr<BuildinFunctionValue>,
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

struct BuildinFunctionValue
{
    std::string name;
    std::function<Value(const std::vector<Value> &)> function;
};

std::string value_to_string(const Value &value);

bool is_truthy(const Value &value);

Value add(const Value &left, const Value &right);
