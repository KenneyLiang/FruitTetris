#include<windows.h>     //Delete if being compiling in Linux
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include "grid.h"
#include <cassert>


int gridX, gridY;

void initGrid(int x, int y){

        gridX = x;
        gridY = y;
}

void unit(int, int);

void drawGrid(){

    for (int x = 0; x < gridX; x++)
    {
        for (int y = 0; y < gridY; y++)
        {
            unit(x,y);
        }
    }
}

void unit(int x, int y){

    glLineWidth(1.0);
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINE_LOOP);

        glVertex2f(x,y);
        glVertex2f(x+1,y);
        glVertex2f(x+1,y+1);
        glVertex2f(x,y+1);

    glEnd();
}

