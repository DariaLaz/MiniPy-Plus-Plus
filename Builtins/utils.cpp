#include "Builtins/AppendBuiltin.h"
#include "Builtins/IntBuiltin.h"
#include "Builtins/LenBuiltin.h"
#include "Builtins/PrintBuiltin.h"
#include "Builtins/RangeBuiltin.h"
#include "Builtins/StrBuiltin.h"
#include "Runtime/Environment.h"

template <typename T>
std::shared_ptr<Builtin> make_builtin()
{
    return std::make_shared<T>();
}

const std::vector<std::function<std::shared_ptr<Builtin>()>> builtins = {
    make_builtin<LenBuiltin>, make_builtin<RangeBuiltin>, make_builtin<PrintBuiltin>,
    make_builtin<AppendBuiltin>, make_builtin<StrBuiltin>, make_builtin<IntBuiltin>};

void install_builtins(Environment &env)
{
    for (const auto &factory : builtins)
    {
        auto builtin = factory();

        env.set(builtin->name(), builtin->make());
    }
}