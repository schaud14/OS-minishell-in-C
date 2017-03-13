CS550
Operating Systems:
Assignment 1:

Group Members:
1.Sagar Kale
email id: skale4@binghamton.edu

2.Saurabh Chaudhari
email id: schaud14@binghamton.edu

Instrutions to Extracr and navigate:
tar -xvf p1-skale4-schaud14.tar

use : cd p1-skale4-schaud14   .. to neviate to program folder.


Instrution to Compile and Run:
To Compile use : gcc minishell.c -o minishell

To Run use : ./minishell

1. This program can run all basic commands provided by "/bin" and "/usr/bin" libraries with one or more arguments if required.

2. Any process appended by "&" at the end can run in background mode. Multiple commands can run in background at same time.

3. The list of background or stopped processes can be viewed by "jobs" command.

4. The stopped or background process can be brought back to foreground by "fg %N" command, where N is job id which obtained by jobs command.

5. Input to any command can be taken from a file saved in same directory as the program by using "<", eg "command_name < Input_file_name"

6. Output of any command can be saved to file instead of stdout which will be saved in same directory as the program by using ">" eg . command_name < output_file_name

7. Any process running in foreground can be killed by pressing Ctrl+C.

8. Any process running in foreground can be stopped temporarily by pressing Ctrl+Z. The entry will be made in jobs table and will be tagged with status as stopped. This can be viewed by "jobs" command.

9. Any process in background can be killed by using either "kill process_id" or "kill %job_id".

10 .Process running is background/stopped can be brought to foreground by using "fg %job_id".

11. "pwd" will provide the current working directory of shell.

12. To change directory cd command can be used. "cd .." will change directory to parent directory. "cd folder_name" will navigate to folder_name if exist in current directory. 
"cd /path/" will navigate to path as mentioned in argument followed by "/".

13. Using filter we can create a pipe to provide IPC. eg "ps -ef | grep minishell" Here output of "ps -ef" will be redirected to "grep minishell" as input and resulting output will be shown to stdout.

14 .The program can handle the chain of filter as mentioned above. The output of a process will be redirected to next process as input till last commmand is executed in the chain of filters.

15. This program can run the output redirection command like in point 6 above, in background mode if whole command is followed by & at the end.

16. This program can run the multiple chain of filters as mentioned in point 14 above, in background mode if whole command is followed by & at the end.

17. Signal to parent process is also delivered to all its child processes and same action is taken for them as well.
eg. Kill for parent process will kill its childs as well.
