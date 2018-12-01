#include "header.h"
#include <GL/glu.h>
#include <GL/glut.h>
#define PI 3.14159265

float x=100.0f,z=100.0f,y=100.0f;
void SpecialKeys(int key, int x0, int y0)
    {
    if(key == GLUT_KEY_UP){
        y = cos(PI/90) * y - sin(PI/90) * z;
        z = cos(PI/90) * z + sin(PI/90) * y;
        glutPostRedisplay();
    }
    else {if(key == GLUT_KEY_DOWN){
        y = cos(PI/90) * y + sin(PI/90) * z;
        z = cos(PI/90) * z - sin(PI/90) * y;
        glutPostRedisplay();
    }
        else{
            if(key==GLUT_KEY_RIGHT){
            x = cos(PI/90) * x - sin(PI/90) * y;
            y = cos(PI/90) * y + sin(PI/90) * x;
            glutPostRedisplay();
            }
            else{ if(key == GLUT_KEY_LEFT){
            x = cos(PI/90) * x + sin(PI/90) * y;
            y = cos(PI/90) * y - sin(PI/90) * x;
            glutPostRedisplay();
            }
            else{
                if(key == GLUT_KEY_PAGE_UP){
                    float R = sqrt(x*x+y*y+z*z);
                    float Rotz = z/R;
                    z = (R - 5)*Rotz;
                    float Roty = y/R;
                    y = (R - 5)*Roty;
                    float Rotx = x/R;
                    x = (R - 5)*Rotx;
                    glutPostRedisplay();
                    }
                    else{
                        if(key == GLUT_KEY_PAGE_DOWN){
                            float R = sqrt(x*x+y*y+z*z);
                            float Rotz = z/R;
                            z = (R + 5)*Rotz;
                            float Roty = y/R;
                            y = (R + 5)*Roty;
                            float Rotx = x/R;
                            x = (R + 5)*Rotx;
                            glutPostRedisplay();
                        }
                    }
            }
            }
        }
    }

    }
void displayMe(void)
{   float normal[3];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
    gluLookAt(	y, x, z,
			0, 0, 0,
			0.0f, 1.0f,  0.0f);
    glColor3f(0.0f, 0.7f, 0.7f);
	glBegin(GL_QUADS);
	for (int i = 0; i < countf; i++)
	{   
        for(int j=0; j<4;j++){
            normal[0]=f[i].vn[j].x;
            normal[1]=f[i].vn[j].y;
            normal[2]=f[i].vn[j].z;
            glNormal3fv(normal);
            glVertex3f(f[i].v[j].x,f[i].v[j].y,f[i].v[j].z);
        }
	}
	glEnd();

	glPopMatrix();
	glutSwapBuffers();
}
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
    gluPerspective(35.0f, fAspect, 1.0, 400.0);
  //  glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
void setup()
{

	File_Read();
	GLfloat  ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat  diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat  specular[] = { 0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat  lightPos[] = { 0.0f, 40.0f, 0.0f, 1.0f };
    GLfloat  specref[] =  { 0.6f, 0.6f, 0.6f, 1.0f };
        glEnable(GL_DEPTH_TEST);    // Hidden surface removal
        glEnable(GL_CULL_FACE);        // Do not calculate inside of solid object
        glFrontFace(GL_CCW);

       // Enable lighting
       glEnable(GL_LIGHTING);

    // Setup light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);

//    Position and turn on the light
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    glEnable(GL_LIGHT0);

//    Enable color tracking

    glEnable(GL_COLOR_MATERIAL);

//    Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

//    All materials hereafter have full specular reflectivity
 //   with a moderate shine
    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT,GL_SHININESS,64);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("Bunny");
	glutReshapeFunc(resize);
	glutDisplayFunc(displayMe);
	glutSpecialFunc(SpecialKeys);
	setup();
	glutMainLoop();
	return 0;
}
