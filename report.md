# UNIX SHELL REPORT

## Table of Contents
1. [Introduction](#introduction)
2. [Solution Overview](#solution-overview)
3. [Program Execution Screenshot](#program-execution-screenshot)
4. [Conclusion](#conclusion)
5. [Appendix: Source Code](#appendix-source-code)


### Introduction
This project is about implementing our own shell for the UNIX OS, using a step by step guide from 'https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/processes-shell'

### Solution Overview
Before beginnning the project, I researched every term used in the guide and familiarised myself with the workings of each 'function call' typically used in a shell. I then began by implementing a simple version to test and observe the output of these terms. After which I then implemented a class structure to encapsulate the vital functionality of the shell.

During the developement of the shell, I ran into bugs such as separating the cmds and arguments properly. Here is an example of one bug: `ls cannot find a directory with '|'` . the pipeline syntax `|` was being passed as an argument of `ls`. So I fixed it by removing the last element of the `args1` array in my `execute_pipeline_internal` function and this helped fix the bug.

I made sure that my code was modularised and had proper error handling, so that debugging would be easy to do.

### Program Execution Screenshot
![alt text](<Screenshot From 2025-04-17 14-24-46.png>)
![alt text](<Screenshot From 2025-04-17 14-25-08.png>)
![alt text](<Screenshot From 2025-04-17 14-25-32.png>)
![alt text](<Screenshot From 2025-04-17 14-27-10.png>)

### Conculsion
I learnt alot about processes and system calls that helped with implementing complex features like pipes. As well how to apply background processes. Another new thing I learnt was about file duping `dup2`, this helps with duplicating a file descriptor(the kernel files) which then can be used in shell commands such as piping `|` and redirection `<`.

Overall, finding out how a shell that many sys admins or advent linux users parse the commands that do vital jobs and to code it on my own is an experience worth having

### Appendix
https://github.com/H8K56/UNIX_SHELL