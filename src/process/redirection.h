#ifndef PROCESS_REDIRECTION_H
#define PROCESS_REDIRECTION_H

#include "../command.h"

#include <iostream>

#include <fcntl.h>
#include <unistd.h>

namespace process {

inline bool redirectFile(const std::string& path, int flags, int targetFd,
                         const char* streamName) {
    const int fileFd = open(path.c_str(), flags, 0644);
    if (fileFd == -1) {
        std::cerr << "Failed to open " << streamName << " file!" << std::endl;
        return false;
    }

    if (fileFd == targetFd) {
        return true;
    }

    const int result = dup2(fileFd, targetFd);
    close(fileFd);
    if (result == -1) {
        std::cerr << streamName << " dup2() failed!" << std::endl;
        return false;
    }

    return true;
}

inline bool applyRedirections(const Command& command) {
    if (!command.inputFile.empty() &&
        !redirectFile(command.inputFile, O_RDONLY, STDIN_FILENO, "Input")) {
        return false;
    }

    if (command.outputFile.empty()) {
        return true;
    }

    const int flags = O_WRONLY | O_CREAT |
                      (command.appendOutput ? O_APPEND : O_TRUNC);
    return redirectFile(command.outputFile, flags, STDOUT_FILENO, "Output");
}

} // namespace process

#endif
