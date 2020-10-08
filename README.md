# Game-Tree
0 sum for Connect-4. There is an app that provides all the winning situations for one of two players, depending on the tree level we are at a particular moment. First-year of my bachelor degree studies, data structures course's second assignment.

# Additional files
A README file, written in Romanian, which compresses implementation details, a Makefile file that includes three rules build, and clean. The build rule creates the executabe, and the clean rule deletes the executable, and one header file, "tema2.h", that includes the structures and the functions headers that I used in my main program.

# How to use the Makefile file/ generate the game tree

Given the current Makefile file, you will run the following commands:

```sh
$ make build
```
From now on, the app must receive a text file which contains the current game state (the current player and the gameboard's details). It can be launched like this:

```sh
$ ./minimax file.txt
```
After generating all winning situations, you can close the app and use the clean rule:

```sh
$ make clean
```
