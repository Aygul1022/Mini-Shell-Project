
#include "../parser/parser.h"
#include "job_table.h"

#include <cassert>
#include <iostream>

int main()
{
    // Verify that the parser recognizes background syntax.
    ParseResult result = parseCommandLine("sleep 10 &");

    assert(result.success);
    assert(result.pipeline.background);
    assert(result.pipeline.commands.size() == 1);

    const Command& command = result.pipeline.commands[0];
    assert(command.args.size() == 2);
    assert(command.args[0] == "sleep");
    assert(command.args[1] == "10");

    std::cout << "PASS: Background command parsing\n";

    // Simulated process information for an interface test.
    // No real process is launched here.
    const pid_t testPid = 4242;

    JobTable jobTable;
    int jobId = jobTable.addJob(
        {testPid}, testPid, "sleep 10 &", true
    );

    assert(jobId > 0);

    auto job = jobTable.findJob(jobId);

    assert(job.has_value());
    assert(job->pgid == testPid);
    assert(job->background);
    assert(job->status == JobStatus::Running);

    std::cout << "PASS: Parser-to-JobTable data contract\n";

    return 0;
}
