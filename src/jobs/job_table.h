#ifndef JOB_TABLE_H
#define JOB_TABLE_H

#include "job.h"
#include <algorithm>
#include <iostream>
#include <vector>

class JobTable
{
private:
    std::vector<Job> jobs;
    int nextJobId = 1;

    static bool isValidProcessInfo(const std::vector<pid_t> &pids, pid_t pgid)
    {
        if (pids.empty() || pgid <= 0)
        {
            return false;
        }

        for (pid_t pid : pids)
        {
            if (pid <= 0)
            {
                return false;
            }
        }

        return std::find(pids.begin(), pids.end(), pgid) != pids.end();
    }

    static const char *statusToString(JobStatus status)
    {
        switch (status)
        {
        case JobStatus::Running:
            return "Running";
        case JobStatus::Stopped:
            return "Stopped";
        case JobStatus::Done:
            return "Done";
        }

        return "Unknown";
    }

public:
    int addJob(const std::vector<pid_t> &pids,
               pid_t pgid,
               const std::string &commandLine,
               bool background)
    {
        if (!isValidProcessInfo(pids, pgid))
        {
            return 0;
        }

        Job job;

        job.jobId = nextJobId++;
        job.pgid = pgid;
        job.pids = pids;
        job.commandLine = commandLine;
        job.status = JobStatus::Running;
        job.background = background;

        jobs.push_back(job);

        return job.jobId;
    }

    const Job *findJob(int jobId) const
    {
        for (const Job &job : jobs)
        {
            if (job.jobId == jobId)
            {
                return &job;
            }
        }

        return nullptr;
    }

    const Job *findJobByPid(pid_t pid) const
    {
        for (const Job &job : jobs)
        {
            for (pid_t processId : job.pids)
            {
                if (processId == pid)
                {
                    return &job;
                }
            }
        }

        return nullptr;
    }

    bool updateJobStatus(int jobId, JobStatus newStatus)
    {
        for (Job &job : jobs)
        {
            if (job.jobId == jobId)
            {
                job.status = newStatus;
                return true;
            }
        }

        return false;
    }

    bool removeJob(int jobId)
    {
        for (auto it = jobs.begin(); it != jobs.end(); ++it)
        {
            if (it->jobId == jobId)
            {
                jobs.erase(it);
                return true;
            }
        }

        return false;
    }

    void removeDoneJobs()
    {
        for (auto it = jobs.begin(); it != jobs.end();)
        {
            if (it->status == JobStatus::Done)
            {
                it = jobs.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void listJobs() const
    {
        for (const Job &job : jobs)
        {
            std::cout << "[" << job.jobId << "] "
                      << statusToString(job.status)
                      << " - " << job.commandLine
                      << '\n';
        }
    }
};

#endif
