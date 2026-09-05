#pragma once

class Expression
{
public:
    virtual ~Expression() noexcept = default;
    virtual int evaluate() const = 0;
};