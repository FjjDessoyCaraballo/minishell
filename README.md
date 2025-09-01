# Minishell

A lightweight Unix shell implementation that replicates core functionality of bash, developed as part of the 42 School curriculum.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Supported Features](#supported-features)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [Testing](#testing)
- [Known Limitations](#known-limitations)
- [Authors](#authors)

## Overview

Minishell is a simplified shell interpreter that provides a command-line interface for executing programs, managing processes, and interacting with the file system. It implements essential shell features including pipes, redirections, environment variables, and signal handling.

## Features

### Core Functionality
- Command execution with PATH resolution
- Process management with proper parent-child relationships
- Environment variable management
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\)
- Command history with readline library
- Exit status tracking ($?)

### Built-in Commands
- `echo` - Display text (supports -n flag)
- `cd` - Change directory (with relative and absolute paths)
- `pwd` - Print working directory
- `export` - Set environment variables
- `unset` - Remove environment variables
- `env` - Display environment
- `exit` - Exit the shell (with optional exit code)

### Input/Output Features
- Input redirection (`<`)
- Output redirection (`>`)
- Append mode (`>>`)
- Here documents (`<<`)
- Pipes (`|`)
- Multiple pipes support

### Parsing Capabilities
- Single quotes (`'`) - Prevent interpretation of metacharacters
- Double quotes (`"`) - Prevent interpretation except for $
- Environment variable expansion (`$VAR`)
- Exit status expansion (`$?`)

## Installation

### Prerequisites
- GCC or Clang compiler
- GNU Make
- Readline library

### Building from Source

```bash
# Clone the repository
git clone https://github.com/FjjDessoyCaraballo/minishell
cd minishell

# Compile the project
make

# Run the shell
./minishell
```

### Makefile Targets
- `make` - Build the minishell executable
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Rebuild the project from scratch

## Usage

### Basic Command Execution
```bash
# Simple command
ls -la

# Command with arguments
grep "pattern" file.txt

# Using absolute path
/bin/echo "Hello World"
```

### Pipes
```bash
# Single pipe
ls -la | grep ".c"

# Multiple pipes
cat file.txt | grep "search" | wc -l
```

### Redirections
```bash
# Input redirection
< input.txt cat

# Output redirection
echo "Hello" > output.txt

# Append to file
echo "World" >> output.txt

# Here document
cat << EOF
Line 1
Line 2
EOF

# Combined redirections and pipes
< input.txt grep "pattern" | sort > output.txt
```

### Environment Variables
```bash
# Display all environment variables
env

# Set a variable
export VAR=value
export PATH=$PATH:/new/path

# Remove a variable
unset VAR

# Use variables in commands
echo $HOME
echo "Path is: $PATH"
```

## Architecture

### Tokenization Process
1. **Input Processing**: Raw input is modified to add spaces around operators
2. **Tokenization**: Input is split into tokens with type identification
3. **Expansion**: Environment variables and special parameters are expanded
4. **Quote Removal**: Quotes are processed and removed while preserving their effects

### Execution Flow
1. **Syntax Validation**: Check for syntax errors in pipes and redirections
2. **Command Resolution**: Locate executables in PATH or use built-ins
3. **Fork Management**: Create child processes for command execution
4. **Redirection Setup**: Configure file descriptors for I/O redirection
5. **Pipe Handling**: Set up inter-process communication for piped commands
6. **Wait and Cleanup**: Parent waits for children and cleans up resources

### Token Types
- `BUILTIN` - Built-in shell commands
- `COMMAND` - External executable commands
- `ARG` - Command arguments
- `PIPE` - Pipe operator
- `FLAG` - Command flags (e.g., -n)
- `RED_IN` - Input redirection
- `RED_OUT` - Output redirection
- `HEREDOC` - Here document
- `APPEND` - Append redirection

## Project Structure

```
minishell/
├── includes/
│   ├── minishell.h     # Main header file
│   └── token.h          # Token structure definitions
├── src/
│   ├── main.c           # Entry point and main loop
│   ├── init.c           # Initialization routines
│   ├── signals.c        # Signal handling
│   ├── execution/       # Command execution logic
│   │   ├── execution.c
│   │   ├── built_ins.c
│   │   ├── redirections.c
│   │   └── ...
│   └── parse/           # Parsing and tokenization
│       ├── tokenizer.c
│       ├── syntax.c
│       ├── expand_env.c
│       └── ...
├── libft/               # Custom C library
├── Makefile
└── README.md
```

## Testing

### Basic Functionality Tests
```bash
# Test echo
echo "Hello World"
echo -n "No newline"

# Test environment variables
export TEST=value
echo $TEST
unset TEST

# Test pipes
ls | wc -l
cat Makefile | grep "gcc" | wc -l

# Test redirections
echo "test" > file.txt
cat < file.txt
echo "append" >> file.txt
```

### Edge Cases
```bash
# Empty command
[just press enter]

# Multiple spaces
echo     "spaced     out"

# Mixed quotes
echo "Hello '$USER'"
echo 'Single quotes $USER'

# Multiple redirections
< input.txt cat > output.txt
```

## Known Limitations

- Does not support command substitution (`$(command)` or backticks)
- No support for logical operators (`&&`, `||`)
- No wildcard expansion (`*`, `?`)
- No background processes (`&`)
- Limited to basic signal handling
- No job control
- No command aliases
- No shell scripting capabilities

## Development Notes

### Compilation Flags
The project is compiled with strict flags:
- `-Wall` - Enable all common warnings
- `-Wextra` - Enable extra warning flags
- `-Werror` - Treat warnings as errors

### Memory Management
- Comprehensive memory cleanup on exit
- Proper handling of malloc failures
- No memory leaks in normal operation

## Authors

**Felipe Dessoy**
- 42 Email: fdessoy-@student.hive.fi
- 42 Profile: [https://profile.intra.42.fr/users/fdessoy-](https://profile.intra.42.fr/users/fdessoy-)

**Leo Storey**
- 42 Email: lstorey@student.hive.fi
- 42 Profile: [https://profile.intra.42.fr/users/lstorey](https://profile.intra.42.fr/users/lstorey)

## Project Status

**Status**: Complete  
**Score**: 100/100

## License

This project is part of the 42 School curriculum. Usage and distribution are subject to the school's academic policies.
