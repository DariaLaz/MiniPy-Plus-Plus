#include "Builtins/Builtin.h"
#include "Runtime/Environment.h"

template <typename T>
std::shared_ptr<Builtin> make_builtin();

void install_builtins(Environment &env);