#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <functional>
#include <stdexcept>
#include "Utils/SourceLocation.h"
#include "Errors/TypeError.h"

class Statement;
class Environment;

struct DictValue;
struct ListValue;
struct FunctionValue;
struct BuildinFunctionValue;

using Value = std::variant<
    int,
    bool,
    std::string,
    std::shared_ptr<ListValue>,
    std::shared_ptr<DictValue>,
    std::shared_ptr<FunctionValue>,
    std::shared_ptr<BuildinFunctionValue>,
    std::monostate>;

struct ListValue
{
    std::vector<Value> elements;
};

struct DictValue
{
    std::unordered_map<std::string, Value> elements;
};

struct FunctionValue
{
    std::vector<std::string> parameters;

    const Statement *body;

    std::shared_ptr<Environment> closure;
};

struct BuildinFunctionValue
{
    std::string name;
    std::function<Value(const std::vector<Value> &, const SourceLocation &)> function;
};

std::string value_to_string(const Value &value, const SourceLocation &location);

bool is_truthy(const Value &value);

Value add(const Value &left, const Value &right, const SourceLocation &location);

template <typename T>
void validate_alternative(const Value &value, const SourceLocation &location, const std::string &message)
{
    if (!std::holds_alternative<T>(value))
    {
        throw TypeError(message, location);
    }
}