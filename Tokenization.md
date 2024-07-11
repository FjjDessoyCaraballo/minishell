Tokenization is the process of breaking down a string of text into smaller components called tokens.
in the project Minishell the tokenization will take each command line input into blocks of valuable information that can be used for parsing and execution.

so far the tokenization stage feels important as it is the first part of any command and string goes through the Minishell it transforms a string of text into a structured format that can be easily manipulated used and accessed by everyone in this project.
it ensures that each command, argument and operator is identified correctly

## Process

- Reading input: reads the command line input as a single string.
- Identifying tokens: the input string is analyzed to find arguments, operators and commands that are separated by delimiters.
- Generating token list: The identified tokens are stored in a linked list of structures in the data structure.

![image](https://github.com/Torutu/minishell/assets/120149333/4840c540-a452-40ce-afef-1800a256e8be)



<!--- ![[Visualized.canvas]]
