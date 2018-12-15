#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include<fstream>
#define PI 3.14159265

using namespace std;
float posx=0.0f,posz=0.0f,posy=100.0f;
static float normalx=0, normaly=1, normalz=0;
vector< vector<float> > vertex3f;
vector< vector<int> > vertexFace;


// Define a constant for the value of PI
#define GL_PI 3.1415f

// Rotation amounts

// These values need to be available globally
// Light values and coordinates
GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat     lightPos[] = { -75.0f, 150.0f, -50.0f, 0.0f };
GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

// Transformation matrix to project shadow
GLfloat shadowMat[4][4];


// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
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
void ReduceToUnit(float vector[3])
    {
    float length;
    
    // Calculate the length of the vector        
    length = (float)sqrt((vector[0]*vector[0]) +
                        (vector[1]*vector[1]) +
                        (vector[2]*vector[2]));

    // Keep the program from blowing up by providing an exceptable
    // value for vectors that may calculated too close to zero.
    if(length == 0.0f)
        length = 1.0f;

    // Dividing each element by the length will result in a
    // unit normal vector.
    vector[0] /= length;
    vector[1] /= length;
    vector[2] /= length;
    }


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
    {
    float v1[3],v2[3];
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
    out[x] = v1[y]*v2[z] - v1[z]*v2[y];
    out[y] = v1[z]*v2[x] - v1[x]*v2[z];
    out[z] = v1[x]*v2[y] - v1[y]*v2[x];

    // Normalize the vector (shorten length to one)
    ReduceToUnit(out);
    }

// Creates a shadow projection matrix out of the plane equation
// coefficients and the position of the light. The return value is stored
// in destMat[][]
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


////////////////////////////////////////////////
// This function just specifically draws the jet
void DrawJet(bool bShadow)
    {
    float normal[3];    // Storeage for calculated surface normal
float corners[2][3];
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
    }

// Called to draw scene
void RenderScene(void)
    {

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    gluLookAt(posx, posy, posz,
			0, 0, 0,
			normalx, normaly,  normalz);
    glPopMatrix();
    glLoadIdentity();
    // Draw the ground, we do manual shading to a darker green
    // in the background to give the illusion of depth
    glBegin(GL_QUADS);
        glColor3ub(0,32,0);
        glVertex3f(400.0f, -150.0f, -200.0f);
        glVertex3f(-400.0f, -150.0f, -200.0f);
        glColor3ub(0,255,0);
        glVertex3f(-400.0f, -150.0f, 200.0f);
        glVertex3f(400.0f, -150.0f, 200.0f);
    glEnd();

    // Save the matrix state and do the rotations
    glPushMatrix();

    // Draw jet at new orientation, put light in correct position
    // before rotating the jet
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

    DrawJet(false);

    // Restore original matrix state
    glPopMatrix();    


    // Get ready to draw the shadow and the ground
    // First disable lighting and save the projection state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glPushMatrix();

    // Multiply by shadow projection matrix
    glMultMatrixf((GLfloat *)shadowMat);

    // Now rotate the jet around in the new flattend space

    // Pass true to indicate drawing shadow
    DrawJet(true);    

    // Restore the projection to normal
    glPopMatrix();

    // Draw the light source
    glPushMatrix();
    glTranslatef(lightPos[0],lightPos[1], lightPos[2]);
    glColor3ub(255,255,0);
    glutSolidSphere(5.0f,10,10);
    glPopMatrix();

    // Restore lighting state variables
    glEnable(GL_DEPTH_TEST);

    // Display the results
    glutSwapBuffers();
    }

// This function does any needed initialization on the rendering
// context.
void SetupRC()
    {
    // Any three points on the ground (counter clockwise order)
    GLfloat points[3][3] = {{ -30.0f, -149.0f, -20.0f },
                            { -30.0f, -149.0f, 20.0f },
                            { 40.0f, -149.0f, 20.0f }};

    glEnable(GL_DEPTH_TEST);    // Hidden surface removal
    glFrontFace(GL_CCW);        // Counter clock-wise polygons face out
    glEnable(GL_CULL_FACE);        // Do not calculate inside of jet

    // Setup and enable light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    
    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT,GL_SHININESS,128);

    // Light blue background
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f );

    // Calculate projection matrix to draw shadow on the ground
    MakeShadowMatrix(points, lightPos, shadowMat);
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

void ChangeSize(int w, int h)
    {
    GLfloat nRange = 200.0f;
    GLfloat fAspect;

    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    
    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    fAspect = (GLfloat)w/(GLfloat)h;

    gluPerspective(60.0f, fAspect, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);

  
    // Move out Z axis so we can see everything
    glTranslatef(0.0f, 0.0f, -400.0f);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    }

int main(int argc, char* argv[])

    {
        File_Read();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 50);
    glutCreateWindow("Shadow");
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
    SetupRC();
    glutMainLoop();

    return 0;
    }