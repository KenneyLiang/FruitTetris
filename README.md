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
* Remove line 1 in FruitTetris.cpp and grid.cpp. This line, #include<windows.h>, will not work in Linux.
* Simply type 'make' in the terminal to compile



## How to setup OpenGL and Freeglut in Linux

If you don't have Freeglut installed in Linux, you can run the following command in the terminal to get Freeglut:

```
sudo apt -get install freeglut3 -dev

```



## How to setup OpenGL and Freeglut in Windows 10 with Visual Studio
* Step 1: [Download and Install Visual Studio](https://visualstudio.microsoft.com/downloads/)
	1. Choose 'Desktop development with C++' in Worloads when installing

* Step 2: [Download OpenGL library](https://www.opengl.org/resources/libraries/glut/glut_downloads.php)
	1. Scroll to 'GLUT for Microsoft Windows 9X, ME, 2000, NT & XP users' section
	2. Click and download 'glutdlls37beta.zip' file

* Step 3: Environment Setup
	* Part 1
		1. Open glutdlls37beta.zip
		2. Go to 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\VS\lib\x64' 
		3. Copy 'glut.lib' and 'glut32.lib' from 'glutdlls37beta.zip' and paste them into this directory
		4. Go to 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\VS\lib\x86'
		5. Copy 'glut.lib' and 'glut32.lib' from 'glutdlls37beta.zip' and paste them into this directory
		6. Go to 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\VS\include' 
		7. Copy 'glut.h' and 'glut.dll' from 'glutdlls37beta.zip' and paste them into this directory
		8. Create a new folder name 'gl' in 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\VS\include'
		9. Copy 'glut.h' and 'glut.dll' from 'glutdlls37beta.zip' and paste them into the new folder
	* Part 2
		1. Go to 'C:\Windows\System32'
		2. Copy 'glut32.dll' from 'glutdlls37beta.zip' and paste it into this directory
		3. Go to 'C:\Windows\SysWOW64'
		4. Copy 'glut32.dll' from 'glutdlls37beta.zip' and paste it into this directory

* Step 6: Run Visual Studio