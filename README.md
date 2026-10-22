# Linux Mini-Shell Project

**COME 361 – Operating Systems Term Project**

## 1. Project Overview

This project aims to develop a simplified Linux shell using **C++17** and **POSIX system calls**.

The Mini-Shell reads commands from the user, parses their arguments and operators, and executes supported commands in a Linux environment.

The main purpose of the project is to understand fundamental operating system concepts, including process creation, program execution, process synchronization, input/output redirection, inter-process communication, and job management.

## 2. Features

### Currently Implemented

- Interactive command-line interface with the `mini-shell>` prompt.
- Command tokenization and parsing.
- Support for single and double quotes.
- Built-in commands: `cd`, `pwd`, `help`, and `exit`.
- External command execution using `fork()`, `execvp()`, and `waitpid()`.
- Input redirection using `<`.
- Output redirection using `>`.
- Append redirection using `>>`.
- Foreground pipelines using `|`.
- Recognition and parsing of the background operator `&`.
- Job representation and JobTable data structures for tracking processes and job states.

### Planned Features / Integration Work

- Actual background command execution.
- Process group management.
- Job control commands: `jobs`, `fg`, and `bg`.
- Signal handling for job control.
- Redirection within pipelines.
- Integration of JobTable with the running shell.
- Final integration tests and documentation.

**Note:** Background execution and job control are not yet supported by the integrated shell. The parser recognizes the `&` operator, but the current shell intentionally rejects background execution until executor integration is completed.

## 3. Technologies and Requirements

- **Programming Language:** C++17
- **Operating System:** Ubuntu Linux or Windows Subsystem for Linux (WSL)
- **Compiler:** g++
- **Version Control:** Git and GitHub
- **Testing:** C++ assertions and Python 3 integration tests

### Main POSIX APIs

- `fork()` – Create child processes.
- `execvp()` – Execute external programs.
- `waitpid()` – Wait for child processes.
- `pipe()` – Create communication channels between processes.
- `dup2()` – Redirect standard input and output.
- `open()` and `close()` – Manage file descriptors.
- `chdir()` and `getcwd()` – Support directory-related built-in commands.

## 4. Installation

Clone the project repository:

```bash
git clone https://github.com/Aygul1022/Mini-Shell-Project.git
cd Mini-Shell-Project
```

The current runnable shell integration is available on the following development branch:

```bash
git checkout feature/executeCommandandtest
```

**Development status:** The `main` branch currently contains the shared command structures and parser. The complete foreground shell integration is still under review and has not yet been merged into `main`.

## 5. Compilation and Execution

Open a terminal in the project directory and compile the integrated shell:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
    src/main.cpp \
    src/builtins/builtins.cpp \
    src/parser/parser.cpp \
    src/parser/tokenizer.cpp \
    src/process/executor.cpp \
    src/process/pipeline_executor.cpp \
    -o mini-shell
```

Run the program:

```bash
./mini-shell
```

The shell should display:

```text
mini-shell>
```

Enter a supported command and press Enter. To exit, type:

```text
exit
```

## 6. Example Commands / Mini Demo

The following commands demonstrate the currently integrated functionality:

```bash
pwd
help
echo "Hello from Mini Shell"
ls
echo hello | cat
echo first > output.txt
echo second >> output.txt
cat < output.txt
cd ..
pwd
exit
```

These examples demonstrate:

- Reading and parsing user commands.
- Executing built-in commands.
- Creating child processes to execute external programs.
- Connecting processes through pipelines.
- Redirecting input and output using file descriptors.

**Important:** Commands such as `sleep 10 &`, `jobs`, `fg`, and `bg` are planned for a later integration stage and should not be used as examples of completed functionality.

## 7. Testing

### Parser and Tokenizer

The tokenizer and parser modules have been developed and merged into the `main` branch.

The modules have passed **51 parser/tokenizer tests**, covering command arguments, quotes, redirection operators, pipelines, background syntax, and error handling.

### JobTable

The JobTable module is available on the `feature/jobs` branch.

Its **7 tests** cover:

1. Rejection of invalid process information.
2. Adding and finding jobs.
3. Finding jobs by PID.
4. Updating job status.
5. Removing jobs.
6. Removing completed jobs.
7. Preserving a retrieved Job copy after removal from the table.

To compile and run these tests, first switch to the JobTable branch:

```bash
git checkout feature/jobs
g++ -std=c++17 -Wall -Wextra -Wpedantic \
    src/jobs/job_table_test.cpp \
    -o /tmp/job_table_test
/tmp/job_table_test
```

### Shell Integration Tests

The integrated foreground shell has passed **17 Python integration tests** in Ubuntu/WSL.

To reproduce these tests, use the integration branch:

```bash
git checkout feature/executeCommandandtest
```

Compile the shell as described in Section 5, then run:

```bash
python3 tests/parser_integration_test.py ./mini-shell
```

Expected result:

```text
Ran 17 tests
OK
```

Test results apply to the corresponding development branches. Final combined testing will be performed after all modules are integrated.

## 8. Project Structure

The project is organized into the following main modules:

```text
Mini-Shell-Project/
├── docs/
│   ├── architecture.md
│   └── shell-fundamentals.md
├── src/
│   ├── command.h
│   ├── main.cpp
│   ├── parser/
│   │   ├── tokenizer.h
│   │   ├── tokenizer.cpp
│   │   ├── parser.h
│   │   └── parser.cpp
│   ├── builtins/
│   │   ├── builtins.h
│   │   └── builtins.cpp
│   ├── process/
│   │   ├── executor.h
│   │   ├── executor.cpp
│   │   ├── pipeline_executor.h
│   │   └── pipeline_executor.cpp
│   └── jobs/
│       ├── job.h
│       ├── job_table.h
│       └── job_table_test.cpp
└── tests/
    ├── parser_integration_test.py
    └── testing.md
```

**Note:** This structure represents the modules developed across the project branches. Not all directories have been merged into one branch yet.

## 9. Team Members and Task Distribution

| Team Member | Responsibilities | Current Progress |
|---|---|---|
| **Aygül** | Shell core, input loop, prompt, built-in commands, shell integration, testing and documentation | Implemented shell input handling, built-ins, and foreground command/pipeline integration. |
| **Ümran** | Tokenizer, parser, quoted arguments, redirection and pipeline syntax, background parsing, Job and JobTable structures, testing | Completed parser and JobTable modules, with passing unit tests. |
| **Sami** | Process execution, `fork/execvp/waitpid`, pipelines, redirection execution, background processes, signals and job control | Implemented foreground process execution and pipelines. Background and job-control integration remains in progress. |

### Next Interim Submission Goals

**Aygül:** Continue coordinating integration of shell core, process execution, and job management; extend integration tests and documentation.

**Ümran:** Complete JobTable code review and support integration of parser and JobTable interfaces with the shell and executor; contribute integration tests.

**Sami:** Extend process execution to support background processes, PID/PGID tracking, signal handling, and job-control functionality.

## 10. Repository and Development Workflow

The project uses GitHub for version control and collaboration.

**Repository:**

https://github.com/Aygul1022/Mini-Shell-Project

The team develops features in separate branches and reviews them through pull requests before integration.

The main development branches include:

- `main` – Shared interface and merged parser.
- `feature/jobs` – Job and JobTable implementation.
- `feature/process-execution` – Process execution and pipeline module.
- `feature/executeCommandandtest` – Integrated shell and foreground execution.

## 11. Current Project Status

The initial project skeleton, parser, process execution foundation, built-in commands, and JobTable structures have been implemented.

A working foreground Mini-Shell is available for demonstration. The next development phase focuses on connecting job management to real processes, adding background execution and signal handling, and integrating all modules into a single tested application.

---

**COME 361 – Operating Systems Term Project**

**Project:** Linux Mini-Shell Using C++ and POSIX System Calls