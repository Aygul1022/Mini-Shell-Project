# Mini Shell test record

Current scope is **Parser-only dispatch** below. All subsequent development sections are historical records; their build commands and expectations may be superseded.

**Current prerequisite satisfied (2026-10-16):** merge commit `61806c9` brings the shared interface and parser into `ayguldev`. The required source files are present. Process/executor sources are not required for the current parser-only scope.

## Parser-only dispatch — current scope

At the user's explicit request, executor includes and calls were removed. Input is sent to `parseCommandLine(input)`; syntax errors are printed and empty results skipped. The dispatcher checks arguments before using `command.args[0]`, then routes cd, pwd, help, and exit. The temporary cd splitter remains removed. All external commands, pipelines, redirection, and background requests report that executor integration is pending, before any built-in side effect occurs. Help describes this scope.

Status: **Passed — user-pasted Ubuntu build and runtime results, 2026-10-16**. Dependencies are present through merge commit `61806c9`. The combined C++17 build with `-Wall -Wextra -Wpedantic` printed `Build passed` with no visible compiler diagnostics. Running `python3 tests/parser_integration_test.py /tmp/minishell-parser-test` reported `Ran 9 tests in 0.027s` and `OK`. This verifies the current parser-only integration, including rejection of executor features; it does not establish external execution, pipes, redirection, or background-job execution.

Approved structural refactor: the built-in handlers and routing now live in `src/builtins/builtins.cpp`, with the public declaration in `src/builtins/builtins.h`. `main.cpp` retains the input loop, parser call, and pipeline restrictions. `dispatchBuiltin()` returns NotBuiltin, Handled, or ExitRequested; Handled does not imply command success. ExitRequested tells the main loop to stop. No child process is introduced, so cd still changes the shell's working directory. The nine regression tests passed against this layout in the user's Ubuntu environment.

After merging the parser/shared interface, run from Ubuntu Bash in the project directory:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp src/builtins/builtins.cpp src/parser/parser.cpp src/parser/tokenizer.cpp -o /tmp/minishell-parser-test && python3 tests/parser_integration_test.py /tmp/minishell-parser-test
```

Expected: nine methods pass and the test runner prints `OK`. The three former executor tests now verify external-command rejection with loop recovery, pipeline rejection, and rejection of input/output/append redirection without creating files. The other six cover whitespace, quoted paths, syntax errors, unsupported forms without side effects, argument validation, repeated help, and a final line without newline.

Manual check after building: launch `/tmp/minishell-parser-test`, enter `  pwd  `, `help`, `ls`, `pwd | cat`, `pwd > output.txt`, `cd /tmp &`, and `exit`. Built-ins should work; all four execution forms should report pending integration and return to the prompt. No output file should be created and the background cd must not change the working directory.

Learning note: parsing and execution are separate. Successful parsing does not authorize running a built-in while ignoring its redirection, pipeline, or background fields.

## Input handling — 2026-10-15

Scope: `src/main.cpp`, approved input-handling changes on `ayguldev`. Results refer to the working-tree implementation tested during this conversation, not a new committed release.

Environment: user-run Ubuntu terminal through WSL. Exact distribution and compiler versions were not recorded. The agent could not access WSL; the request for elevated access was declined. Agent compilation and runtime verification are therefore **Blocked**.

### Build and terminal context

The following build command was supplied for execution from Ubuntu Bash in the project directory:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp -o /tmp/minishell-input-test
```

The user ran the executable and reported results below. No compiler transcript was supplied; compiler warnings and exact build provenance were not independently verified.

Commands containing `printf`, pipes, redirection, or `echo $?` belong in **Ubuntu Bash**, not at `mini-shell>`. This version of the mini-shell echoes ordinary input and does not execute external programs or pipelines.

### IN-01: Final line without a newline

Status: **Passed — user-pasted output**.

Run from Ubuntu Bash:

```bash
printf 'hello' | /tmp/minishell-input-test
```

Expected: print `You entered: hello`, then exit when no further input is available. A final prompt is printed before the failed read.

Observed output, with the surrounding Bash prompt omitted:

```text
mini-shell> You entered: hello
mini-shell>
```

The pasted transcript confirms return to Bash. An earlier attempt entered this command inside the mini-shell, which merely echoed it; that attempt did not exercise the intended test.

Learning note: a successful `getline()` may reach EOF after extracting characters. Checking EOF alone would discard this valid final line.

### IN-02: Empty and whitespace-only input

Status: **Passed — user-reported confirmation** of the interactive checklist; no per-case transcript supplied.

Start from Ubuntu Bash:

```bash
/tmp/minishell-input-test
```

At `mini-shell>`, press Enter on an empty line, then enter spaces and press Enter, then tabs and press Enter.

Expected and user-reported outcome: a new prompt for each case, without `You entered:`.

Learning note: the whitespace set includes ordinary spaces and tabs. Vertical-tab, form-feed, and carriage-return handling was not individually tested.

### IN-03: Interactive exit

Status: **Passed — user-reported confirmation** of the interactive checklist.

At `mini-shell>`, type `exit` and press Enter.

Expected and user-reported outcome: return to Bash.

### IN-04: Interactive EOF

Status: **Passed — user-reported confirmation** of the interactive checklist.

Start the executable from Bash again, then press Ctrl+D on an empty input line.

Expected and user-reported outcome: return to Bash.

Learning note: Ctrl+D at an empty terminal input line lets the read encounter EOF; it is not a literal command named Ctrl+D.

### IN-05: Exit without a trailing newline

Status: **Not run / no specific result reported**.

Run from Ubuntu Bash:

```bash
printf 'exit' | /tmp/minishell-input-test
```

Expected: return to Bash without printing `You entered: exit`. The later general confirmation answered the interactive checklist and does not establish this separate result.

### IN-06: Closed standard input attempt

Status: **Withdrawn as a reliable non-EOF error test; branch remains unverified**.

Originally suggested from Ubuntu Bash:

```bash
/tmp/minishell-input-test <&-
echo $?
```

The original expectation was an input-error diagnostic and status `1`. The user supplied a command line containing duplicated text (`<&-put-test <&-`) and output showing a prompt followed by `0`, without the diagnostic.

This does not establish a code failure. The submitted command is ambiguous, and closing standard input is not a dependable way to induce the specific C++ stream state needed to exercise the non-EOF error branch. The original expected result was corrected during the conversation. No replacement test has been run.

### CHECK-01: Diff whitespace validation

Status: **Passed — agent tool result** for the source change, before this test document was added.

```bash
git diff --check
```

Observed: exit status `0`, with no whitespace errors reported. This checks patch formatting, not compilation or runtime correctness.

## Remaining coverage

- IN-05 has no specific reported result.
- The non-EOF input-error diagnostic and status `1` remain unverified.
- Additional whitespace characters were not individually tested.
- No parser, executor, pipeline, or background-job integration tests were performed in this development.

## Built-in pwd — 2026-10-15

Scope: approved exact-command `pwd` implementation in `src/main.cpp`, in the uncommitted working tree. Uses `getcwd()` with a growing buffer; reports retrieval errors without terminating the loop. Options, arguments, and surrounding whitespace await parser integration.

Runtime status: **PWD-01 passed — user-pasted Ubuntu transcript**. PWD-02 and PWD-03 remain pending. Agent WSL access remains unavailable following the earlier declined access request.

### PWD-01: Build and compare with Bash

Status: **Passed — user-pasted output and explicit confirmation**.

From Ubuntu Bash in the project directory:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp -o /tmp/minishell-pwd-test
pwd -P
/tmp/minishell-pwd-test
```

Expected build: success; report any diagnostics. At `mini-shell>`, type `pwd` and press Enter twice, once per command. Each result should match Bash's physical path printed above, followed by another prompt. Type `exit` to return to Bash.

Observed: the supplied compilation command produced no visible diagnostics. Bash's `pwd -P` and both mini-shell `pwd` calls printed `/mnt/c/Users/aygul/OneDrive/Desktop/OS/Minishell`. Each call returned to the mini-shell prompt, and `exit` returned to Bash. The user confirmed the test passed. No separate compiler exit-status capture was supplied.

Learning note: `getcwd()` retrieves the process's physical working-directory path; plain Bash `pwd` may preserve symbolic-link names, so comparison uses `pwd -P`.

### PWD-02: Inherited directory

Status: **Not run**.

From Ubuntu Bash:

```bash
(cd /tmp && printf 'pwd\nexit\n' | /tmp/minishell-pwd-test)
```

Expected: print `/tmp` and exit. The parentheses keep the directory change inside a Bash subshell, preserving the original Bash directory.

Learning note: the mini-shell inherits its starting directory from its launching process; the path is not hardcoded.

### PWD-03: Input-handling regression

Status: **Not run** for the new executable.

From Ubuntu Bash:

```bash
printf '\n \t\nhello\nexit\n' | /tmp/minishell-pwd-test
```

Expected: blank and whitespace-only lines produce prompts only; `hello` produces `You entered: hello`; `exit` ends the process.

### Untested pwd branches

Buffer growth for paths exceeding the initial 256-byte buffer and `getcwd()` failure diagnostics have not been exercised. A success on an ordinary path does not establish these cases.

## Built-in cd

Scope: approved `changeDirectory()` implementation in `src/main.cpp`, with an argument list including the command name. The parser and executor are developed on teammate branches according to the user, but are not merged or available in this checkout. A temporary whitespace-only adapter enables manual testing; it will be replaced with the shared parser's arguments during integration. Quoting, paths containing spaces, expansion, options, pipelines, and redirection are outside this adapter's scope. No teammate interfaces were changed.

CD-01 and CD-02 **Passed — user-pasted runtime transcripts**. CD-03 through CD-05 remain **Not run**. The earlier declined WSL access request remains respected.

### CD-01: Build, absolute path, and relative path

In Ubuntu Bash, from the project directory:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp -o /tmp/minishell-cd-test
/tmp/minishell-cd-test
```

At `mini-shell>`, enter each line separately:

```text
cd /tmp
pwd
cd ..
pwd
```

Expected: build succeeds; successful `cd` prints nothing. The first `pwd` prints `/tmp`, the second `/`. The loop continues after each command.

Observed: the user transcript shows `cd /tmp` followed by `/tmp`, then `cd ..` followed by `/`, and `exit` returning to Bash. However, the compile command names `/tmp/minishell-cd-testhell-cd-test`, while the executed binary is `/tmp/minishell-cd-test`. The observed behavior is correct for the executed binary, but its relationship to the current source is unverified. Rebuild and execute the same output path before marking this case Passed.

Successful rerun: the user compiled with `g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp -o /tmp/minishell-cd-test && /tmp/minishell-cd-test`. The shell launched, confirming successful compilation through `&&`, with no visible compiler diagnostics. `cd /tmp` followed by `pwd` printed `/tmp`; `cd ..` followed by `pwd` printed `/`; `exit` returned to Bash. This resolves the earlier executable-path mismatch and establishes CD-01 as Passed. Bare `cd` was not included in this rerun, so CD-02 remains pending.

### CD-02: HOME directory

Status: **Passed — user-pasted output**.

In the same mini-shell session, enter `cd`, then `pwd`, then `exit`. Expected: `pwd` displays the physical path of the directory named by HOME. For a comparison, run `(cd "$HOME" && pwd -P)` in Ubuntu Bash.

Observed: Bash's comparison printed `/home/aygul`. In `/tmp/minishell-cd-test`, bare `cd` followed by `pwd` also printed `/home/aygul`. A subsequent `wxit` typo produced `You entered: wxit`, consistent with the current fallback; `exit` then returned to Bash. The typo does not affect the HOME-directory test result.

### CD-03: Errors preserve the current directory

Start the test executable again from Bash. At `mini-shell>`, enter:

```text
cd /tmp
cd /dev/null/child
pwd
cd /tmp /usr
pwd
exit
```

Expected: the first invalid path produces a diagnostic because `/dev/null` is not a directory. Two path arguments produce `mini-shell: cd: too many arguments`. Both `pwd` calls still print `/tmp`, and the shell remains usable.

### CD-04: Missing HOME

In Ubuntu Bash:

```bash
printf 'pwd\ncd\npwd\nexit\n' | env -u HOME /tmp/minishell-cd-test
```

Expected: `mini-shell: cd: HOME is not set`; the two printed working directories match. This removes HOME only from the launched program's environment.

### CD-05: Parent Bash directory is unchanged

In Ubuntu Bash:

```bash
pwd -P
printf 'cd /\npwd\nexit\n' | /tmp/minishell-cd-test
pwd -P
```

Expected: mini-shell prints `/`; both Bash paths match each other. Learning note: `chdir()` changes the calling process's directory. Running it inside mini-shell affects later mini-shell commands, but cannot change its parent Bash process.

The built-in returns `0` on success and `1` on errors for future dispatch integration. The current loop does not yet expose a last-command status; these tests do not claim to verify one.

## Built-in help

Scope: `printHelp()` and exact-command dispatch in `src/main.cpp`, in the working tree. Lists available built-ins, usage, Ctrl+D behavior, and current input/execution limitations. Teammates' parser/executor development is not represented as integrated functionality.

### HELP-01: Build, help output, and continued operation

Status: **Core help behavior passed — user-pasted output and confirmation**. Build, matching help output across two launches, prompt continuation, and unchanged working directory passed. Two consecutive help calls in one process and return to Bash after exit remain unverified by the transcripts. Agent Linux execution remains unavailable following the earlier declined WSL access request.

From Ubuntu Bash in the project directory:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp -o /tmp/minishell-help-test && /tmp/minishell-help-test
```

At `mini-shell>`, enter each line separately:

```text
pwd
help
help
pwd
exit
```

Expected: compilation succeeds. Each `help` prints the same command list containing help, pwd, cd, exit, and Ctrl+D instructions, plus examples and current limitations. Both `pwd` results match, the prompt returns after help, and exit returns to Bash. Report any compiler diagnostics.

Observed: the user launched the executable using the documented build command with `&&`, confirming compilation succeeded; no compiler diagnostics were visible. One `help` call displayed the command list, examples, and limitations. Exact `pwd` calls before and after help both printed `/mnt/c/Users/aygul/OneDrive/Desktop/OS/Minishell`. The transcript does not show a second help call or exit from this executable, so those portions remain unverified.

Additional observation: `pwd ` with a trailing space was echoed as `You entered: pwd `, consistent with the documented exact-string dispatch limitation. Shared parser integration should route by parsed command name and arguments; this transcript does not authorize or verify that integration.

Follow-up: the user rebuilt and launched with `&&`, then supplied a second matching help display from a new shell process. The transcript ends with `mini-shell> exit passed`. It is unclear whether `passed` is user commentary or part of the typed input, and no subsequent Bash prompt is shown. Record the user's success report without claiming a directly observed exit or two consecutive help calls in the same process.

Learning note: help only writes usage text to standard output. It does not change the working directory or launch a process. Its command list must be updated as integration makes additional functionality available.

## Parser integration

Historical checkpoint, superseded by Executor integration below. It was not runtime-tested before the user requested executor integration.

Scope: the shared parser now feeds built-in dispatch. The temporary cd splitter and echo fallback have been removed. Parsed pipelines, background requests, and redirection are rejected before any built-in is called. External commands receive an explicit integration-pending diagnostic. `help`, `pwd`, and `exit` reject arguments; `cd` accepts its parsed path argument, including quoted spaces. Blank parsed results and empty command names are handled safely.

Source provenance: these five files were copied unchanged from GitHub branch `feature/parser`, without a branch merge or commit. Local Git blob hashes match the retrieved file hashes:

| File | Blob SHA |
| --- | --- |
| src/command.h | d5b2e3407a821687690d0daeafc6979626d89ead |
| src/parser/parser.h | 9e4d68a94e9ada44307217a05c7d17b7fda7aae7 |
| src/parser/parser.cpp | d61c2009176bddeafb2198a4d06a80813da416e0 |
| src/parser/tokenizer.h | 5d21a3f4dedf7a1722af65026e5bf79b08be2dec |
| src/parser/tokenizer.cpp | 7cb66190637b89695710115c8fa47b4105210000 |

The parser's standalone test programs were not imported or run. Existing explanatory comments in the user's cd handler were preserved. The earlier declined WSL access request remains respected; Linux compilation and runtime verification are pending.

### INT-01: Build and manual walkthrough

Status: **Not run — user verification pending**.

Run in Ubuntu Bash from the project directory. Compile the production source files explicitly:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp src/parser/parser.cpp src/parser/tokenizer.cpp -o /tmp/minishell-parser-test && /tmp/minishell-parser-test
```

At `mini-shell>`, enter:

```text
  pwd  
cd "/tmp"
pwd
cd / &
pwd
pwd |
help
  exit  
```

Expected: whitespace around built-ins is accepted; quoted `/tmp` is accepted; both subsequent pwd calls print `/tmp` because background cd is rejected. `pwd |` reports a missing-command syntax error. Help prints updated usage and returns to the prompt. Spaced exit returns to Bash. Report any compiler diagnostics.

### INT-02: Automated integration checks

Status: **Not run against Linux executable**. `parser_integration_test.py` uses Python's standard library and temporary directories, including a folder with spaces. It launches the supplied shell executable for each case, checks actual output/errors and side effects, and returns nonzero on test failure.

After the successful build above, run in Ubuntu Bash:

```bash
python3 tests/parser_integration_test.py /tmp/minishell-parser-test
```

Expected: six test methods pass with a final `OK`. Coverage includes spaces/tabs, blank input, single/double quoted paths, syntax-error recovery, unsupported pipeline/background/redirection rejection without changing directory or creating files, external-command diagnostics, built-in argument validation, empty command names, repeated help, exit, and a final line without newline. This verifies rejection of unconnected execution forms, not their execution.

### INT-03: Static checks

Source-copy fidelity: **Passed — agent compared Git blob hashes** for all five imported files. Full `git diff --check` identified trailing spaces in user-added cd comments/blank lines present before this integration; those unrelated lines were preserved. This is not a successful full whitespace check or runtime test.

Learning note: the flow is input -> parser -> syntax/result checks -> execution-form checks -> built-in dispatch. Recognizing `&` or `>` in the parser does not mean execution is supported. Checking those fields before dispatch prevents silently performing a different command than the user requested. The shared parser's existing limitations (including no variable, tilde, or backslash expansion) remain unchanged.

## Executor integration

The dispatcher now calls `executeCommand(const Command&)` for single external commands (including redirection), and `executePipeline(const Pipeline&)` for external pipelines without redirection. Built-ins still run in the shell process. The four process files were temporarily copied unchanged from `feature/process-execution`, then removed at the user's request along with the five parser/interface files. No Git merge or commit was performed. The hashes below identify the inspected interfaces, not files currently present in this checkout.

| File | Retrieved blob SHA |
| --- | --- |
| src/process/executor.h | 7ee932b673fe683dde9ff39e9473236cbd41365d |
| src/process/executor.cpp | 6c6418319b75701aca410b75a28a2a7391ed9813 |
| src/process/pipeline_executor.h | 92aa958a48709f10b3a05f625f7ba3ae21c5ac35 |
| src/process/pipeline_executor.cpp | 5cb98dfd98d3bc19777cdbe747dae2f077a272cf |

Source inspection shows the pipeline executor does not apply redirection and neither executor supports background execution. Those forms remain rejected, as do built-ins in pipelines or with redirection. The executor APIs return void and discard child exit statuses; last-command status propagation is not implemented. These are remaining integration limitations, not completed features.

### EXEC-01: Build combined modules

Status: **Blocked until teammate dependencies are merged; then user Linux verification is pending**. The earlier declined WSL access request remains respected.

From Ubuntu Bash in the project directory:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp src/parser/parser.cpp src/parser/tokenizer.cpp src/process/executor.cpp src/process/pipeline_executor.cpp -o /tmp/minishell-integrated
```

Expected: successful compilation; report any diagnostics. This explicitly excludes teammates' standalone test programs with their own main functions.

### EXEC-02: Combined regression tests

Status: **Not run against Linux executable**.

After a successful build, in Ubuntu Bash:

```bash
python3 tests/parser_integration_test.py /tmp/minishell-integrated
```

Expected: nine test methods and final `OK`. The original six parser/dispatch methods were retained and updated to reflect current unsupported forms. Three methods now verify actual external execution and recovery after a missing program, a three-stage pipeline, and output/input/append redirection with file-content checks. All test files are created inside temporary directories.

### EXEC-03: Interactive smoke test

Status: **Not run**.

Launch `/tmp/minishell-integrated` from Ubuntu Bash, then enter:

```text
ls
echo hello | wc -c
cd /tmp
pwd
exit
```

Expected: directory listing, count `6`, successful cd followed by `/tmp`, then return to Bash. Linux runtime results are required before marking integration passed.

Learning note: the dispatcher selects the route; your teammate's executor owns fork/exec/wait and file-descriptor setup. A merge makes source available but does not supply missing execution semantics automatically.
