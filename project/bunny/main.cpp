#include <vector>
#include "header.h"
#define PI 3.14159265

using namespace std;

vector< vector<float> > vertex3f;
vector< vector<int> > vertexFace;

// XZ position of the camera
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
    GLUquadricObj *pObj;  
    float normal[3];
    float corners[2][3];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
    gluLookAt(	posx, posy, posz,
			0, 0, 0,
			normalx, normaly,  normalz);
    glColor3f(0.0f, 0.0f, 0.0f);
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);
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
    glBegin(GL_QUADS);
    corners[0][0]=-100;
    corners[0][1]=-5;
    corners[0][2]=-100;
    corners[1][0]=-100;
    corners[1][1]=-5;
    corners[1][2]=100;
    corners[2][0]=100;
    corners[2][1]=-5;
    corners[2][2]=100;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(-100,-5,-100);
    glVertex3f(-100,-5,100);
    glVertex3f(100,-5,100);
    glVertex3f(100,-5,-100);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=-100;
    corners[0][1]=0;
    corners[0][2]=-100;
    corners[1][0]=-100;
    corners[1][1]=0;
    corners[1][2]=100;
    corners[2][0]=-100;
    corners[2][1]=-5;
    corners[2][2]=100;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(-100,0,-100);
    glVertex3f(-100,0,100);
    glVertex3f(-100,-5,100);
    glVertex3f(-100,-5,-100);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=-100;
    corners[0][1]=0;
    corners[0][2]=100;
    corners[1][0]=100;
    corners[1][1]=0;
    corners[1][2]=100;
    corners[2][0]=100;
    corners[2][1]=-5;
    corners[2][2]=100;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(-100,0,100);
    glVertex3f(100,0,100);
    glVertex3f(100,-5,100);
    glVertex3f(-100,-5,100);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=100;
    corners[0][1]=0;
    corners[0][2]=100;
    corners[1][0]=100;
    corners[1][1]=0;
    corners[1][2]=-100;
    corners[2][0]=100;
    corners[2][1]=-5;
    corners[2][2]=-100;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(100,0,100);
    glVertex3f(100,0,-100);
    glVertex3f(100,-5,-100);
    glVertex3f(100,-5,100);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=100;
    corners[0][1]=0;
    corners[0][2]=-100;
    corners[1][0]=-100;
    corners[1][1]=0;
    corners[1][2]=-100;
    corners[2][0]=-100;
    corners[2][1]=-5;
    corners[2][2]=-100;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(100,0,-100);
    glVertex3f(-100,0,-100);
    glVertex3f(-100,-5,-100);
    glVertex3f(100,-5,-100);
    glEnd();

    GLUquadricObj *pObjj;  
    float normall[3];
    float cornerss[2][3];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
    gluLookAt(	posx, posy, posz,
			0, 0, 0,
			normalx, normaly,  normalz);
    glColor3f(0.0f, 0.5f, 0.4f);
    pObjj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);
    glBegin(GL_QUADS);
    corners[0][0]=-70;
    corners[0][1]=0;
    corners[0][2]=-70;
    corners[1][0]=-70;
    corners[1][1]=0;
    corners[1][2]=70;
    corners[2][0]=70;
    corners[2][1]=0;
    corners[2][2]=70;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(-70,0,-70);
    glVertex3f(-70,0,70);
    glVertex3f(70,0,70);
    glVertex3f(70,0,70);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=-70;
    corners[0][1]=-3;
    corners[0][2]=-70;
    corners[1][0]=-70;
    corners[1][1]=-3;
    corners[1][2]=70;
    corners[2][0]=70;
    corners[2][1]=-3;
    corners[2][2]=70;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(-70,-3,-70);
    glVertex3f(-70,-3,70);
    glVertex3f(70,-3,70);
    glVertex3f(70,-3,-70);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=-70;
    corners[0][1]=0;
    corners[0][2]=-70;
    corners[1][0]=-70;
    corners[1][1]=0;
    corners[1][2]=70;
    corners[2][0]=-70;
    corners[2][1]=-3;
    corners[2][2]=70;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(-70,0,-70);
    glVertex3f(-70,0,70);
    glVertex3f(-70,-3,70);
    glVertex3f(-70,-3,-70);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=-70;
    corners[0][1]=0;
    corners[0][2]=70;
    corners[1][0]=70;
    corners[1][1]=0;
    corners[1][2]=70;
    corners[2][0]=70;
    corners[2][1]=-3;
    corners[2][2]=70;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(-70,0,70);
    glVertex3f(70,0,70);
    glVertex3f(70,-3,70);
    glVertex3f(-70,-3,70);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=70;
    corners[0][1]=0;
    corners[0][2]=70;
    corners[1][0]=70;
    corners[1][1]=0;
    corners[1][2]=-70;
    corners[2][0]=70;
    corners[2][1]=-3;
    corners[2][2]=-70;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(70,0,70);
    glVertex3f(70,0,-70);
    glVertex3f(70,-3,-70);
    glVertex3f(70,-3,70);
    glEnd();
    glBegin(GL_QUADS);
    corners[0][0]=70;
    corners[0][1]=0;
    corners[0][2]=-70;
    corners[1][0]=-70;
    corners[1][1]=0;
    corners[1][2]=-70;
    corners[2][0]=-70;
    corners[2][1]=-3;
    corners[2][2]=-70;
    calcNormal(corners, normal);
    glNormal3fv(normal);
    glVertex3f(70,0,-70);
    glVertex3f(-70,0,-70);
    glVertex3f(-70,-3,-70);
    glVertex3f(70,-3,-70);
    glEnd();
    
    
    glColor3f(0.4f, 0.4f, 0.5f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < vertexFace.size(); i++)
	{

        float v[3][3];
        corners[0][0]=vertex3f[vertexFace[i][0] - 1][0];
        corners[0][1]=vertex3f[vertexFace[i][0] - 1][2]+13;
        corners[0][2]=-vertex3f[vertexFace[i][0] - 1][1];
        corners[1][0]=vertex3f[vertexFace[i][1] - 1][0];
        corners[1][1]=vertex3f[vertexFace[i][1] - 1][2]+13;
        corners[1][2]=-vertex3f[vertexFace[i][1] - 1][1];
        corners[2][0]=vertex3f[vertexFace[i][2] - 1][0];
        corners[2][1]=vertex3f[vertexFace[i][2] - 1][2]+13;
        corners[2][2]=-vertex3f[vertexFace[i][2] - 1][1];
        calcNormal(corners, normal);
        glNormal3fv(normal);
        glVertex3f(vertex3f[vertexFace[i][0] - 1][0], vertex3f[vertexFace[i][0] - 1][2]+13, -vertex3f[vertexFace[i][0] - 1][1]);
		glVertex3f(vertex3f[vertexFace[i][1] - 1][0], vertex3f[vertexFace[i][1] - 1][2]+13, -vertex3f[vertexFace[i][1] - 1][1]);
        glVertex3f(vertex3f[vertexFace[i][2] - 1][0], vertex3f[vertexFace[i][2] - 1][2]+13, -vertex3f[vertexFace[i][2] - 1][1]);
	}
	glEnd();
    //glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	for (int i = 0; i < countf; i++)
	{   
        for(int j=0; j<4;j++){
            normal[0]=f[i].vn[j].x;
            normal[1]=f[i].vn[j].y;
            normal[2]=f[i].vn[j].z;
            glNormal3fv(normal);
            // glTexCoord2f(f[i].vt[j].x,f[i].vt[j].y);
            glVertex3f(2*f[i].v[j].x+50,2*f[i].v[j].y,2*f[i].v[j].z);
        }
	}
	glEnd();
    glBegin(GL_QUADS);
	for (int i = 0; i < countf; i++)
	{   
        for(int j=0; j<4;j++){
            normal[0]=f[i].vn[j].x;
            normal[1]=f[i].vn[j].y;
            normal[2]=f[i].vn[j].z;
            glNormal3fv(normal);
            // glTexCoord2f(f[i].vt[j].x,f[i].vt[j].y);
            glVertex3f(f[i].v[j].x+30,f[i].v[j].y,f[i].v[j].z);
        }
	}
	glEnd();
    glBegin(GL_QUADS);
	for (int i = 0; i < countf; i++)
	{   
        for(int j=0; j<4;j++){
            normal[0]=f[i].vn[j].x;
            normal[1]=f[i].vn[j].y;
            normal[2]=f[i].vn[j].z;
            glNormal3fv(normal);
            // glTexCoord2f(f[i].vt[j].x,f[i].vt[j].y);
            glVertex3f(f[i].v[j].x-30,f[i].v[j].y,f[i].v[j].z);
        }
	}
	glEnd();
    glBegin(GL_QUADS);
	for (int i = 0; i < countf; i++)
	{   
        for(int j=0; j<4;j++){
            normal[0]=f[i].vn[j].x;
            normal[1]=f[i].vn[j].y;
            normal[2]=f[i].vn[j].z;
            glNormal3fv(normal);
            // glTexCoord2f(f[i].vt[j].x,f[i].vt[j].y);
            glVertex3f(2*f[i].v[j].x-50,2*f[i].v[j].y,2*f[i].v[j].z);
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
    gluPerspective(35.0f, fAspect, 1.0, 1000.0);
  //  glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void setup()
{
    File_Read1();
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
    glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping


   // int ss=LoadBitmap("./Mapping.bmp");
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
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("Bunny");
	glutReshapeFunc(resize);
	glutDisplayFunc(displayMe);
	glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(normalKeys);
	setup();
	glutMainLoop();
	return 0;
}