# XJCO 1921 Coursework 2
# Map Navigation Code

This repository contains code for a map navigation application. The code is written in C and uses the SDL (Simple DirectMedia Layer) library for graphics and user interface components. The application allows users to create and visualize maps, read map data from files, plan routes, and display them on the screen.

## Prerequisites
Before compiling and running the code, make sure you have the following prerequisites installed on your system:
- GCC (GNU Compiler Collection)
- SDL 2 library
- SDL 2 TTF library

## Compilation
To compile the code, use the following command:

```
gcc createMap.o readFile.o routePlan.o plotSDL.o dijkstra.o optionSDL.o shortSDL.o -o map1 `sdl2-config --libs` -lSDL2_ttf
```

This command links several object files together to create the executable `map1`. The object files are:
- `createMap.o`: Contains functions for creating maps.
- `readFile.o`: Handles reading map data from files.
- `routePlan.o`: Implements route planning algorithms.
- `plotSDL.o`: Displays the map and routes using SDL.
- `dijkstra.o`: Implements the Dijkstra algorithm for route planning.
- `optionSDL.o`: Provides SDL user interface functionality.
- `shortSDL.o`: Manages the display of the shortest route.

The command also includes the necessary flags and libraries for SDL 2 and SDL 2 TTF.

## Usage
After successfully compiling the code, you can run the `map1` executable to launch the map navigation application. The application provides a graphical user interface (GUI) where you can interact with the map and perform various actions, such as creating a map, loading map data, planning routes, and displaying them on the screen.
Users can select the shortest path, shortest time, pass through the given point and change the map properties in the menu page. When selecting points there are two ways of input, keyboard input the node id or mouse click on the node in the map. When using this map program, the user only needs to follow the menu instructions on the map for mode selection.

# Using The Files

You can compile the provided files by using:
```
make all
```
Delete all compiled files
```
make clean
```
Executables can then be run the standard way.

You run the test suite using:
```
bash ./test.sh
```

## Linux and Development Environment
Your code must compile and run on a standard Linux Distribution (e.g., Ubuntu)

If you're using a Windows operating system, you can still achieve this by installing the Windows Subsystem for Linux (WSL) and selecting Ubuntu as your Linux distribution.

To install WSL on Windows 10/11, follow the instructions provided at: https://learn.microsoft.com/zh-cn/windows/wsl/install. Once you have WSL installed, you can install the necessary development tools, such as gcc, gdb, and make, on Ubuntu by running the following commands in your Ubuntu terminal:

```
sudo apt update
sudo apt upgrade

sudo apt-get install -y build-essential
```

These commands will update the Ubuntu package list and install the build-essential package, which includes the development tools you'll need to compile and run your code on the Ubuntu Linux distribution.

## Acknowledgements
This project was made possible by the SDL library and its contributors. We would like to express our gratitude for their efforts in developing and maintaining this fantastic library.

If you encounter any issues or have suggestions for improvements, please feel free to submit an issue or pull request. We appreciate your feedback!


## Test data

We have provide some test data for you to start with. These PGM files are in the test_data/pgms directory, which has the following structure:
```
test_data/pgms/ascii: containing some input PGM in the ASCII form. These include some corrupted PGM files to test error handling.
test_data/pgms/binary: containing some binary input PGM
test_data/pgms/output: containing the expected outputs for tasks I and III when applying your developed program to the given input images.

```
However, you should test your program on other data including test cases for error handling. 

## Testing script

The messages and return codes which you should use are:


| Value  | String | Condition |
| ------------- | ------------- | ------------- |
| 1  | ERROR: Bad Argument Count | Program given wrong # of arguments |
| 2 | ERROR: Bad File Name (fname) | Program fails to open file |
| 3 | ERROR: Bad Magic Number (fname) | File has an illegal magic number |
| 4 | ERROR: Bad Comment Line (fname) | Program failed on comment line |
| 5 | ERROR: Bad Dimensions (fname) | File has illegal dimensions |
| 6 | ERROR: Bad Max Gray Value (fname) | Program failed on max gray value |
| 7 | ERROR: Image Malloc Failed | Malloc failed to allocate memory |
| 8 | ERROR: Bad Data (fname) | Reading in data failed |
| 9 | ERROR: Output Failed (fname) | Writing out data failed |
| 100 | ERROR: Miscellaneous (text description) | Any other error which is detected |

In all cases, fname should be replaced by the name of the file which caused the error.
In the case of Miscellaneous errors, in 100 chars or less please describe what the error was.

All other print statements will be ignored by the final test script.

You should also use these standard success messages, for which the value is always 0.

| Value | String | Program | Meaning |
| ------------- | ------------- | ------------- | ------------- |
| 0 | ECHOED | pgmEcho | Program echoed the input |
| 0 | IDENTICAL | pgmComp | The two files were logically identical |
| 0 | DIFFERENT | pgmComp | The two files were not logically identical |
| 0 | CONVERTED | pgma2b/pgmb2a | The file was converted |
| 0 | REDUCED | pgmReduce| The file was successfully reduced in size |

Usage messages are also a Unix convention - when an executable is run with no arguments, it is assumed that the user does not know the correct syntax and wishes to see a prompt. As this is not an error state, the return value is 0.
