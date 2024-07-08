# minishell

## Description:
This project is meant to replicate some basic core parts of bash shell.

## Usage:
1. Initialize the program with ```./minishell```
2. Utilize the shell as you would use a proper shell

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

## DA PLAN:
1. Parse the line given to readline() (Bruno and Leo?)
	- Turn everything into lowercase
 	- No whites
  	- No special characters
3. Tokenization (Waleed)
4. Built-ins (Felipe)
	- echo (with -n) DONE
 	- cd DONE
	- pwd DONE
 	- export 1/2 DONE
  	- unset
   	- env DONE
   	- exit DONE
6. Piping (Felipe)
7. Handle $? (Felipe)
8. CTRL-C + CTRL-D + CTRL-\ (Bruno or Leo)
9. Environment variables ($ expands values) (everyone at certain point)
