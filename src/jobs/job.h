#ifndef JOB_H
#define JOB_H

#include <string>
#include <vector>
#include <sys/types.h>

enum class JobStatus {
    Running,
    Stopped,
    Done
};

struct Job {
    int jobId = 0;
    pid_t pgid = -1;
    std::vector<pid_t> pids;
    std::string commandLine;
    JobStatus status = JobStatus::Running;
    bool background = false;
};

#endif
