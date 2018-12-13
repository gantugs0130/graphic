#include<iostream>
#include<fstream>//1616, 1970, 1616, 1604
#include <math.h>
#include<string.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include<stdio.h>
#include "texture.cpp"
class vertexs{
public:
    float x, y ,z;
};
class textures{
public:
    float x, y;
};
class normals{
public:
    float x, y, z;
};
class faces{
public:
    vertexs v[4];
    textures vt[4];
    normals vn[4];
};
normals vn[2000];
textures vt[2000];
vertexs v[2000];
faces f[2000];
int countv=0, countvn=0, countvt=0, countf=0;

using namespace std;
void File_Read(){
    ifstream  file;
    char a[2];
    file.open("../MayaCharacter/Character.obj");
    file >> a;
    int i = 0;
    while(strcmp(a,"v") == 0){
        file >> v[i].x;
        file >> v[i].y;
        file >> v[i].z;
        file >> a;
        i++;
    }
    countv=i;
    i=0;

        while(strcmp(a,"vt") == 0){
        file >> vt[i].x;
        file >> vt[i].y;
        file >> a;
        i++;
    }
    countvt=i;
    i=0;
    while(strcmp(a,"vn") == 0){
        file >> vn[i].x;
        file >> vn[i].y;
        file >> vn[i].z;
        file >> a;
        i++;
    }
    countvn=i;
    i=0;
    int pos;
    char ch;
    while(!file.eof()){
        for(int j=0; j<4; j++){
            file >> pos;
            f[i].v[j].x = v[pos-1].x;
            f[i].v[j].y = v[pos-1].y;
            f[i].v[j].z = v[pos-1].z;
            file >> ch;
            file >> pos;
            f[i].vt[j].x = vt[pos-1].x;
            f[i].vt[j].y = vt[pos-1].y;
            file >> ch;
            file >> pos;
            f[i].vn[j].x = vn[pos-1].x;
            f[i].vn[j].y = vn[pos-1].y;
            f[i].vn[j].z = vn[pos-1].z;     
        }
        i++;
        file >> a;
    }
    countf = i;
    }