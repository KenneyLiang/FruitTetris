# FruitTetris

FruitTetris is a simple interative game that combines the features of Tetris and Falling Fruits.


The different colours correspnds to differernt fruits:
* Purple = Grapes
* Red = Apples
* Yellow = Bananas
* Green = Pears
* Orange = Orange


FruitTetris is implemented using OpenGL and GLUT. 




## Table of contents

* [Game Controls](https://github.com/KenneyLiang/FruitTetris#game-controls)
* [Compiling in Windows 10 using Visual Studio](https://github.com/KenneyLiang/FruitTetris#how-to-compile)
* [Compiling in Linux]()
* [How to setup OpenGL and Freeglut in Linux](https://github.com/KenneyLiang/FruitTetris#how-to-setup-opengl-and-freeglut-in-linux)
* [How to setup OpenGL and Freeglut in Windows 10 with Visual Studio](https://github.com/KenneyLiang/FruitTetris#how-to-setup-opengl-and-freeglut-in-windows-10-with-visual-studio)



## Game Logics

A random Tetris tile consisting of four fruits will descend from the top of the grid. During this descent, the player can move the pieces laterally and rotate them until they have landed on the bottom of the grid or on a piece that had been placed before it. The objective of the game is to use the pieces to create as many horizontal lines of blocks as possible. Each filled horizontal lines will disappear. When three of the same fruit are consecutive in a row or column, they will disappear and the fruis from above will fall down 1 row for each tiles that disappeared. 

The game will termiante if the pieces reached to the top and prevents the arrivial of additional pieces. 



## Game Controls

* :arrow_up:	Rotate the tile counterclockwise 
* :arrow_down:	Accelerates the downward movement
* :arrow_left:	Move the tile 1 position to the left 
* :arrow_right:	Move the tile 1 position to the right
* 'Q' to quit
* 'R' to restart


## Note Before Compiling

If you are compiling in Linux:
* Remove line 1 (#include<windows.h>) in FruitTetris.cpp and grid.cpp
* Simply type 'make' in the terminal



## How to setup OpenGL and Freeglut in Linux

If you don't have Freeglut installed in Linux, you can run the following command in the terminal to get Freeglut:

'''
sudo apt -get install freeglut3 -dev

'''


## How to setup OpenGL and Freeglut in Windows 10 with Visual Studio
