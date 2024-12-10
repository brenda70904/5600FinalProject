# Programming Project: File System
Chia-Kuan Huang, Brenda Jow, Isabelle Li  
CS5600 - Computer Systems   
07DEC2024

## Project Overview
The following project is an implementation of a very simple file system (VSFS) over a simulated disk, represented by a large Linux file. Here, each file is represented by an array of blocks of size 128B. 

Based on the provided file skeleton, the project implements the following system commands in the file system layer, (FileSys.cpp):

1. mkdir directory
2. ls
3. cd directory
4. home
5. rmdir directory
6. create filename
7. append filename data
8. stat name
9. cat filename
10. tail filename
11. rm filename

The project takes advantage of the provided block types -- superblock, directories, inodes, and data blocks -- to implement the above shell commands.

When running the program after compilation there are two options: interactive mode and script mode. These are detailed below:

1. Interactive Mode: this allows the user to directly enter shell commands in the terminal. When the user wants to terminate the program, they can simply enter *quit*.
2. Script Mode: this allows the user to enter commands in a .txt file, and directly run the program with the name of the .txt file as an argument. The program will then execute the commands detailed in the script file.

It is important to note that the program checks for any invalid commands and conditions, such as entering an invalid file name or warning the user when the disk is full. In these cases, the program will output an error message, but will not terminate the program.

Furthermore, the results of the program is persistent across runs. If a clean disk is desired, simply run delete the *DISK* file and compile the program again.


## Compiling Instructions
1. Copy FileSys.cpp and FileSys.h files to the same directory as the other CS5600 file skeleton files. 
2. On the terminal, compile the program using the *make* command.
3. Run the program
   1. Interactive Mode:
      1. To start the program, enter *./filesys*.
      2. Enter commands into the program.
      3. To quit, the program, enter *quit*.
   2. Script Mode
      1. Ensure the script is in the same folder as the other project files.
      2. To run the program, enter *./filesys - s &lt;scriptname&gt;*.
4. The results of the program is persistent across runs. If at any time a clean file system is needed, delete the DISK and resume from step 2.
