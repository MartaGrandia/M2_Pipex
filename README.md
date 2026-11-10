# 🔌 pipex

> A low-level C implementation simulating the UNIX shell pipeline mechanism, managing inter-process communication (IPC), file descriptor redirection, and process execution.

## 📖 Overview

`pipex` reproduces the exact mechanics of the UNIX pipeline construct (`< file1 cmd1 | cmd2 > file2`). The project focuses on handling low-level system calls, spawning child processes using `fork()`, establishing unidirectional inter-process communication channels via `pipe()`, redirecting standard file descriptors using `dup2()`, and resolving/executing binaries with `execve()`.

## ⚙️ Technical Constraints & Architecture

- **Language:** C (compiled with `gcc -Wall -Wextra -Werror`).
- **Allowed POSIX Syscalls:** `open`, `close`, `read`, `write`, `malloc`, `free`, `perror`, `strerror`, `access`, `dup`, `dup2`, `execve`, `exit`, `fork`, `pipe`, `unlink`, `wait`, `waitpid`.
- **Memory & Resource Safety:** 100% leak-free execution. All heap-allocated environment arrays, argument vectors, and file descriptors (pipe ends and open file handles) are systematically closed and freed across all execution branches (audited via Valgrind).
- **Error Handling:** Gracefully handles invalid command inputs, non-existent files, permission errors, and execution failures while replicating standard shell exit codes.
- **Standards:** 100% compliant with 42's **Norminette** coding standard.

---

## 🏛️ Inter-Process Communication Architecture

`pipex` connects the output stream of the first command directly to the input stream of the second command using a system pipe.

### Execution Pipeline Step-by-Step

1. **Environment Parsing (`envp`):** Extracts the `$PATH` variable from the environment array, splits paths by `:`, and validates executable access using `access(path, X_OK)`.
2. **Pipe Creation (`pipe`):** Instantiates an integer array `pipefd[2]` where `pipefd[0]` represents the read end and `pipefd[1]` represents the write end.
3. **Process Forking (`fork`):**
   - **Child 1:** Redirects `file1` to `stdin`, redirects `pipefd[1]` to `stdout`, closes unused file descriptors, and calls `execve(cmd1)`.
   - **Child 2:** Redirects `pipefd[0]` to `stdin`, redirects `file2` to `stdout`, closes unused file descriptors, and calls `execve(cmd2)`.
4. **Parent Process Synchronization:** Closes both pipe ends in the parent process and waits for both child processes using `waitpid()` to prevent zombie processes and gather exit codes.

---

## 🛠️ Quickstart & Execution

### Program Syntax

```bash
./pipex file1 cmd1 cmd2 file2
< file1 cmd1 | cmd2 > file2

# Clone the repository
git clone [https://github.com/your-username/pipex.git](https://github.com/your-username/pipex.git)
cd pipex

# Compile binary
make

# Clean object files
make clean

# Clean object files and binary
make fclean

# Recompile project
make re

# Example 1: Count lines of a file section
./pipex infile "ls -l" "wc -l" outfile
# Shell equivalent: < infile ls -l | wc -l > outfile

# Example 2: Search for specific keywords and count words
./pipex infile "grep a" "wc -w" outfile
# Shell equivalent: < infile grep a | wc -w > outfile
