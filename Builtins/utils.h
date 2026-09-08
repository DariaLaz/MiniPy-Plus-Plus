#include "AST/Environment.h"
#include "Builtins/Builtin.h"

template <typename T>
std::shared_ptr<Builtin> make_builtin();

void install_builtins(Environment &env);