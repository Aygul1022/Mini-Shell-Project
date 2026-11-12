# Mini Shell — Weekly Progress and Planning Report

Report date: **31 October 2026**. Team: **Aygül Rana Özcan, Ümran Akpınar, Sami Yusuf**.

Project start: **25 September 2026**, as confirmed by the user. Project weeks run from Friday to Thursday. The eight-week planning period ends on 19 November 2026; this is not a confirmed instructor deadline. The project is currently in Week 6. Actual contributions and proposed future work are recorded separately.

## Current progress summary for the instructor

The team has integrated the C++ shell core, parser/tokenizer, built-in commands, and foreground execution modules. The integrated shell supports input/output redirection for individual external commands and foreground pipelines of external programs. All 17 integration tests passed in Aygül's Ubuntu environment, supported by the 16 October conversation and test record. The 22 October demo rehearsal demonstrated command execution, redirection, and pipeline output. On 31 October, Aygül reported that the instructor was pleased with the demonstrated progress and requested weekly contributions by team member; the precise presentation date has not been inferred from that report.

Ümran's JobTable implementation remains on a separate branch. A completed JobTable module does not mean background processes have been integrated into the running shell. PR #3 and PR #4 were open when checked on 31 October. Adding a graphical terminal interface is a new scope decision; no UI has been implemented yet.

## 1. Actual contributions — evidence-based weekly record

### Week 1 — 25 September–1 October

| Member | Actual work | Evidence / status |
| --- | --- | --- |
| Aygül | Detailed activities for this period have not yet been supplied. | Team confirmation required. |
| Ümran | Detailed activities for this period have not yet been supplied. | Team confirmation required. |
| Sami | Detailed activities for this period have not yet been supplied. | Team confirmation required. |

The project start date is confirmed. There is no separate evidence establishing that topic selection, research, or task allocation occurred during this week. These activities can be added if confirmed by the team.

### Week 2 — 2–8 October

Dated activity details are unavailable for all three members. The earliest inspected Git record is from 9 October; this does not establish that no work occurred before that date. Preparatory activities and individual responsibilities await team confirmation.

### Week 3 — 9–15 October

| Member | Actual contribution | Evidence / limitations |
| --- | --- | --- |
| Aygül | Project skeleton, initial shell loop and prompt; subsequent input/EOF improvements, cd/pwd/help/exit built-in development, and module organization. | 9 October: `6104216`, `25ec652`; 15 October: `4e0369d`; implementation and user-test records in the conversation. Commit dates show when work was recorded. |
| Ümran | Typed tokens, quote handling, tokenizer error checks; parser support for redirection, pipelines, and background syntax; parser tests. Job and JobTable development was recorded separately. | 13 October: `a8aeb20`, `6a07f9f`, `6b0e0a3`; 15 October: `e38759a`, `989356a`, `2a69ba8`, `abdf4e1`; JobTable: `2ecd790`, `314eae8`, `a306f97`. Parser PR #2 merged into main on 15 October. |
| Sami | Single-command process executor, input/output redirection, multi-stage pipeline executor, shared Command/Pipeline interface, and error messages. | 9 October: `84e408c`; 10 October: `6c2fdac`, `bfab540`; 14 October: `19f7b14`, `d536ea9`. Shared-interface PR #1 merged into main on 14 October. |

### Week 4 — 16–22 October

| Member | Actual contribution | Evidence / limitations |
| --- | --- | --- |
| Aygül | Integrated parser and executor through Git; connected foreground dispatch, single-command redirection, and pipelines; updated built-in restrictions and tests. Successive Ubuntu test suites contained 9, 11, 14, and 17 tests. Rehearsed the demo. | 16 October: `61806c9`, `57b31f2`, `c0ca022`, `40cb432`; PR #4; tests/testing.md. Final result: 17 tests in 1.971 seconds, OK. User-provided rehearsal output on 22 October. |
| Ümran | A team message describing parser/JobTable completion and integration coordination was supplied by the user. | The message's precise sending date was not verified. No new code delivery is definitively assigned to this week. |
| Sami | His existing executor interface was used in Aygül's integration; a review request and coordination for subsequent executor work were initiated. | PR #4 review request. No new implementation or PR approval by Sami during this week was verified. |

Team documentation: README PR #5 merged into main on 22 October. Its author was not independently verified for this report, so it has not been attributed to a particular member.

### Week 5 — 23–29 October

| Member | Actual contribution | Evidence / limitations |
| --- | --- | --- |
| Aygül | Troubleshooting of terminal paste markers, incorrect working directory, and the distinction between Bash and mini-shell during demo preparation. | User terminal output dated 23 October. These attempts were not counted as a new successful build or feature test. |
| Ümran | No new activity specifically dated to this week was supplied. | Team confirmation required. |
| Sami | No new activity specifically dated to this week was supplied. | Team confirmation required. |

The user confirmed that a presentation to the instructor took place. Its exact date and individual presentation contributions remain unconfirmed, so they have not been assigned definitively to a particular week.

### Week 6 — 30 October–5 November — in progress

| Member | Actual / reported contribution as of 31 October | Status |
| --- | --- | --- |
| Aygül | Communicated instructor feedback and requested weekly individual reporting and a standalone terminal window. This report and the Weekly Specification skill were prepared with assistant support. | Reporting created; UI not yet implemented. |
| Ümran | No new individual deliverable for this week has been reported. | Awaiting confirmation. |
| Sami | No new individual deliverable for this week has been reported. | Awaiting confirmation. |

## 2. Eight-week roadmap — a plan, not a claim about past activity

Themes for the first five weeks are retrospective classifications. Undocumented preparatory activities are not treated as completed work. The final three weeks are conditional targets for the expanded UI scope; feasibility must be reassessed once the actual deadline and terminal component are agreed.

| Week | Theme / objective | Aygül | Ümran | Sami | Status / completion criterion |
| --- | --- | --- | --- | --- | --- |
| 1 | Requirements and task allocation | Define shell scope | Define syntax scope | Define process scope | Proposed initial theme; historical completion unconfirmed. |
| 2 | Design and module contracts | Loop and dispatcher design | Token/parse-result design | Command/executor contract | Proposed preparation theme; historical completion unconfirmed. |
| 3 | Initial modules | Loop and built-ins | Tokenizer/parser, JobTable | Executor, redirection, pipes | Implementations supported by the commits above. |
| 4 | Foreground integration | Git integration and tests | Parser-interface support | Executor-interface support | Integrated 17-test result available; individual support activities may require attribution/date confirmation. |
| 5 | Demo and feedback | Demo preparation and troubleshooting | Proposed parser presentation | Proposed process presentation | Presentation reported; date and speaking responsibilities unconfirmed. |
| 6 | UI architecture and decisions on remaining work | Window workflow and initial Qt/terminal prototype plan | Proposed error scenarios, JobTable contract, and UI acceptance criteria | Proposed PTY, process-group/signal requirements, and pipeline-redirection work | Subject to team agreement; prototype should launch the existing C++ shell and preserve the 17-test baseline. |
| 7 | Terminal connection and bounded integration | UI–shell connection, responsive window, exit lifecycle | Parser/job integration-test support and consistent diagnostics | PTY resize/signal behavior and agreed executor gaps | Shell runs within GUI; Ctrl+C/Ctrl+D and resizing tested. Add background behavior only when its APIs and tests are ready. |
| 8 | Stabilization, reporting, and presentation | Architecture/workflow documentation, setup, demo | Scenario and regression report | Process and resource-cleanup review | Reproducible build, CLI/GUI test results, and an explicit list of incomplete scope. |

Assignments for the final three weeks are **proposals**, not completed work or confirmed team commitments. Writing full terminal emulation from scratch or delivering all job-control features within the same period would require revisiting the eight-week target. Background/job management remains part of the original requirements; adding a UI neither completes nor cancels it.

## 3. New UI scope — technical decision draft

Goal: operate the existing C++ shell in a dedicated terminal window on Ubuntu/WSL2. Proposed structure: a Qt-based window, a suitable terminal component/PTY connection, and a separate mini-shell process. The same shell process remains alive across commands, preserving state changes such as cd. The UI event loop must remain responsive while a foreground program runs.

Error responsibilities: the tokenizer/parser validates syntax; the executor produces system-call and program-launch diagnostics; the UI reports startup, connection, and lifecycle errors. stdout and stderr typically share a PTY, so reliably classifying errors cannot be based solely on guessing from displayed text; shell-generated diagnostics need an agreed design. Use of an existing component such as QTermWidget must be checked against the instructor's library rules and environment compatibility. Windows/WSLg compatibility was not tested locally in this session.

## 4. Evidence and follow-up

- Detailed test history: [tests/testing.md](../tests/testing.md).
- [PR #1 — shared interface](https://github.com/Aygul1022/Mini-Shell-Project/pull/1): merged.
- [PR #2 — parser/tokenizer](https://github.com/Aygul1022/Mini-Shell-Project/pull/2): merged.
- [PR #3 — JobTable](https://github.com/Aygul1022/Mini-Shell-Project/pull/3): open when checked on 31 October.
- [PR #4 — shell/executor integration](https://github.com/Aygul1022/Mini-Shell-Project/pull/4): open when checked on 31 October; targets feature/process-execution.
- [PR #5 — README](https://github.com/Aygul1022/Mini-Shell-Project/pull/5): merged on 22 October.
- Local Git inspection on 31 October: latest development commit `40cb432`. Existing local changes to tests/testing.md and an output.txt file were preserved while preparing this report.

Next Weekly Specification update: obtain actual activities for the first two weeks, presentation date and individual contributions, the official deadline, new commits/PRs from Ümran and Sami, and confirmation of UI task assignments. Future weeks must not be marked complete without new results.