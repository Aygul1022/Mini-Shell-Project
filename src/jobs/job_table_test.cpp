#include "job_table.h"

#include <cassert>
#include <iostream>

int main()
{
    JobTable jobTable;

    int jobId = jobTable.addJob(
        {4231, 4232},
        4231,
        "ls | grep cpp &",
        true
    );

    assert(jobId == 1);

    const Job* job = jobTable.findJob(jobId);

    assert(job != nullptr);
    assert(job->pgid == 4231);
    assert(job->pids.size() == 2);
    assert(job->background == true);
    assert(job->status == JobStatus::Running);

    std::cout << "PASS: Add and find job\n";

    const Job* firstProcessJob = jobTable.findJobByPid(4231);
    const Job* secondProcessJob = jobTable.findJobByPid(4232);
    const Job* unknownProcessJob = jobTable.findJobByPid(9999);

    assert(firstProcessJob != nullptr);
    assert(firstProcessJob->jobId == jobId);

    assert(secondProcessJob != nullptr);
    assert(secondProcessJob->jobId == jobId);

    assert(unknownProcessJob == nullptr);

    std::cout << "PASS: Find job by PID\n";

    bool updated = jobTable.updateJobStatus(
        jobId,
        JobStatus::Stopped
    );

    assert(updated);

    job = jobTable.findJob(jobId);
    assert(job != nullptr);
    assert(job->status == JobStatus::Stopped);

    std::cout << "PASS: Update job status\n";

    bool removed = jobTable.removeJob(jobId);

    assert(removed);
    assert(jobTable.findJob(jobId) == nullptr);
    assert(jobTable.findJobByPid(4231) == nullptr);

    std::cout << "PASS: Remove job\n";

    return 0;
}