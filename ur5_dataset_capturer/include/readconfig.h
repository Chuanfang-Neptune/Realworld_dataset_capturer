#ifndef _Readconfig_H_
#define _Readconfig_H_
using namespace std;
#include <fstream>
#include <iostream>
#include <map>
#include "opengl_math.h"
#define PI 3.141592653
#define PI_DIV_180 (0.017453292519943296)
#define INV_PI_DIV_180 (57.2957795130823229)
#define DegToRad(x)  ((x)*PI_DIV_180)
#define RadToDeg(x)  ((x)*INV_PI_DIV_180)

extern float FOV,zNear,zFar,fx,fy,cx,cy,caml;  //Target---N  Up----V
extern float LEGO_POSITION_X,LEGO_POSITION_Y,LEGO_POSITION_Z,LEGO_ROTATION_EULER_Y,LEGO_ROTATION_EULER_P,LEGO_ROTATION_EULER_R;
extern float c1, s1, c2, s2, c3, s3, c1_h, s1_h, c2_h, s2_h, c3_h, s3_h;
extern std::string filename1s,STLFileName,pVSFileName,pFSFileName,jsonFilePath,depthTopic,rgbTopic;
void readconfig();
#endif
