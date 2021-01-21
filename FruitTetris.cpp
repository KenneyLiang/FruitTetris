#include<windows.h>     //Delete if being compiling in Linux
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<GL/gl.h>
#include<GL/glut.h> 
#include<math.h>
#include "grid.h"
#include <cassert>      // for assertion 
#include <cstring>      //for memeset and memecpy

#define FPS 3           // Frames per Second
#define COLUMNS 10      //grid size
#define ROWS 20
bool UP = false;
bool DOWN = false;
bool pause = false;


int rotation = 0;
int piece_index = 0;
int posX = 0, posY = 0; 


struct Tetromino{
	int *value;         //colour of the pieces
	int size;           //size of the pieces

};

struct Piece{
    Tetromino tetromino;
	int index;          // the shape of the tetromino 
	int offset_row;     //left/right
	int offset_col;     //down
	int rotation;       
};

struct Game{
    Piece piece;
    int board[ROWS*COLUMNS];     //the game space
    int lines[ROWS];            //lines that are fill will have a value of 1         
    int cleared_lines;          //lines cleared
    int pending_clear_fruits[ROWS* COLUMNS];    //consecutive fruits in a row to be cleared
    int pending_clear_fruits_vertical[ROWS* COLUMNS];   //consecutive fruits in a column to be cleared
    
};


Tetromino tetromino(int *value, int size){      // constructor function
	return {value, size};
}


int TETROMINO_1[]={         //I shape in horizontal
	0, 0, 0, 0,
    1, 1, 2, 2,
    0, 0, 0, 0,
	0, 0, 0, 0
};

int TETROMINO_2[] = {       //right z shape
	0, 0, 0,
    0, 2, 2,
    2, 2, 0
};
int TETROMINO_3[] = {       //left z shape
	0, 0, 0,
    3, 3, 0,
    0, 3, 3
};

int TETROMINO_4[] = {       // T shape
	0, 0, 0,
	4, 4, 4,
	0, 4, 0
};

int TETROMINO_5[] = {       // right L shape
	0, 0, 0, 
	5, 5, 5, 
	5, 0, 0,
};

int TETROMINO_6[] = {       // left L shape
	0, 0, 0, 
	6, 6, 6, 
	0, 0, 6,
};

Tetromino TETRINOS[] = {    // list containing all tetrominos
	tetromino(TETROMINO_1,4),
	tetromino(TETROMINO_2,3),
	tetromino(TETROMINO_3,3),
    tetromino(TETROMINO_4,3),
    tetromino(TETROMINO_5,3),
    tetromino(TETROMINO_6,3)
};

//all the functions for call back

int random_values(int);
int random_tetriminos(int);
void set_matrix_value(int *, int , int , int , int );
int get_matrix_value(int*, int, int , int);
int get_tetromino_rotation_value(Tetromino, int, int, int);
void draw_board(int*, int, int, int, int);
int check_termination_row_filled(int *, int , int );
int find_filled_lines(int *, int , int , int *);
void clear_filled_lines(int *, int , int , int *);
bool validate_piece( Piece *, int *,int , int );
void stack_piece(Game* );
void spawn_piece(Game* );
void draw_square(int , int , int , int , int );
void draw_piece(Piece *, int, int);
void dropdown(Game* );
bool game_over(int *, int , int );
int find_clearing_fruits_horizontal(Game *, int *, int , int );
void clear_fruits( int *, int , int , int *);
int find_clearing_fruits_vertical(Game *, int *, int , int );
void clear_vertical_fruits(int *, int , int , int *);
void update_game(Game *);
void restart(Game *);





int random_values(int max_value){
    int x =rand() %(max_value) + 1;
    return x;
}

int random_tetriminos(int number_of_tetriminos){
    int x = rand() %(number_of_tetriminos);
    return x;
}


void set_matrix_value(int *array_matrix, int width, int row, int col, int value){
    int index = row * width + col;
    array_matrix[index] = value;
}


int get_matrix_value(int *array_matrix, int width, int row, int col){
    int index = row * width + col;
    return array_matrix[index];
}


int get_tetromino_rotation_value(Tetromino *tetromino, int row, int col, int rotation){
    int size = tetromino-> size;
    switch (rotation)
    {
    case 0:
        return tetromino-> value[row*size + col];

    case 1:
        return tetromino-> value[(size-col-1)* size + row];

    case 2:
        return tetromino-> value[(size-row-1) * size + (size - col-1)];
    case 3:
        return tetromino-> value[col*size + (size-row-1)];

    default:
        break;
    }
    return 0 ;      //error case
}

int check_termination_row_filled(int *array_matrix, int width, int row){

    for(int col = 0; col<width; col++){
        if(get_matrix_value(array_matrix, width, row,col)== 0){
            return 0;
        }
    }

    return 1;

}



int find_filled_lines(int *array_matrix, int width, int height, int *lines_to_be_cleared){


    int count  = 0;
    for (int row = 0; row < height; row++){
        int filled = check_termination_row_filled(array_matrix, width, row);

        lines_to_be_cleared[row] = filled;
        count = count + filled;
    }
    return count;

}



void clear_filled_lines(int *array_matrix, int width, int height, int *lines_to_be_cleared){

    int source_row = height -1;

    for(int row = height -1; row >=0 ; row--){

        while(source_row >= 0 && lines_to_be_cleared[source_row] != 0){

            --source_row;
        }

        if ( source_row < 0){

            memset(array_matrix + row *width, 0, width*4); // empty row

        }
        else{
            memcpy(array_matrix+row*width, array_matrix +source_row*width, width*4);
            --source_row;
        }

    }

}







bool validate_piece( Piece *piece, int *board,int width, int height){
    Tetromino *tetromino = TETRINOS + piece-> index;
    
    assert(tetromino);      //if it's true, do nothing, else program terminates

    for (int row = 0; row < tetromino->size; row++)
    {
        for (int col = 0; col < tetromino->size; col++ ){
            int value = get_tetromino_rotation_value( tetromino, row, col, piece->rotation);

            if(value > 0){      // if there is a piece
                int board_row = piece->offset_row + row;
                int board_col = piece->offset_col + col;

                if (board_row >= height){
                    return false;
                }
                if (board_col < 0){
                    posX++;
                    piece->offset_col = posX;
                    return false;
                }
                if (board_col >= width){
                    posX--;
                    piece->offset_col = posX;
                    return false;
                }
                if ( get_matrix_value(board, width,board_row,board_col) > 0){       // has a piece in that position 
                    return false;               //collision detection
                }


            }

        }
        
    }
    return true;    //valid
}

void stack_piece(Game* game){

    Tetromino* tetromino = TETRINOS + game->piece.index;

    for (int row = 0; row < tetromino->size; row++)
    {
        for(int col = 0; col < tetromino->size; col++){

            int value = get_tetromino_rotation_value(tetromino, row, col, game->piece.rotation);

            if(value > 0){

                int board_row = game->piece.offset_row + row;
                int board_col = game->piece.offset_col + col;

                set_matrix_value(game->board,COLUMNS, board_row, board_col,value);
            }


        }
    }
    

}

void spawn_piece(Game* game){
    
    // spawning random tetrominos with different colours;

    TETROMINO_1[4] = random_values(5);
    TETROMINO_1[5] = random_values(5);
    TETROMINO_1[6] = random_values(5);
    TETROMINO_1[7] = random_values(5);

    TETROMINO_2[4] = random_values(5);
    TETROMINO_2[5] = random_values(5);
    TETROMINO_2[6] = random_values(5);
    TETROMINO_2[7] = random_values(5);


    TETROMINO_3[3] = random_values(5);
    TETROMINO_3[4] = random_values(5);
    TETROMINO_3[7] = random_values(5);
    TETROMINO_3[8] = random_values(5);

    TETROMINO_4[3] = random_values(5);
    TETROMINO_4[4] = random_values(5);
    TETROMINO_4[5] = random_values(5);
    TETROMINO_4[7] = random_values(5);


    TETROMINO_5[3] = random_values(5);
    TETROMINO_5[4] = random_values(5);
    TETROMINO_5[5] = random_values(5);
    TETROMINO_5[6] = random_values(5);

    TETROMINO_6[3] = random_values(5);
    TETROMINO_6[4] = random_values(5);
    TETROMINO_6[5] = random_values(5);
    TETROMINO_6[8] = random_values(5);


    game->piece = {};
    game->piece.index = random_tetriminos(6);
    game->piece.offset_row = -1;
    game->piece.rotation = rand()%4;   //random rotations 


    posX = rand() % 7;
    posY = 0;
    rotation = 0;
}

void dropdown(Game* game){

    if (DOWN == true){
        game->piece.offset_row++;
        if(validate_piece(&game->piece, game->board, COLUMNS, ROWS) == true){
            game->piece.offset_row++; 
        }
        DOWN = false;
    }

    else{
        ++game->piece.offset_row;

    }
    if(validate_piece(&game->piece, game->board, COLUMNS, ROWS) == false){
        --game->piece.offset_row;
        stack_piece(game);
        spawn_piece(game);      //spawn after the piece has landed
    }
}


bool game_over(int *array_matrix, int width, int row){

    for(int col = 0; col<width; col++){
        if(get_matrix_value(array_matrix, width, row,col) != 0){
            return true;
            break;
        }
    }

    return false;

}


int find_clearing_fruits_horizontal(Game *game, int *array_matrix, int width, int height){

    int count = 0;

    for(int row = 0; row <height; row++){

        for(int col = width - 2; col >=1; col--){

            int previous_value = get_matrix_value(array_matrix, width, row, col-1);
            int current_value= get_matrix_value(array_matrix, width, row, col);
            int next_value = get_matrix_value(array_matrix,width, row, col+1);

            if(previous_value > 0 && current_value > 0 && next_value){
                if(previous_value == current_value && previous_value == next_value && current_value ==  next_value){
                    game->pending_clear_fruits[row*width+col-1] = 1;
                    game->pending_clear_fruits[row*width+col] = 1;
                    game->pending_clear_fruits[row*width+(col+1)] = 1;
                    count ++;
                }
            }


        }

    }

    return count;

}



void clear_fruits( int *array_matrix, int width, int height, int *fruits){

    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){

            if ( fruits[row*width + col] > 0 ){

                memset(array_matrix+row*width+col, 0, sizeof(int));

                for(int i = row; i>=0; i--){

                    if(i >= 1){
                        memcpy(array_matrix + i*width + col, array_matrix + (i-1)* width + col, sizeof(int));
                        memset(array_matrix + (i-1)* width + col, 0, sizeof(int));
                    }
                    else{
                        memset(array_matrix + (i)* width + col, 0, sizeof(int));
                    }

                }
            }
        }
    }

}


int find_clearing_fruits_vertical(Game *game, int *array_matrix, int width, int height){
    
    int count = 0;
    
    for(int row = 1; row < height - 1; row++){
        for(int col = 0; col < width; col++){

            int top_value = get_matrix_value(array_matrix, width, row-1, col);

            int middle_value = get_matrix_value(array_matrix, width, row, col);
            int bottom_value = get_matrix_value(array_matrix, width, row+1, col);

            if(top_value > 0 && middle_value> 0 && bottom_value > 0){

                if(top_value == middle_value && top_value == bottom_value && middle_value ==bottom_value ){

                    //game->pending_clear_fruits_vertical[(row-1)*width + col] = 1;
                    //game->pending_clear_fruits_vertical[(row)*width +col] = 1;
                    game->pending_clear_fruits_vertical[(row+1)*width +col] = 1;
                    count++;

                }

            }

        }


    }

    return count;
}

void clear_vertical_fruits(int *array_matrix, int width, int height, int *fruits){

    for(int row =0; row <= height-1; row++){

        for(int col = 0;col < width; col++){

            if(fruits[row*width + col] > 0){

                //memset(array_matrix + (row)*width + col, 0 , sizeof(int));
                
                for(int i = row; i>=0; i--){
                    if( i >= 3){
                        memcpy(array_matrix + (i)*width + col, array_matrix + (i-3)*width + col, sizeof(int));
                        memset(array_matrix + (i-3)*width + col, 0 , sizeof(int));
                    }
                    else{

                        memset(array_matrix + (i)*width + col, 0 , sizeof(int));
                    }
                }

            }


        }
    }


}






void update_game(Game *game){
    Piece piece = game -> piece;

    piece.offset_col = posX;

    piece.rotation = rotation;


    if (validate_piece (&piece,game->board, COLUMNS, ROWS) == true){
        game->piece = piece;
    }

    if( pause == false){
        dropdown(game);
    }
    draw_board(game->board, COLUMNS, ROWS, 0,0);
    draw_piece(&game->piece,game->piece.offset_col,game->piece.offset_row);
    
    game->cleared_lines = find_filled_lines(game->board,COLUMNS,ROWS,game->lines);
    if (game->cleared_lines > 0){
        clear_filled_lines(game->board, COLUMNS, ROWS, game->lines);
    }

    int game_over_row = 0;

    if ( game_over(game->board, COLUMNS, game_over_row) == true){
        pause = true;
    }



    int number_of_three_same_fruits = find_clearing_fruits_horizontal(game ,game->board, COLUMNS, ROWS);



    if(number_of_three_same_fruits > 0){
        clear_fruits(game->board, COLUMNS, ROWS, game->pending_clear_fruits);
    }


    for(int row = 0; row < ROWS; row++){

        for(int col = 0; col < COLUMNS; col++){

            game->pending_clear_fruits[row*COLUMNS + col] = 0;
        }
    }

   int number_of_three_same_fruits_vertical = find_clearing_fruits_vertical(game ,game->board, COLUMNS, ROWS);
    
    if(number_of_three_same_fruits_vertical >0){
        clear_vertical_fruits(game->board, COLUMNS, ROWS, game->pending_clear_fruits_vertical);
    }

    for(int row = 0; row < ROWS; row++){

        for(int col = 0; col < COLUMNS; col++){

            game->pending_clear_fruits_vertical[row*COLUMNS + col] = 0;
        }
    }


    number_of_three_same_fruits = 0;
    number_of_three_same_fruits_vertical =0;

}

void draw_square(int row, int col, int value, int offset_x, int offset_y){

    glLineWidth(1.0);

    if (value == 1){
        
        glColor3f(1.0, 0.0 ,1.0); //purple
    }
    else if (value == 2) {
        glColor3f(1.0,0.0,0.0); //red
    }
    else if (value == 3) {
        glColor3f(1.0,1.0,0.0); //yellow
    }
    else if (value == 4) {
        glColor3f(0.0,1.0,0.0); //green
    }
    else if (value == 5) {
        glColor3f(1.0,0.5,0.0); //orange
    }


    int x = col + offset_x;
    int y = row + offset_y;

    glRectd(x,y,x+1,y+1);
}


void draw_piece(Piece *piece, int offset_x, int offset_y){

    Tetromino *tetromino = TETRINOS + piece-> index;
    
    for( int row = 0; row < tetromino->size; row++){
        for(int col =0; col < tetromino->size; col++){
            int value = get_tetromino_rotation_value( tetromino, row, col, piece->rotation);
            
            if(value != 0){
                draw_square(row, col,value, offset_x, offset_y);
            }
        }
    }
}

void draw_board(int* board, int width, int height,
                int offset_x, int offset_y){

    for (int row = 0; row < height; row++)
    {           
        for(int col = 0; col < width; col++){

            int value = get_matrix_value(board, width, row, col);

            if ( value != 0){
                draw_square(row, col, value, offset_x, offset_y);
            }
        }
    }



}

void restart(Game *game){
    
    for(int row = 0; row < ROWS; row++){

        for(int col = 0; col < COLUMNS; col++){

            game->board[row*COLUMNS + col] = 0;
            
            spawn_piece(game);
        }
    }
}



void timer(int);
void special (int,int, int);

void Init(){

    glClearColor(0.0, 0.0, 0.0, 1.0);

	initGrid(COLUMNS,ROWS);
	
}

void display();

void reshape(int, int);
void special (int, int, int);
void keyboard(unsigned char, int, int);

Piece piece = {};  //first piece
Game game = {};     //create a game 


int main (int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200,60);
	
	glutInitWindowSize(350,700);

	// Giving name to window 
    glutCreateWindow("Kenneth's Game");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer, 0);
    glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	Init();
	glutMainLoop();

	return 0;
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    update_game(&game);
	glutSwapBuffers();

}

void reshape(int w, int h){
	//viewport
	glViewport(0,0,(GLsizei)w,(GLsizei)h);

	//projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0,COLUMNS+1.0,ROWS+1.0,-2.0);
	glMatrixMode(GL_MODELVIEW);
}


void timer(int){
	glutPostRedisplay();
	glutTimerFunc(1000/FPS, timer, 0);
}


void keyboard(unsigned char key, int x, int y){
	switch (key)
	{
	case 'q':       //exit
        exit(0);
		break;
    case 'r':       //restart
        restart(&game);
        pause = false;
        break;
	}
    glutPostRedisplay();
}


void special(int key, int, int){
	switch (key)
	{
	case GLUT_KEY_UP:
        rotation =(rotation +1 ) % 4;       //rotate piece
		break;
	case GLUT_KEY_DOWN:
        DOWN = true;                        //pressing downward arrow speeds up the game
        glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:                    //move the piece to the right
        posX++;
		break;
	case GLUT_KEY_LEFT:                     //move the piece to the right
        posX--;
		break;
	}
}
