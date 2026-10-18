#include "executor.h"
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <vector>

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void executeCommand(const Command& command) {
    if (command.args.empty()) {
        std::cerr << "Empty command cannot be executed." << std::endl;
        return;
    }

    std::vector<char*> args;

    for (const auto& item : command.args) {
        args.push_back(const_cast<char*>(item.c_str()));
    }

    args.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork() failed!" << std::endl;
        return;
    }

    if (pid == 0) {

        // < input redirection
        if (!command.inputFile.empty()) {
            int inputFd = open(
                command.inputFile.c_str(),
                O_RDONLY
            );

            if (inputFd == -1) {
                std::cerr << "Failed to open Input file!" << std::endl;
                _exit(1);
            }

            if (dup2(inputFd, STDIN_FILENO) == -1) {
                std::cerr << "Input dup2() failed!" << std::endl;
                close(inputFd);
                _exit(1);
            }

            close(inputFd);
        }

        // > veya >> output redirection
        if (!command.outputFile.empty()) {
            int flags = O_WRONLY | O_CREAT;

            if (command.appendOutput) {
                flags |= O_APPEND;
            }
            else {
                flags |= O_TRUNC;
            }

            int outputFd = open(
                command.outputFile.c_str(),
                flags,
                0644
            );

            if (outputFd == -1) {
                std::cerr << "Failed to open Output file!" << std::endl;
                _exit(1);
            }

            if (dup2(outputFd, STDOUT_FILENO) == -1) {
                std::cerr << "Output dup2() failed!" << std::endl;
                close(outputFd);
                _exit(1);
            }

            close(outputFd);
        }

        execvp(args[0], args.data());

        std::cerr << "Command Execution Failed: "
                  << command.args[0] << std::endl;

        _exit(127);
    }

    while (waitpid(pid, nullptr, 0) == -1) {
    if (errno == EINTR) {
        continue;
    }

    std::perror("waitpid failed");
    break;
}
}
