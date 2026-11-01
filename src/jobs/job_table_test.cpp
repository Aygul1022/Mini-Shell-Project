#include "job_table.h"

#include <cassert>
#include <iostream>

int main()
{
    JobTable validationTable;

    assert(validationTable.addJob({}, 4231, "empty PID list", true) == 0);
    assert(validationTable.addJob({4231}, -1, "invalid PGID", true) == 0);
    assert(validationTable.addJob({0}, 4231, "invalid PID", true) == 0);
    assert(validationTable.addJob({4231}, 9999, "PGID not in PID list", true) == 0);

    int firstValidId = validationTable.addJob(
        {4231}, 4231, "valid job", true);

    assert(firstValidId == 1);

    std::cout << "PASS: Reject invalid process information\n";

    JobTable jobTable;

    int jobId = jobTable.addJob(
        {4231, 4232},
        4231,
        "ls | grep cpp &",
        true);

    assert(jobId == 1);

    std::optional<Job> job = jobTable.findJob(jobId);

    assert(job.has_value());
    assert(job->pgid == 4231);
    assert(job->pids.size() == 2);
    assert(job->background == true);
    assert(job->status == JobStatus::Running);

    std::cout << "PASS: Add and find job\n";

    std::optional<Job> firstProcessJob = jobTable.findJobByPid(4231);
    std::optional<Job> secondProcessJob = jobTable.findJobByPid(4232);
    std::optional<Job> unknownProcessJob = jobTable.findJobByPid(9999);

    assert(firstProcessJob.has_value());
    assert(firstProcessJob->jobId == jobId);

    assert(secondProcessJob.has_value());
    assert(secondProcessJob->jobId == jobId);

    assert(!unknownProcessJob.has_value());

    std::cout << "PASS: Find job by PID\n";

    bool updated = jobTable.updateJobStatus(
        jobId,
        JobStatus::Stopped);

    assert(updated);

    job = jobTable.findJob(jobId);

    assert(job.has_value());
    assert(job->status == JobStatus::Stopped);

    std::cout << "PASS: Update job status\n";

    bool removed = jobTable.removeJob(jobId);

    assert(removed);
    assert(!jobTable.findJob(jobId).has_value());
    assert(!jobTable.findJobByPid(4231).has_value());

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

    assert(!cleanupTable.findJob(doneId1).has_value());
    assert(!cleanupTable.findJob(doneId2).has_value());

    std::optional<Job> runningJob = cleanupTable.findJob(runningId);
    std::optional<Job> stoppedJob = cleanupTable.findJob(stoppedId);

    assert(runningJob.has_value());
    assert(runningJob->status == JobStatus::Running);

    assert(stoppedJob.has_value());
    assert(stoppedJob->status == JobStatus::Stopped);

    std::cout << "PASS: Remove completed jobs\n";

    JobTable safetyTable;

    int safetyId = safetyTable.addJob(
        {6001}, 6001, "sleep 10 &", true);

    std::optional<Job> savedJob = safetyTable.findJob(safetyId);

    assert(savedJob.has_value());

    safetyTable.removeJob(safetyId);

    assert(!safetyTable.findJob(safetyId).has_value());
    assert(savedJob->jobId == safetyId);
    assert(savedJob->commandLine == "sleep 10 &");

    std::cout << "PASS: Job copy remains valid after removal\n";

    return 0;
}
