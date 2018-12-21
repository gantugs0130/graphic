#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#define PI 3.14159265
using namespace std;
double alpha =PI/90;
static float normalx=0, normaly=0, normalz=1;
static float posx=200.0f,posz=50.0f,posy=500.0f;
vector< vector<float> > vertex3f;
vector< vector<int> > vertexFace;

	GLfloat  ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat  diffuseLight[] = {0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat  specular[] = { 0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat  lightPos[] = { posx+50, posy, 0, 1.0f };
    GLfloat  specref[] =  { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat shadowMat[4][4];

       GLfloat points[3][3] = {{ -1.0f, -49.0f, 0.0f },
                            { -1.0f, -49.0f, 1.0f },
                            { 0.0f, -49.0f, 1.0f }};
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

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
    gluPerspective(35.0f, fAspect, 1.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
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
void MakeShadowMatrix(GLfloat points[3][3], GLfloat lightPos[4], GLfloat destMat[4][4])
    {
    GLfloat planeCoeff[4];
    GLfloat dot;

    // Find the plane equation coefficients
    // Find the first three coefficients the same way we
    // find a normal.
    calcNormal(points,planeCoeff);

    // Find the last coefficient by back substitutions
    planeCoeff[3] = - (
        (planeCoeff[0]*points[2][0]) + (planeCoeff[1]*points[2][1]) +
        (planeCoeff[2]*points[2][2]));


    // Dot product of plane and light position
    dot = planeCoeff[0] * lightPos[0] +
            planeCoeff[1] * lightPos[1] +
            planeCoeff[2] * lightPos[2] +
            planeCoeff[3] * lightPos[3];

    // Now do the projection
    // First column
        destMat[0][0] = dot - lightPos[0] * planeCoeff[0];
        destMat[1][0] = 0.0f - lightPos[0] * planeCoeff[1];
        destMat[2][0] = 0.0f - lightPos[0] * planeCoeff[2];
        destMat[3][0] = 0.0f - lightPos[0] * planeCoeff[3];

    // Second column
    destMat[0][1] = 0.0f - lightPos[1] * planeCoeff[0];
    destMat[1][1] = dot - lightPos[1] * planeCoeff[1];
    destMat[2][1] = 0.0f - lightPos[1] * planeCoeff[2];
    destMat[3][1] = 0.0f - lightPos[1] * planeCoeff[3];

    // Third Column
    destMat[0][2] = 0.0f - lightPos[2] * planeCoeff[0];
    destMat[1][2] = 0.0f - lightPos[2] * planeCoeff[1];
    destMat[2][2] = dot - lightPos[2] * planeCoeff[2];
    destMat[3][2] = 0.0f - lightPos[2] * planeCoeff[3];

    // Fourth Column
    destMat[0][3] = 0.0f - lightPos[3] * planeCoeff[0];
    destMat[1][3] = 0.0f - lightPos[3] * planeCoeff[1];
    destMat[2][3] = 0.0f - lightPos[3] * planeCoeff[2];
    destMat[3][3] = dot - lightPos[3] * planeCoeff[3];
    } 
void SpecialKeys(int key, int x0, int y0)
    {
    lightPos[0]=posx+50;
    lightPos[1]=posy;
    MakeShadowMatrix(points, lightPos, shadowMat);
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
void setup()
{

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
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA);
    glEnable(GL_COLOR_MATERIAL);

//    Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

//    All materials hereafter have full specular reflectivity
 //   with a moderate shine
    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT,GL_SHININESS,64);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
MakeShadowMatrix(points, lightPos, shadowMat);
}


// Calculate the next bezier point.
void draw_world(float fx, float fy, float fz, float r){
    float normal[3];
    glPushMatrix();
    normal[0] = 0;
    normal[1] = 0;
    normal[2] = 1;
    glColor3f(0,1,0);
    glNormal3fv(normal);
    glBegin(GL_QUADS);
    glVertex3f(fx+r,fy+r/2,fz);
    glVertex3f(fx-r,fy+r/2,fz);
    glVertex3f(fx-r,fy-r/2,fz);
    glVertex3f(fx+r,fy-r/2,fz);
    glEnd();
    normal[0] = 0;
    normal[1] = 1;
    normal[2] = 0;
    glNormal3fv(normal);
    glColor3f(0.0,0.9,0.9);
    glBegin(GL_QUADS);
    glVertex3f(fx+r,fy-r/2,fz);
    glVertex3f(fx-r,fy-r/2,fz);
    glVertex3f(fx-r,fy-r/2,fz+r);
    glVertex3f(fx+r,fy-r/2,fz+r);
    
    glEnd();
    normal[0] = 0;
    normal[1] = 1;
    normal[2] = 0;
    glNormal3fv(normal);
    glColor3f(0.0,0.9,0.9);
    glBegin(GL_QUADS);

    glVertex3f(fx-r,fy+r/2,fz);
    glVertex3f(fx+r,fy+r/2,fz);
    glVertex3f(fx+r,fy+r/2,fz+r);
    glVertex3f(fx-r,fy+r/2,fz+r);
    
    glEnd();
    normal[0] = 0;
    normal[1] = 1;
    normal[2] = 0;
    glNormal3fv(normal);
    glColor3f(0.0,0.9,0.9);
    glBegin(GL_QUADS);
    glVertex3f(fx-r,fy-r/2,fz);
    glVertex3f(fx-r,fy+r/2,fz);
    glVertex3f(fx-r,fy+r/2,fz+r);
    glVertex3f(fx-r,fy-r/2,fz+r);
    glEnd();
    normal[0] = 1;
    normal[1] = 0;
    normal[2] = 0;
    glNormal3fv(normal);
    glColor3f(0.0,0.9,0.9);
    glBegin(GL_QUADS);
    glVertex3f(fx+r,fy+r/2,fz+r);
    glVertex3f(fx+r,fy+r/2,fz);
    glVertex3f(fx+r,fy-r/2,fz);
    glVertex3f(fx+r,fy-r/2,fz+r);
    glEnd();
    glPopMatrix();

}
void draw_blend(float fx, float fy, float fz, float r){
    float normal[3];
    glEnable(GL_BLEND);
    normal[0] = 0;
    normal[1] = 0;
    normal[2] = 1;
    glNormal3fv(normal);
    glColor4f(1,1,0, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(fx+r,fy+r/2,fz+r);
    glVertex3f(fx-r,fy+r/2,fz+r);
    glVertex3f(fx-r,fy-r/2,fz+r);
    glVertex3f(fx+r,fy-r/2,fz+r);
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
}
void draw_bunny(bool bshadow){
    float normal[3];    // Storeage for calculated surface normal
float corners[2][3];
if(!bshadow){
    glColor3f(0.5,0,0.5);
}else{
    glColor3f(0,0,0);
}
    glPushMatrix(); 
    glTranslatef(0,0,13);
    
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
}
void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    gluLookAt(	posx, posy, posz,
			0, 0, 0,
			normalx, normaly, normalz); 
    glPushMatrix();
    draw_bunny(false);
    draw_world( 0 , 0 , 0 , 100);
    draw_blend( 0 , 0 , 0 , 100);
    glMultMatrixf((GLfloat *)shadowMat);
    draw_bunny(true);
    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    File_Read();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,800);
    glutInitWindowPosition(100,150);
    glutCreateWindow("Shadow");   
    glutReshapeFunc(resize);
    glutDisplayFunc(myDisplay);
    glutSpecialFunc(SpecialKeys);
    setup();
    glutMainLoop();
    return 0;
}