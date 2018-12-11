#include<iostream>
#include <math.h>
#include<string.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include<stdio.h>
GLint nNumPoints =4;
GLfloat ctrlPoints[4][3]={
    {-4.0f, -2.0f,0.0f},
    {-3.0f, 4.0f, 0.0f},
    {4.0f, -4.0f, 0.0f},
    {4.0f, -1.0f, 0.0f}
};
void ChangeSize(int w, int h)
{
    if(h==0)
    h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0f,10.0f,-10.0f,10.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void DrawPoint(){
    int i;
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for(i=0; i<nNumPoints; i++){
        glVertex2fv(ctrlPoints[i]);
    }
    glEnd();
}
void RenderScene(void){
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
    glMap1f(GL_MAP1_VERTEX_3,
            0.0f,
            100.0f,
            3,
            nNumPoints,
            &ctrlPoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);
    glPushMatrix();
    for(i=0; i<=100; i++){
        glEvalCoord1f((GLfloat)i);   
    }
    glEnd();
    DrawPoint();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{   
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("Bunny");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	return 0;
}