# ATAK SHELL
A very simple command-line shell written in C. This project is currently still in development, with the eventual goal
of making it a not so simple shell. 

Currently, only executable commands found in `/usr/bin` can be executed, assuming the system call `execvp` can handle them (e.g. `ls`, `cat`, `df`, `man`, `diff` etc.).

# Future plans
I plan to make this a fully functioning shell completly written in C, for the purpose of learning. In which I hope to include the use of tab completion, history regex, more commands (e.g `cd`, `fg`, `&` etc), user login/logout, separating ATAK from the current shell it runs on top of (i.e. bash/zsh), and much more!

# Compilation
To compile the project you can run:

`make all` or `make`

To start ATAK, run: `make run`
