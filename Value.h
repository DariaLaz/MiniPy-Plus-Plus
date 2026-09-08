#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

struct ListValue;

using Value = std::variant<int, bool, std::string, std::shared_ptr<ListValue>>;

struct ListValue
{
    std::vector<Value> elements;
};

void print_value(const Value &value);

bool is_truthy(const Value &value);

Value add(const Value &left, const Value &right);
