# Windows Subsystem for Linux (WSL)

- Type in Windows Terminal :

  `wsl --install`

- Restart PC

- Setup username, password for Ubuntu Subsystem

- In Visual Studio Code download : <br>
  `WSL Extension`

- To open WSL in VSCode : <br>
 `Cntrl + Shift + P` <br>
 `New WSL Window`

- To open bash Terminal on Windows type in Windows Terminal: <br>
 `ubuntu`
- /mnt/c/Users (Ubuntu) == c:\Users (Windows)

## Setup *compilers* and *libraries* for WSL
### Open bash Terminal on WSL and type the following commands :
- C : <br>
 `sudo apt-get install gcc`

- Java : <br>
 `sudo apt-get install openjdk-6-jre`

- Cpp : <br>
 `sudo apt install gcc g++ clang gdb`

- Ncurses : <br>
 `sudo apt-get install libncurses5-dev libncursesw5-dev`
 
- MPI : <br>
 `sudo apt install mpicc`
