#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include<fstream>
#define PI 3.14159265

using namespace std;

vector< vector<float> > vertex3f;
vector< vector<int> > vertexFace;
float angle =0;

// XZ position of the camera
float x=100.0f,z=100.0f,y=100.0f;

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

bool is_read = true;

void File_Read(){
    ifstream  infile;
    infile.open("./bunny.obj");
    int i=0;
    char a;
        infile >>a;
    while(a=='v'){
        vector<float> temp;
        float x;
        float y;
        float z;
        infile >>x;
        infile >>y;
        infile >>z;
        temp.push_back(x);
        temp.push_back(y);
        temp.push_back(z);
        vertex3f.push_back(temp);
        infile >>a;
    }
    while(!infile.eof()){
        int x1, x2, x3;
        vector<int> temp1;
        infile >>x1;
        infile >>x2;
        infile >>x3;
        temp1.push_back(x1);
        temp1.push_back(x2);
        temp1.push_back(x3);
        vertexFace.push_back(temp1);
        infile >>a;

    }
    infile.close();
}

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
    float corners[2][3];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
    gluLookAt(	y, x, z,
			0, 0, 0,
			0.0f, 1.0f,  0.0f);
    glColor3f(0.0f, 0.7f, 0.7f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < vertexFace.size(); i++)
	{

        float v[3][3];
        corners[0][0]=vertex3f[vertexFace[i][0] - 1][0];
        corners[0][1]=vertex3f[vertexFace[i][0] - 1][1];
        corners[0][2]=vertex3f[vertexFace[i][0] - 1][2];
        corners[1][0]=vertex3f[vertexFace[i][1] - 1][0];
        corners[1][1]=vertex3f[vertexFace[i][1] - 1][1];
        corners[1][2]=vertex3f[vertexFace[i][1] - 1][2];
        corners[2][0]=vertex3f[vertexFace[i][2] - 1][0];
        corners[2][1]=vertex3f[vertexFace[i][2] - 1][1];
        corners[2][2]=vertex3f[vertexFace[i][2] - 1][2];
        calcNormal(corners, normal);
        glNormal3fv(normal);
        glVertex3f(vertex3f[vertexFace[i][0] - 1][0], vertex3f[vertexFace[i][0] - 1][1], vertex3f[vertexFace[i][0] - 1][2]);
		glVertex3f(vertex3f[vertexFace[i][1] - 1][0], vertex3f[vertexFace[i][1] - 1][1], vertex3f[vertexFace[i][1] - 1][2]);
		glVertex3f(vertex3f[vertexFace[i][2] - 1][0], vertex3f[vertexFace[i][2] - 1][1], vertex3f[vertexFace[i][2] - 1][2]);
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