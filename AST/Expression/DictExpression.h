#pragma once

#include "AST/Expression/Expression.h"
#include "Runtime/Value.h"

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

class DictExpression : public Expression
{
public:
    using Entry = std::pair<std::unique_ptr<Expression>, std::unique_ptr<Expression>>;

    DictExpression(std::vector<Entry> entries, const SourceLocation &location)
        : entries(std::move(entries)), Expression(location)
    {
    }

    Value evaluate(Environment &env) const override
    {
        auto dict = std::make_shared<DictValue>();

        for (const auto &entry : entries)
        {
            Value key_value = entry.first->evaluate(env);

            validate_alternative<std::string>(key_value, entry.first->get_location(),
                                              "Dictionary key must be a string");

            std::string key = std::get<std::string>(key_value);

            Value value = entry.second->evaluate(env);

            dict->elements[key] = value;
        }

        return dict;
    }

private:
    std::vector<Entry> entries;
};