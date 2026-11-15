#include "executor.h"
#include "redirection.h"
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>

int executeCommand(const Command& command) {
    if (command.args.empty()) {
        std::cerr << "Empty command cannot be executed." << std::endl;
        return 1;
    }

    std::vector<char*> args;

    for (const auto& item : command.args) {
        args.push_back(const_cast<char*>(item.c_str()));
    }

    args.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork() failed!" << std::endl;
        return 1;
    }

    if (pid == 0) {
        if (!process::applyRedirections(command)) {
            _exit(1);
        }

        execvp(args[0], args.data());

        std::cerr << "Command Execution Failed: "
                  << command.args[0] << std::endl;

        _exit(127);
    }

    int status = 0;

    while (waitpid(pid, &status, 0) == -1) {
        if (errno == EINTR) {
            continue;
        }

        std::perror("waitpid failed");
        return 1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }

    return 1;
}
