# XJCO 1921 Coursework 2 - Map Navigation Code

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
/test_data/: Store all map files that need to be tested.

```
However, you should test your program on other data including test cases for error handling. 

## Testing script

The messages and return codes which you should use are:

| Value  | String | Condition |
| ------------- | ------------- | ------------- |
| 1 | ERROR: EXIT_WRONG_ARG_COUNT | Program given wrong # of arguments |
| 2 | ERROR: EXIT_BAD_FILE_NAME | Program fails to open file |
| 3 | ERROR: EXIT_MALLOC_FAILED | Malloc failed to allocate memory |
| 4 | ERROR: EXIT_BAD_DATA | Program read in illegitimate data |
| 5 | ERROR: EXIT_EXCEED_RANGE | Beyond the map area |
| 6 | ERROR: EXIT_NO_PATH_FOUND | Can't find the right path |
| 7 | ERROR: EXIT_SDL_FAILED | Failed to load sdl library|
| 8 | ERROR: EXIT_NORMATIVE_MAP | Do not meet map specifications |
| 9 | ERROR: EXIT_OUTPUT_FAILED | Writing out data failed |

All other print statements will be ignored by the final test script.

You should also use these standard success messages, for which the value is always 0.

Usage messages are also a Unix convention - when an executable is run with no arguments, it is assumed that the user does not know the correct syntax and wishes to see a prompt. As this is not an error state, the return value is 0.
