#ifndef _Robot_motion_H_
#define _Robot_motion_H_

#include <cstring>

<<<<<<< HEAD
int WINDOWWIDTH = 640;
int WINDOWHEIGHT = 480;
//camera parameters

#define MAX_VERTICES 300000
#define MAX_INDICES 300000


extern Matrix44f mvp;
float c1, s1, c2, s2, c3, s3, c1_h, s1_h, c2_h, s2_h, c3_h, s3_h;
float FOV = 51.2f ; 
float zNear = 0.1f;
float zFar = 3.0f;
float fx = 668.0703112366639;
float fy = 668.0703112366639;
float cx = 321.74;
float cy = 238.02;
float caml = 0.06 ;  //Target---N  Up----V

//training object pose
float LEGO_POSITION_X = 0.6;
float LEGO_POSITION_Y = 0.55;
float LEGO_POSITION_Z = 1.072;

float LEGO_ROTATION_EULER_Y = 0.0f;
float LEGO_ROTATION_EULER_P = 0.0f;
float LEGO_ROTATION_EULER_R = 120.0f; //LEGO euler R rotation degree

string STLFileName = "./data/model_stl.stl";
string pVSFileName = "./data/shader.vs";
string pFSFileName = "./data/shader.fs";
string rgbTopic = "/camera_kinect_head_mount/depth/image_raw";
string depthTopic = "/camera_kinect_head_mount_ir/depth/image_raw";
string jsonFilePath = "./data/json/";
=======
#define PI 3.141592653
#define PI_DIV_180 (0.017453292519943296)
#define INV_PI_DIV_180 (57.2957795130823229)
#define DegToRad(x)  ((x)*PI_DIV_180)
#define RadToDeg(x)  ((x)*INV_PI_DIV_180)

#define WINDOWWIDTH 640
#define WINDOWHEIGHT 480
//camera parameters
#define FOV 51.2f  
#define zNear 0.1f
#define zFar 3.0f
#define fx 668.0703112366639
#define fy 668.0703112366639
#define cx 321.74
#define cy 238.02
#define caml 0.06   //Target---N  Up----V

//training object pose
#define LEGO_POSITION_X 0.6
#define LEGO_POSITION_Y 0.55
#define LEGO_POSITION_Z 1.072

#define LEGO_ROTATION_EULER_Y 0.0f
#define LEGO_ROTATION_EULER_P 0.0f
#define LEGO_ROTATION_EULER_R 120.0f //LEGO euler R rotation degree

#define PI 3.141592653
#define MAX_VERTICES 300000
#define MAX_INDICES 300000

const float c1 = cos(DegToRad(LEGO_ROTATION_EULER_Y));
const float s1 = sin(DegToRad(LEGO_ROTATION_EULER_Y));
const float c2 = cos(DegToRad(LEGO_ROTATION_EULER_P));
const float s2 = sin(DegToRad(LEGO_ROTATION_EULER_P));
const float c3 = cos(DegToRad(LEGO_ROTATION_EULER_R));
const float s3 = sin(DegToRad(LEGO_ROTATION_EULER_R));
const float c1_h = cos(DegToRad(LEGO_ROTATION_EULER_Y)/2);
const float s1_h = sin(DegToRad(LEGO_ROTATION_EULER_Y)/2);
const float c2_h = cos(DegToRad(LEGO_ROTATION_EULER_P)/2);
const float s2_h = sin(DegToRad(LEGO_ROTATION_EULER_P)/2);
const float c3_h = cos(DegToRad(LEGO_ROTATION_EULER_R)/2);
const float s3_h = sin(DegToRad(LEGO_ROTATION_EULER_R)/2);
extern std::string filename1s;
extern Matrix44f mvp;
extern const char* pVSFileName;
extern const char* pFSFileName;
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
#endif
