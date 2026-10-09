#include "executor.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void executeCommand(std::vector<std::string> tokens) {
    std::vector<char*> args;

    for (auto& item : tokens) {
        args.push_back(item.data());
    }

    args.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork() basarisiz!" << std::endl;
        return;
    }

    if (pid == 0) {
        execvp(args[0], args.data());

        std::cerr << "Komut calistirilamadi: "
                  << tokens[0] << std::endl;

        _exit(127);
    }

    waitpid(pid, nullptr, 0);
}
