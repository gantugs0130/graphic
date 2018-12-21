#include <vector>
#include<iostream>
#include<fstream>
#include <math.h>
#include<string.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define PI 3.14159265
using namespace std;

float posx=0.0f,posz=0.0f,posy=200.0f;
static float normalx=0, normaly=0, normalz=1;
void ReduceToUnit(float vector[3])
{
	float length;

	// Calculate the length of the vector
	length = float(sqrt((vector[0] * vector[0]) +
						 (vector[1] * vector[1]) +
						 (vector[2] * vector[2])));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}

void calcNormal(float v[3][3], float out[3])
{
	float v1[3], v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y] * v2[z] - v1[z] * v2[y];
	out[y] = v1[z] * v2[x] - v1[x] * v2[z];
	out[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}

void normalKeys(unsigned char key, int x00, int y00){
    switch(key){
        case 'p':
        posx = cos(PI/90) * posx - sin(PI/90) * posz;
        posz = cos(PI/90) * posz + sin(PI/90) * posx;
        normalx=cos(PI/90) * normalx - sin(PI/90) * normalz;
        normalz=cos(PI/90) * normalz + sin(PI/90) * normalx;  
        glutPostRedisplay();
        break;
        case 'o':
        posx = cos(PI/90) * posx + sin(PI/90) * posz;
        posz = cos(PI/90) * posz - sin(PI/90) * posx;
        normalx=cos(PI/90) * normalx + sin(PI/90) * normalz;
        normalz=cos(PI/90) * normalz - sin(PI/90) * normalx;  
        glutPostRedisplay();
        break;
    }
}
void SpecialKeys(int key, int x0, int y0)
    {
    if(key == GLUT_KEY_UP){
        posy = cos(PI/90) * posy - sin(PI/90) * posz;
        posz = cos(PI/90) * posz + sin(PI/90) * posy;
        normaly=cos(PI/90) * normaly - sin(PI/90) * normalz;
        normalz=cos(PI/90) * normalz + sin(PI/90) * normaly;  
        glutPostRedisplay();
    }
    else {if(key == GLUT_KEY_DOWN){
        posy = cos(PI/90) * posy + sin(PI/90) * posz;
        posz = cos(PI/90) * posz - sin(PI/90) * posy;
        normaly=cos(PI/90) * normaly + sin(PI/90) * normalz;
        normalz=cos(PI/90) * normalz - sin(PI/90) * normaly;  
        glutPostRedisplay();
    }
        else{
            if(key==GLUT_KEY_RIGHT){
            posx = cos(PI/90) * posx - sin(PI/90) * posy;
            posy = cos(PI/90) * posy + sin(PI/90) * posx;
            normaly=cos(PI/90) * normaly + sin(PI/90) * normalx;
            normalx=cos(PI/90) * normalx - sin(PI/90) * normaly; 
            glutPostRedisplay();
            }
            else{ if(key == GLUT_KEY_LEFT){
            posx = cos(PI/90) * posx + sin(PI/90) * posy;
            posy = cos(PI/90) * posy - sin(PI/90) * posx;
            normaly=cos(PI/90) * normaly - sin(PI/90) * normalx;
            normalx=cos(PI/90) * normalx + sin(PI/90) * normaly; 
            glutPostRedisplay();
            }
            else{
                if(key == GLUT_KEY_PAGE_UP){
                    float R = sqrt(posx*posx+posy*posy+posz*posz);
                    float Rotz = posz/R;
                    posz = (R - 5)*Rotz;
                    float Roty = posy/R;
                    posy = (R - 5)*Roty;
                    float Rotx = posx/R;
                    posx = (R - 5)*Rotx;
                    glutPostRedisplay();
                    }
                    else{
                        if(key == GLUT_KEY_PAGE_DOWN){
                            float R = sqrt(posx*posx+posy*posy+posz*posz);
                            float Rotz = posz/R;
                            posz = (R + 5)*Rotz;
                            float Roty = posy/R;
                            posy = (R + 5)*Roty;
                            float Rotx = posx/R;
                            posx = (R + 5)*Rotx;
                            glutPostRedisplay();
                        }
                    }
            }
            }
        }
    }

    }
void displayMe(void)
{   
    float normal[3];
    float corners[2][3];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
    gluLookAt(	posx, posy, posz,
			0, 0, 0,
			normalx, normaly,  normalz);
    glColor3f(0.0f, 0.5f, 0.4f);

    glBegin(GL_QUADS);
    corners[0][0]=-100;
    corners[0][1]=0;
    corners[0][2]=-100;
    corners[1][0]=-100;
    corners[1][1]=0;
    corners[1][2]=100;
    corners[2][0]=100;
    corners[2][1]=0;
    corners[2][2]=100;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(-100,0,-100);
    glVertex3f(-100,0,100);
    glVertex3f(100,0,100);
    glVertex3f(100,0,-100);
    glEnd();
	glPopMatrix();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
    gluPerspective(35.0f, fAspect, 0.0, 2000.0);
  //  glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void setup()
{
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

    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT,GL_SHININESS,64);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("World");
	glutReshapeFunc(resize);
	glutDisplayFunc(displayMe);
	glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(normalKeys);
	//setup();
	glutMainLoop();
	return 0;
}