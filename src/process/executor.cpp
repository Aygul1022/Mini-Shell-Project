#include "executor.h"

#include <iostream>
#include <vector>

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void executeCommand(const Command& command) {
    std::vector<char*> args;

    for (const auto& item : command.args) {
        args.push_back(const_cast<char*>(item.c_str()));
    }

    args.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork() basarisiz!" << std::endl;
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
                std::cerr << "Input dosyasi acilamadi!" << std::endl;
                _exit(1);
            }

            if (dup2(inputFd, STDIN_FILENO) == -1) {
                std::cerr << "Input dup2() basarisiz!" << std::endl;
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
                std::cerr << "Output dosyasi acilamadi!" << std::endl;
                _exit(1);
            }

            if (dup2(outputFd, STDOUT_FILENO) == -1) {
                std::cerr << "Output dup2() basarisiz!" << std::endl;
                close(outputFd);
                _exit(1);
            }

            close(outputFd);
        }

        execvp(args[0], args.data());

        std::cerr << "Komut calistirilamadi: "
                  << command.args[0] << std::endl;

        _exit(127);
    }

    waitpid(pid, nullptr, 0);
}
