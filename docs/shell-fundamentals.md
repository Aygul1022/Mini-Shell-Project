# Shell Fundamentals

## 1. What Is a Shell?

A shell is a command-line interpreter that allows users to interact with the operating system by entering commands.

## 2. How Does a Shell Execute a Command?

When a user enters a command, the shell generally follows these steps:

1. **Read input:** The shell reads the command entered by the user.
2. **Parse input:** It separates the command name from its arguments.
3. **Identify the command:** It determines whether the command is a built-in command or an external program.
4. **Execute the command:** Built-in commands are handled by the shell. External programs are typically executed in a child process using `fork()` and `execvp()`.
5. **Wait for completion:** For foreground commands, the shell usually waits for the child process using `waitpid()`.
6. **Display the prompt:** After execution, the shell displays the prompt again and waits for another command.

## 3. Built-in Commands vs. External Commands

* **Built-in commands:** Commands implemented directly inside the shell, such as `cd`, `help`, and `exit`.
* **External commands:** Separate programs executed by the shell, such as `ls`, `date`, and `whoami`.

The `cd` command should be implemented as a built-in because changing the working directory of a child process would not change the working directory of the parent shell.

## 4. Our Mini Shell

Our project will implement a basic command-line shell with a prompt, command parsing, built-in commands, external process execution, input/output redirection, pipelines, and background job management.
