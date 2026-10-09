	#include "pipeline_executor.h"

#include <iostream>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>

std::vector<char*> buildArgs(const Command& command) {
    std::vector<char*> args;

    for (const auto& item : command.args) {
        args.push_back(const_cast<char*>(item.c_str()));
    }

    args.push_back(nullptr);

    return args;
}

void executePipeline(const Pipeline& pipeline) {
    if (pipeline.commands.empty()) {
        std::cerr << "Pipeline bos olamaz." << std::endl;
        return;
    }

    for (const auto& command : pipeline.commands) {
        if (command.args.empty()) {
            std::cerr << "Bos komut calistirilamaz." << std::endl;
            return;
        }
    }

    std::vector<pid_t> childPids;

    int previousReadFd = -1;

    for (std::size_t i = 0; i < pipeline.commands.size(); ++i) {
        bool hasNextCommand =
            (i + 1 < pipeline.commands.size());

        int pipefd[2] = {-1, -1};

        if (hasNextCommand) {
            if (pipe(pipefd) == -1) {
                std::cerr << "pipe() basarisiz!" << std::endl;

                if (previousReadFd != -1) {
                    close(previousReadFd);
                }

                for (pid_t childPid : childPids) {
                    waitpid(childPid, nullptr, 0);
                }

                return;
            }
        }

        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "fork() basarisiz!" << std::endl;

            if (previousReadFd != -1) {
                close(previousReadFd);
            }

            if (hasNextCommand) {
                close(pipefd[0]);
                close(pipefd[1]);
            }

            for (pid_t childPid : childPids) {
                waitpid(childPid, nullptr, 0);
            }

            return;
        }

        if (pid == 0) {
            if (previousReadFd != -1) {
                if (dup2(previousReadFd, STDIN_FILENO) == -1) {
                    std::cerr << "stdin dup2() basarisiz!"
                              << std::endl;
                    _exit(1);
                }
            }

            if (hasNextCommand) {
                if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                    std::cerr << "stdout dup2() basarisiz!"
                              << std::endl;
                    _exit(1);
                }
            }

            if (previousReadFd != -1) {
                close(previousReadFd);
            }

            if (hasNextCommand) {
                close(pipefd[0]);
                close(pipefd[1]);
            }

            std::vector<char*> args =
                buildArgs(pipeline.commands[i]);

            execvp(args[0], args.data());

            std::cerr << "Komut calistirilamadi: "
                      << pipeline.commands[i].args[0]
                      << std::endl;

            _exit(127);
        }

        childPids.push_back(pid);

        if (previousReadFd != -1) {
            close(previousReadFd);
        }

        if (hasNextCommand) {
            close(pipefd[1]);
            previousReadFd = pipefd[0];
        }
        else {
            previousReadFd = -1;
        }
    }

    for (pid_t childPid : childPids) {
        waitpid(childPid, nullptr, 0);
    }
}
