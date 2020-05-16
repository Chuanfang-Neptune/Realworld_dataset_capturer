#ifndef _Glfunc_H_
#define _Glfunc_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "opengl_math.h"
#include <FreeImage.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
<<<<<<< HEAD
#include<cstring>
//#include "robot_motion.h"
extern std::string pFileName,pVSFileName,pFSFileName,filename1s;
extern int WINDOWWIDTH,WINDOWHEIGHT;
extern Matrix44f mvp;
using namespace std;
=======
#include "robot_motion.h"
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339

#define MAX_VERTICES 300000
#define MAX_INDICES 300000
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL


// LEGO Car's geometric information 
struct lego_geo{  
    //geometric center
    float x;
    float y;
    float z;
    //geometry box
    float maxx;
    float minx;
    float maxy;
    float miny;
    float maxz;
    float minz;
};

//point's coordinates and index number 
struct point {
	int no;
	float X;
	float Y;
	float Z;
};

//triangle's three points
struct tri {
	int p[3];
};


void Render();
lego_geo readstl(char const *fname);
void CreateVertexBuffer();
void CreateIndexBuffer();
void CompilerShaders();
#endif
