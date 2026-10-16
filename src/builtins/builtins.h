#ifndef MINI_SHELL_BUILTINS_H
#define MINI_SHELL_BUILTINS_H

#include <string>
#include <vector>

enum class BuiltinResult {
    NotBuiltin,
    Handled,
    ExitRequested
};

bool isBuiltin(const std::string& name);

// args[0] is the command name. The caller must first check pipeline,
// redirection, and background restrictions. Runs in the calling process.
// Handled means the command was recognized, not necessarily successful.
BuiltinResult dispatchBuiltin(const std::vector<std::string>& args);

#endif
