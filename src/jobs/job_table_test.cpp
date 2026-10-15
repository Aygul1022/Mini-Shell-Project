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
        true);

    assert(jobId == 1);

    const Job *job = jobTable.findJob(jobId);

    assert(job != nullptr);
    assert(job->pgid == 4231);
    assert(job->pids.size() == 2);
    assert(job->background == true);
    assert(job->status == JobStatus::Running);

    std::cout << "PASS: Add and find job\n";

    const Job *firstProcessJob = jobTable.findJobByPid(4231);
    const Job *secondProcessJob = jobTable.findJobByPid(4232);
    const Job *unknownProcessJob = jobTable.findJobByPid(9999);

    assert(firstProcessJob != nullptr);
    assert(firstProcessJob->jobId == jobId);

    assert(secondProcessJob != nullptr);
    assert(secondProcessJob->jobId == jobId);

    assert(unknownProcessJob == nullptr);

    std::cout << "PASS: Find job by PID\n";

    bool updated = jobTable.updateJobStatus(
        jobId,
        JobStatus::Stopped);

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

    JobTable cleanupTable;

    int runningId = cleanupTable.addJob(
        {5001}, 5001, "sleep 10 &", true);

    int doneId1 = cleanupTable.addJob(
        {5002}, 5002, "sleep 20 &", true);

    int stoppedId = cleanupTable.addJob(
        {5003}, 5003, "sleep 30 &", true);

    int doneId2 = cleanupTable.addJob(
        {5004}, 5004, "sleep 40 &", true);

    assert(cleanupTable.updateJobStatus(
        doneId1, JobStatus::Done));

    assert(cleanupTable.updateJobStatus(
        stoppedId, JobStatus::Stopped));

    assert(cleanupTable.updateJobStatus(
        doneId2, JobStatus::Done));

    cleanupTable.removeDoneJobs();

    assert(cleanupTable.findJob(doneId1) == nullptr);
    assert(cleanupTable.findJob(doneId2) == nullptr);

    const Job *runningJob = cleanupTable.findJob(runningId);
    const Job *stoppedJob = cleanupTable.findJob(stoppedId);

    assert(runningJob != nullptr);
    assert(runningJob->status == JobStatus::Running);

    assert(stoppedJob != nullptr);
    assert(stoppedJob->status == JobStatus::Stopped);

    std::cout << "PASS: Remove completed jobs\n";

    return 0;
}