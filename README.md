
## About

This project is part of the Tennessee Technological University CSC-2770 Assignment 4. It is written in C++. The purpose of this program is to be an approximation of the edlin text editor.

Its features are:
- `l` - list lines stored in buffer
- `r <filename>` - read lines into buffer from file in current directory
- `s <filename>` - store current buffer of lines into file
- `e <line number> <text>`- replace text on specified line number (stars at 0) 
- `! < line number> <command>` - pipes the line into a bash command
- `q` - quits the program


## Usage

You can use the following Linux command to compile and run the program:

- `make` 
- `make run`

An `edlin>` prompt will appear, allowing you to begin using the program.

## Requirements 

You will need the following installed in order for `compile.sh` to run properly:
- gcc compiler 
