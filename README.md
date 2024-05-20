# minishell

## Description:
This project is meant to replicate some basic core parts of bash shell.

## Usage:
1. Initialize the program with ```./minishell```;
2. Utilize the shell as you would use a proper shell.

## Result:
- Status: <span style="color:red">Incomplete</span>
- Result: <span style="color:red">0%</span>

## Contact: 
- 42-email: fdessoy-@student.hive.fi
- 42-profile: [https://profile.intra.42.fr/users/fdessoy-](https://profile.intra.42.fr/users/fdessoy-)

## Instructions for branching
```1. git clone repo
2. git checkout -b 'BranchName'
3. git status
4. git add (files)
5. git commit -m "comment"
6. git pull origin main
7. git checkout main
8. git pull
9. git merge BranchName
10. git push
11. git checkout BranchName
```

## TO DO (aka DA PLAN):
1. Parse the line given to readline()
	1.1. Turn everything into lowercase;
	1.2. No whites;
	1.3. No special characters;
2. Tokenization;
3. Built-ins
	3.1. echo (with -n) DONE;
	3.2. cd;
	3.3. pwd DONE;
	3.4. export;
	3.5. unset;
	3.6. env DONE;
	3.7. exit DONE;
4. Piping;
5. Handle $?;
6. CTRL-C + CTRL-D + CTRL-\;
7. Environment variables ($ expands values);

## Capn' Log:
```
Week 1 (week 20 of 2024)
13.05.2024

1. Made the Makefile. This specific Makefile had to be done differently from other that we previously did, because the readline() package had to be installed using brew.
2. Made the headerfile minishell.h. Added some structs that will definitely change throughout the project.
3. Currently the project is/was able to initialize the readline() without any extra functionalities.
4. In total, four files have been added: init.c, exit_handler.c, line_handler.c, and main.c. Project had to come a halt because copying the env into a linked list became a bit troublesome.

14.05.2024
1. Extracted envp into a linked list, now all elements of the envp are accessible and easily managed for the future while still mantaining the original envp.
2. Extracted the binaries folders path to a variable in the t_data struct into the char pointer bin. This will be useful later when looking for executable binaries and trying to pipe them.

15.05.2024
1. Added built-ins: env and pwd. Right now the lines are not being parsed, so there cannot be spaces before the instructions. There is urgent need to parse the line extracted by readline().
2. ENV is properly printing the envp into the STDOUT. During the parsing it might be neccessary to parse out empty spaces.
3. PWD is being printed into the STDOUT. During the parsing it might be neccessary to parse out the empty spaces.

17.05.2024
1. Added built-ins: exit. It frees the linked list and exits. Functions responsible for it is called get_the_hell_out() in built_ins.c.
2. 
```