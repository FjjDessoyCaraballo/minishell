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
ALWAYS remember to pull from main to have the latest version of minishell before continuing your work. At the same time, always remember to only push to main directory when the program is properly working. AVOID PUSHING FAULTY PROGRAM AT ALL COSTS!

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

20.05.2024
1. Fixed the echo command with the no newline flag (-n);
2. Added a TO DO list in the README;
3. We have started to learn about branching. We created two different branches to work independently in each branch. Felipe's branch is THICC_BRANCH and Leo's branch is Ginger_branch.
4. Started working on the command 'cd' and it is working in the basic form: when you input just 'cd', it takes you to the $HOME/home (data->home_pwd). Next step is to update env so PWD will give the actual position and then work to go up and down directories;

21.05.2024
1. The plan for today is to deliver at least the cd command fully working. The biggest challenge might be making the "cd .." command work;
2. The solution to "cd .." might rely on concatenation of the current path and deduction the current position and giving back the path until the last (second to last?) backslash;
3. At the end of the day, cd was unreliable and had to start from scratch;

22.05.2024
1. Upon closer inspection to the behavior of the CD built-in, it is not susceptible to changes while unsetting variables from ENV; Therefore, for architectural, integrity, and replication reasons, pwd is being initialized into a variable in the data struct. Check for perm_pwd for a pwd that should be always available;
2. Apparently, the envp can be unset and should still work with the basic builtins. this may call for two parallel envp's when piping;
3. CD is working with arguments, but now the sole CD is not working;
4. CD is fully working. The problem originated from logical problem with the structure of the function, as it was calling chdir and later calling it again to same position as it was, making the first chdir() redundant;
5. CD is segfaulting randomly, which probably means that there is a problem with some pointer;

23.05.2024
1. Fixed exit and not it takes an integer to represent exit status number. For example, if user inputs 'exit 1', when the user later inputs '$?' it will get that exit number;
```