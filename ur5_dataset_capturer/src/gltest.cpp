//#include <GL/glut.h>
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))==NULL

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <cassert>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "opengl_math.h"

#include <cv_bridge/cv_bridge.h>
#include <FreeImage.h>
#define PI 3.141592653
#define PI_DIV_180 (0.017453292519943296)
#define INV_PI_DIV_180 (57.2957795130823229)
#define DegToRad(x)  ((x)*PI_DIV_180)
#define RadToDeg(x)  ((x)*INV_PI_DIV_180)
/*#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include <glm/glm.hpp>
*/
//using namespace glm;


/*
{
   "camera_data" : {
      "location_worldframe" : [ 0.11887708695273554, 0.22291060676013175, 0.20052210809078458 ],
      "quaternion_xyzw_worldframe" : [
         -0.010499895940162245,
         0.029651254533955899,
         0.36732053307254914,
         0.92956236007763704
      ]
   },
   "object_data" : {
      "location_worldframe" : [ 0.40000000000000002, 0.5, 0.20000000000000001 ],
      "quaternion_xyzw_worldframe" : [ 0, 0, 0.70710678108228575, 0.70710678129080928 ]
      LEGO_ROTATION_EULER_Y
   }
   // Projection matrix : 45° Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units
}

*/

//projection settings
#define WINDOWWIDTH 640
#define WINDOWHEIGHT 480
#define FOV 90.0f  
#define zNear 0.1f
#define zFar 3.0f
//camera transformation
#define crx -0.0064328140579164028
#define cry -0.06613536924123764
#define crz 0.71097517013549805
#define crw 0.9213130095837655
#define clx 0.36334344744682312
#define cly 0.17165933549404144
#define clz 1.110893726348877
/*
0.36334344744682312, 0.17165933549404144, 1.110893726348877
#define crx -0.010499895940162245
#define cry 0.029651254533955899
#define crz 0.36732053307254914
#define crw 0.92956236007763704
#define clx 0.11887708695273554
#define cly 0.22291060676013175
#define clz 0.20052210809078458*/
//object settings
#define olx 0.4
#define oly 0.55
#define olz 1.1
#define LEGO_ROTATION_EULER_Y 0.0f
#define LEGO_ROTATION_EULER_P 0.0f
#define LEGO_ROTATION_EULER_R 0.0f

#define PI 3.141592653
#define MAX_VERTICES 300000
#define MAX_INDICES 300000
const float c1 = cos(DegToRad(LEGO_ROTATION_EULER_Y));
const float s1 = sin(DegToRad(LEGO_ROTATION_EULER_Y));
const float c2 = cos(DegToRad(LEGO_ROTATION_EULER_P));
const float s2 = sin(DegToRad(LEGO_ROTATION_EULER_P));
const float c3 = cos(DegToRad(LEGO_ROTATION_EULER_R));
const float s3 = sin(DegToRad(LEGO_ROTATION_EULER_R));

const float ar = WINDOWWIDTH / WINDOWHEIGHT;
const float zRange = zNear - zFar;
const float tanHalfFOV = tanf(DegToRad(FOV / 2.0f));

Vector3f Vertices[MAX_VERTICES];//顶点坐标
unsigned int Indices[MAX_INDICES];
int vertices_count = 0;

int indices_count = 0;

/*
cv::Mat Euler2Quaternion(float *angle)
{
  float heading = angle[0];
  float attitude = angle[1];
  float bank = angle[2];

  float c1 = cos(heading/2);
  float s1 = sin(heading/2);
  float c2 = cos(attitude/2);
  float s2 = sin(attitude/2);
  float c3 = cos(bank/2);
  float s3 = sin(bank/2);
  float c1c2 = c1*c2;
  float s1s2 = s1*s2;
  float w =c1c2*c3 - s1s2*s3;
  float x =c1c2*s3 + s1s2*c3;
  float y =s1*c2*c3 + c1*s2*s3;
  float z =c1*s2*c3 - s1*c2*s3;
  float q[4] = {w,x,y,z};
  cv::Mat ret(4,1,CV_32FC1,q);
  return ret.clone();

}
cv::Mat Euler2Quaternion(cv::Mat Angle)
{
  //angle:roll pitch yaw
  //    q:w x y z
  float angle[3];
  angle[0] = Angle.at<float>(0);
  angle[1] = Angle.at<float>(1);
  angle[2] = Angle.at<float>(2);

  return Euler2Quaternion(angle).clone();
}
cv::Mat Quaternion2Matrix (cv::Mat q)
{
  float w = q.at<float>(0);
  float x = q.at<float>(1);
  float y = q.at<float>(2);
  float z = q.at<float>(3);

  float xx = x*x;
  float yy = y*y;
  float zz = z*z;
  float xy = x*y;
  float wz = w*z;
  float wy = w*y;
  float xz = x*z;
  float yz = y*z;
  float wx = w*x;

  float ret[4][4];
  ret[0][0] = 1.0f-2*(yy+zz);
  ret[0][1] = 2*(xy-wz);
  ret[0][2] = 2*(wy+xz);
  ret[0][3] = 0.0f;
 
  ret[1][0] = 2*(xy+wz);
  ret[1][1] = 1.0f-2*(xx+zz);
  ret[1][2] = 2*(yz-wx);
  ret[1][3] = 0.0f;
 
  ret[2][0] = 2*(xz-wy);
  ret[2][1] = 2*(yz+wx);
  ret[2][2] = 1.0f-2*(xx+yy);
  ret[2][3] = 0.0f;
 
  ret[3][0] = 0.0f;
  ret[3][1] = 0.0f;
  ret[3][2] = 0.0f;
  ret[3][3] = 1.0f;
 
  return cv::Mat(4,4,CV_32FC1,ret).clone();
}
cv::Mat Quaternion2MatrixInvert(float x, float y, float z, float w)
{
  float ret[3] = {0};
  float test = x*y + z*w;
  if (test > 0.4999f) {
    ret[2] = 2.0f * atan2f(x, w);
    ret[1] = PI/2;
    ret[0] = 0.0f;
    ret[0] *= -180/PI;
    ret[1] *= -180/PI;
    //ret[1] -= 90;
    ret[2] *= -180/PI;
    return Quaternion2Matrix(Euler2Quaternion(cv::Mat(3,1,CV_32FC1,ret).clone())).clone();
  }
  if (test < -0.4999f) {
    ret[2] = 2.0f * atan2f(x, w);
    ret[1] = -PI/2;
    ret[0] = 0.0f;
    ret[0] *= -180/PI;
    ret[1] *= -180/PI;
    //ret[1] -= 90;
    ret[2] *= -180/PI;
    return Quaternion2Matrix(Euler2Quaternion(cv::Mat(3,1,CV_32FC1,ret).clone())).clone();
  }
  float sqx = x * x;
  float sqy = y * y;
  float sqz = z * z;
  ret[2] = atan2f(2.0f * y * w - 2.0f * x * z, 1.0f - 2.0f * sqy - 2.0f * sqz);
  ret[1] = asin(2.0f * test);
  ret[0] = atan2f(2.0f * x * w - 2.0f * z * y, 1.0f - 2.0f * sqx - 2.0f * sqz);
      
  ret[0] *= -180/PI;
  ret[1] *= -180/PI;
  //ret[0] -= 90/PI;
  //ret[1] -= 20;
  ret[0] +=80;
  ret[2] *= -180/PI;

  return Quaternion2Matrix(Euler2Quaternion(cv::Mat(3,1,CV_32FC1,ret).clone())).clone();
}
*/

cv::Mat Quaternion2Matrix (float x, float y, float z, float w)
{
  float xx = x*x;
  float yy = y*y;
  float zz = z*z;
  float xy = x*y;
  float wz = w*z;
  float wy = w*y;
  float xz = x*z;
  float yz = y*z;
  float wx = w*x;

  float ret[4][4];
  ret[0][0] = 1.0f-2*(yy+zz);
  ret[0][1] = 2*(xy-wz);
  ret[0][2] = 2*(wy+xz);
  ret[0][3] = 0.0f;
 
  ret[1][0] = 2*(xy+wz);
  ret[1][1] = 1.0f-2*(xx+zz);
  ret[1][2] = 2*(yz-wx);
  ret[1][3] = 0.0f;
 
  ret[2][0] = 2*(xz-wy);
  ret[2][1] = 2*(yz+wx);
  ret[2][2] = 1.0f-2*(xx+yy);
  ret[2][3] = 0.0f;
 
  ret[3][0] = 0.0f;
  ret[3][1] = 0.0f;
  ret[3][2] = 0.0f;
  ret[3][3] = 1.0f;
 
  return cv::Mat(4,4,CV_32FC1,ret).clone();
}

GLuint VBO;
GLuint IBO;
GLint PVMmatrix;

Matrix44f mmatrix,cmatrix,temp,mvp;

Matrix44f mtmatrix = {
    1,0,0,olx-clx,
    0,1,0,oly-cly,
    0,0,1,olz-clz,
    0,0,0,1
};

Matrix44f mrmatrix = {
    c1*c3+s1*s2*s3, c3*s1*s2-c1*s3,c2*s1,0,
    c2*s3,c2*c3,-s2,0,
    c1*s2*s3-s1*c3,s1*s3+c1*c3*s2,c1*c2,0,
    0,0,0,1
};
/*
Matrix44f mrmatrix = {
    cos(DegToRad(LEGO_ROTATION_EULER_R)),-sin(DegToRad(LEGO_ROTATION_EULER_R)),0,0,
    sin(DegToRad(LEGO_ROTATION_EULER_R)),cos(DegToRad(LEGO_ROTATION_EULER_R)),0,0,
    0,0,1,0,
    0,0,0,1
};
*/
Matrix44f msmatrix = {
    1.0/100,0,0,0,
    0,1.0/100,0,0,
    0,0,1.0/100,0,
    0,0,0,1
};

cv::Mat cr = Quaternion2Matrix(crx,cry,crz,crw);
Vector3f ct = {cr.at<float>(0,0),cr.at<float>(1,0),cr.at<float>(2,0)};
Vector3f cu = {0,0,1};
/*
Matrix44f pmatrix = {
    1.0f / (tanHalfFOV * ar), 0, 0, 0,
    0, 1.0f / tanHalfFOV, 0, 0,
    0, 0, (-zNear - zFar) / zRange, 2.0f*zFar*zNear / zRange,
    0, 0, 1, 0
};
*/
float fx=668.0703112366639;
float fy=668.0703112366639;
float cx=321.74;
float cy=238.02;

float maxx1,maxx2,maxy1,maxy2,maxz1,maxz2;
Matrix44f pmatrix = {
    2*fx/WINDOWWIDTH, (2*cx-WINDOWWIDTH)/WINDOWWIDTH, 0, 0,
    0, 2*fy/WINDOWHEIGHT, (2*cy-WINDOWHEIGHT)/WINDOWHEIGHT, 0,
    0, 0, (-zNear - zFar) / zRange, 2.0f*zFar*zNear / zRange,
    0, 0, 1, 0
};
Matrix44f testmatrix ={
    0.01,0,0,0,
    0,0.01,0,0,
    0,0,0.01,0,
    0,0,0,1
};
//shader
const char* pVSFileName = "/home/ncf/catkin/src/usb_cam/src/shader.vs";
const char* pFSFileName = "/home/ncf/catkin/src/usb_cam/src/shader.fs";

void grab()
{
	unsigned char *mpixels = new unsigned char[WINDOWWIDTH * WINDOWHEIGHT * 3];
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, WINDOWWIDTH, WINDOWHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, mpixels);
    glReadBuffer(GL_BACK);
    for(int i = 0; i < (int)WINDOWWIDTH*WINDOWHEIGHT*3; i += 3)
    {   
        mpixels[i] ^= mpixels[i+2] ^= mpixels[i] ^= mpixels[i+2];
    }
    FIBITMAP* bitmap = FreeImage_Allocate(WINDOWWIDTH, WINDOWHEIGHT, 24, 8, 8, 8);
 
    for(int y = 0 ; y < FreeImage_GetHeight(bitmap); y++)
    {
        BYTE *bits = FreeImage_GetScanLine(bitmap, y);
        for(int x = 0 ; x < FreeImage_GetWidth(bitmap); x++)
        {
            bits[0] = mpixels[(y*WINDOWWIDTH+x) * 3 + 0];
            bits[1] = mpixels[(y*WINDOWWIDTH+x) * 3 + 1];
            bits[2] = mpixels[(y*WINDOWWIDTH+x) * 3 + 2];
            bits += 3;
        }
 
    }
 
	FreeImage_Save(FIF_JPEG, bitmap, "/home/ncf/ROS/test.jpg", JPEG_DEFAULT);
 
    FreeImage_Unload(bitmap);
}

bool ReadShader(const char* pFileName, std::string &outFile)
{
    std::ifstream f(pFileName);

    bool ret = false;

    if (f.is_open()) {
        std::string line;
        while (getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }
        f.close();
        ret = true;
    }
    else {
        fprintf(stderr, "%s:%d: unable to open file `%s`\n", __FILE__,__LINE__,pFileName);
    }
    return ret;
}

struct point {
	int no;
	float X;
	float Y;
	float Z;

};

struct tri {
	int p[3];
};

std::vector<point> vp;
int push_vp(point p)//查找顶点是否已经存在，如果不存在则将顶点压栈，返回顶点编号
{

	point tmp_p;
	int i = -1;
	for (i = 0; i < (int)vp.size(); i++) {
		if (p.X == vp[i].X &&p.Y == vp[i].Y &&p.Z == vp[i].Z) { //找到顶点了
			break;
		}
	}
	if (i == (int)vp.size()) { //没找到 压栈
		tmp_p.X = p.X;
		tmp_p.Y = p.Y;
		tmp_p.Z = p.Z;
		tmp_p.no = i;
		vp.push_back(tmp_p);
	}
	return(vp[i].no);
}

int readfile(char const *fname)
{
	FILE* f;
	int errno1 = fopen_s(&f, fname, "rb");  //定义文件名
	char buff[100];
	std::vector<tri> v_tri;//三角形点向量 存放点索引
	point tp;
	tri tt;
	fread(buff, 1, 80, f);
	printf("model name:%s\n", buff);
	int unTriangles;
	fread((char*)&unTriangles, sizeof(int), 1, f);
	printf("triangle counts:%d\n", unTriangles);
	if (unTriangles == 0) {
		printf("wrong triangle counts\n");
	}
	else {
		for (int i = 0; i < unTriangles; i++)
		{
			float coorXYZ[12];
			fread((char*)coorXYZ, sizeof(float), 12, f);//3f 法矢量  p1 p2 p3
			for (int j = 1; j < 4; j++)
			{
				tp.no = 0;
				tp.X = coorXYZ[j * 3];
				tp.Y = coorXYZ[j * 3 + 1];
				tp.Z = coorXYZ[j * 3 + 2];
				tp.no = push_vp(tp);
				tt.p[j - 1] = tp.no;
			}
			v_tri.push_back(tt);
			fread((char*)coorXYZ, 1, 2, f);//2字节属性
		}
	}
	fclose(f);
    maxx1 = vp[0].X;
    maxx2 = maxx1;
    maxy1 = vp[0].Y;
    maxy2 = maxy1;
    maxz1 = vp[0].Z;
    maxz2 = maxz1;

	for (int i = 0; i < (int)vp.size(); i++) {
        LoadVector3(Vertices[i], vp[i].X, vp[i].Y, vp[i].Z);
        if(maxx1<vp[i].X)
        maxx1 =vp[i].X;
        if(maxx2>vp[i].X)
        maxx2 =vp[i].X;
        if(maxx1<vp[i].Y)
        maxy1 =vp[i].Y;
        if(maxx2>vp[i].Y)
        maxy2 =vp[i].Y;
        if(maxx1<vp[i].Z)
        maxz1 =vp[i].Z;
        if(maxx2>vp[i].Z)
        maxz2 =vp[i].Z;
		//Vertices[i] = Vector3f(vp[i].X,vp[i].Y,vp[i].Z);
		//printf("%d\t%f\t%f\t%f\t\n", i, Vertices[i][0], Vertices[i][1], Vertices[i][2]);
	}
	//printf("Indices:\n");
	for (int i = 0; i < (int)v_tri.size(); i++) {
		Indices[i * 3] = v_tri[i].p[0];
		Indices[i * 3 + 1] = v_tri[i].p[1];
		Indices[i * 3 + 2] = v_tri[i].p[2];
		//printf("%d\t%d\t%d\n", Indices[i * 3], Indices[i * 3+1], Indices[i * 3+2]);
	}
    indices_count = 3 * (int)v_tri.size();
    vertices_count = (int)vp.size();
    printf("%s","Model loaded\n");
    printf("%f %f %f %f %f %f",maxx1,maxx2,maxy1,maxy2,maxz1,maxz2);
	return(0);

}
static void findmodelboarder(){
    	for (int i = 0; i < (int)vp.size(); i++) {
        if (vp[i].X > maxx1) maxx1 = vp[i].X;
        if (vp[i].X < maxx2) maxx2 = vp[i].X;
        if (vp[i].Y > maxy1) maxy1 = vp[i].Y;
        if (vp[i].Y < maxy2) maxy2 = vp[i].Y;
        if (vp[i].Z > maxz1) maxz1 = vp[i].Z;
        if (vp[i].Z < maxz2) maxz2 = vp[i].Z;
        }
        printf("max%f,minx%f,maxy%f,miny%f,maxz%f,minz%f",maxx1,maxx2,maxy1,maxy2,maxz1,maxz2);
}
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);
    //check if it is successful
    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    //define shader code source
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    //Compiler shader object
    glCompileShader(ShaderObj);

    //check the error about shader
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }
    //bound the shader object to shader program
    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompilerShaders()
{
    //We start the process of setting up our shaders by creating a program object. 
    //We will link all the shaders together into this object.
    GLuint ShaderProgram = glCreateProgram();

    //Check yes or not success
    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    //the buffer of shader texts
    std::string vs, fs;
    //read the text of shader texts to buffer
    if (!ReadShader(pVSFileName, vs)) {
        exit(1);
    }
    if (!ReadShader(pFSFileName, fs)) {
        exit(1);
    }

    //add vertex shader and fragment shader
    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    //Link the shader program, and check the error
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };
    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram,GL_LINK_STATUS,&Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    //check if it can be execute
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    //use program
    glUseProgram(ShaderProgram);

    //uniform locate
    PVMmatrix = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(PVMmatrix != 0xFFFFFFFF);


}

static void CreateVertexBuffer()
{
    // 创建含有一个顶点的顶点数组
    // 将点置于屏幕中央
    /*
    Vector3f Vertices[8];
    LoadVector3(Vertices[0],  8.772f,3.124f, -2.997f);
    
    LoadVector3(Vertices[1],  8.7717f,-3.076f, -2.997f);
    LoadVector3(Vertices[2],  -4.832f,-3.076f, -2.997f);
    LoadVector3(Vertices[3],  -4.832f,3.124f, -2.997f);
    LoadVector3(Vertices[4],  8.772f,3.124f, 3.069f);
    LoadVector3(Vertices[5],  8.772f,-3.076f, 3.069f);
    LoadVector3(Vertices[6],  -4.832f,-3.076f, 3.069f);
    LoadVector3(Vertices[7],  -4.832f,3.124f, 3.069f);*/
    /*
    LoadVector3(Vertices[0], 3.1f, 4.8f, -3.1f);
    3.2 9.4
    LoadVector3(Vertices[1], -3.1f, 4.8f, -3.1f);
    LoadVector3(Vertices[2], -3.1f, -4.8f, -3.1f);
    LoadVector3(Vertices[3], 3.1f, -4.8f, -3.1f);
    LoadVector3(Vertices[4], 3.1f, 4.8f, 3.1f);
    LoadVector3(Vertices[5], -3.1f, 4.8f, 3.1f);
    LoadVector3(Vertices[6], -3.1f, -4.8f, 3.1f);
    LoadVector3(Vertices[7], 3.1f, -4.8f, 3.1f);
*/
    // 创建缓冲器
    glGenBuffers(1, &VBO);
    // 绑定GL_ARRAY_BUFFER缓冲器
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 绑定顶点数据
    glBufferData(GL_ARRAY_BUFFER, vertices_count*12, Vertices, GL_STATIC_DRAW);
    //printf("vertices=%d\n", sizeof(Vertices));
}
static void CreateIndexBuffer()
{
/*
    GLuint Indices[] = {0,1,2,
                        0,2,3,
                        0,1,5,
                        0,4,5,
                        0,3,4,
                        3,4,7,
                        2,7,3,
                        2,7,6,
                        1,6,5,
                        1,6,2,
                        4,6,5,
                        4,6,7};*/
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * indices_count, Indices, GL_STATIC_DRAW); 
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*4, Indices, GL_STATIC_DRAW); 
    //printf("indices count = %d", indices_count);
}

void Render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    //matrix calculation
    CameraMatrix44(cmatrix, ct, cu);
    MatrixMultiply44(temp, mrmatrix, msmatrix);
    MatrixMultiply44(mmatrix, mtmatrix, temp);
    MatrixMultiply44(temp, cmatrix, mmatrix);
    MatrixMultiply44(mvp, pmatrix, temp);
    //printf("%f,%f,%f",ct[0],ct[1],ct[2]);
    glUniformMatrix4fv(PVMmatrix, 1, GL_TRUE, &mvp[0]); // T: horizontal / F: vertical
    for (int i =0; i <16; i++) printf("%f\n",mvp[i]) ;
    
    /*printf("%f,%f,%f",
    40*mvp[0]+50*mvp[1]-20*mvp[2]+mvp[3])*(40*mvp[12]+50*mvp[13]-20*mvp[14]+mvp[15])+1)/2*WINDOWWIDTH,
    40*mvp[4]+50*mvp[5]-20*mvp[6]+mvp[7])*(40*mvp[12]+50*mvp[13]-20*mvp[14]+mvp[15])+1)/2*WINDOWHEIGHT

    );*/
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_POINTS, 0, 8);
    glDisableVertexAttribArray(0);
    // 交换前后缓存
    glutSwapBuffers();
    grab();
}

//Glut callback

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(Render);
}

//Main program
int main(int argc, char **argv)
{   
    int i = readfile("/home/ncf/ROS/LEGOCbinary.stl");
    //findmodelboarder();
    glutInit(&argc, argv);
    //Simple buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB ); //buffercount, color mode, buffer type
    glutInitWindowPosition(0,0);
    glutInitWindowSize(WINDOWWIDTH,WINDOWHEIGHT);

    glutCreateWindow("Camera Capture");
    //Call to the drawing function
    InitializeGlutCallbacks();
    
    

    GLenum res = glewInit();
    if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
    }

    printf("GL version: %s\n", glGetString(GL_VERSION));
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    CreateVertexBuffer();
    CreateIndexBuffer();
    CompilerShaders();
    

    glutMainLoop();
    return 0;
}