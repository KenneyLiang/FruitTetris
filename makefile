all:
	g++ FruitTetris.cpp grid.cpp -o FruitTetris -lGL -lGLU -lglut -lm

clean: 
		rm -rf *o FruitTetris


	