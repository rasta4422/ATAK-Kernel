# ATAK SHELL
A very simple command-line shell written in C. This project is currently still in development, with the eventual goal
of making it a not so simple shell. 

Currently, only executable commands found in `/usr/bin` can be executed, assuming the system call `execvp` can handle them (e.g. `ls`, `cat`, `df`, `man`, `diff` etc.).

# Future plans
I plan to make this a fully functioning shell completly written in C, for the purpose of learning. In which I hope to include the use of tab completion, history regex, more commands (e.g `cd`, `fg`, `&` etc), user login/logout, separating ATAK from the current shell it runs on top of (i.e. bash/zsh) using a simple kernel, and much more!

# Dependancies
    1. grub (For running grub-mkrescue)
    2. ld (The GNU linker)
    3. nasm
    4. qemu (with x86 support)

# Compilation


To compile the shell you can run:
```
make shell
```

To start ATAK, run: 
```
make ATAK
```

NOTE: The kernel is still in development so it does not work yet.
To compile the entire project (kernel and shell):
```
make all
```
This will compile the shell, threading library and kernel


To generate and run the kernel iso file in qemu:
```
make qemu
```
Currently the kernel will have a boot option in grub, but will not properly start as I haven't added the functionality for a 64-bit kernel.
