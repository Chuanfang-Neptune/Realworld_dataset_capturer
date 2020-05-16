#include "glfunc.h"

Vector3f Vertices[MAX_VERTICES];    //Vertex coordinates
unsigned int Indices[MAX_INDICES];
int vertices_count = 0;
int indices_count = 0;
GLuint VBO;
GLuint IBO;
GLint PVMmatrix;
std::vector<point> vp;


//==================================================================
//function name：  ReadShader
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function： open shader file and read content into string variable
 //input parameters：
 //          pFileName shader file's path 
 //          outFile  string  shader content
 //return：  type（boolean)
 //          true : read shader file success
 //          false : read shader file failure
 //modify：
 //
//==================================================================
bool ReadShader(const char *pFileName, std::string &outFile) 
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
  } else {
    fprintf(stderr, "%s:%d: unable to open file `%s`\n", __FILE__, __LINE__, pFileName);
  }
  return ret;
}

//==================================================================
//function name：  push_vp
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function： push point p into vector vp when not found it in vp
 //input parameters：
 //          p point should be pushed 
 //          vp vecter of points
 //return：  type（int)
 //          return number of point p in vector vp
 //modify：
 //
//==================================================================
int push_vp(point p) 
{
  point tmp_p;
  int i = -1;
  for (i = 0; i < (int)vp.size(); i++) {
    if (p.X == vp[i].X && p.Y == vp[i].Y && p.Z == vp[i].Z) { 
      //found,break loop 
      break;
    }
  }
  if (i == (int)vp.size()) { 
    //not found ,push p into vp
    tmp_p.X = p.X;
    tmp_p.Y = p.Y;
    tmp_p.Z = p.Z;
    tmp_p.no = i;
    vp.push_back(tmp_p);
  }
  return (vp[i].no);
}

//==================================================================
//function name：  readstl
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function： open STL file and read content into vertex Indices
 //           all points are stored into vector and marked by index number
 //           all triangles are stored into vector 
 //           triangle points are remaped by point's index number
 //           calculate object's geometric information
 //input parameters：
 //          fname STL file's path 
 //return：  type（lego_geo)
 //          LEGO Car's geometric information 
 //modify：
 //
//==================================================================
lego_geo readstl(char const *fname) {
  FILE *f;
  lego_geo lg;
  char buff[100];
  std::vector<tri> v_tri; //vecter of triangles
  point tp;
  tri tt;
  int unTriangles;

  //open STL file 
  int errno1 = fopen_s(&f, fname, "rb"); 
 //read STL file
  //first line : model's name
  fread(buff, 1, 80, f);
  // read triangle counts
  fread((char *)&unTriangles, sizeof(int), 1, f);
  printf("triangle counts:%d\n", unTriangles);
  if (unTriangles == 0) {
    printf("wrong triangle counts\n");
  } else {
    for (int i = 0; i < unTriangles; i++) {
      float coorXYZ[12];
      //triangle consists of 12 floats and 2 bytes property
      fread((char *)coorXYZ, sizeof(float), 12, f); 
      // struct:3floats Normal vector, 3floats point1,3floats point2,3floats point3
      for (int j = 1; j < 4; j++) {
        tp.no = 0;
        tp.X = coorXYZ[j * 3];
        tp.Y = coorXYZ[j * 3 + 1];
        tp.Z = coorXYZ[j * 3 + 2];
        tp.no = push_vp(tp); //store this point into vp and get index number in vp
        tt.p[j - 1] = tp.no;
      }
      v_tri.push_back(tt); //store triangle tt into vecter v_tri
      fread((char *)coorXYZ, 1, 2, f); // 2bytes property
    }
  }
  fclose(f);
  
 //calculate LEGO Car's geometric infomation 
 // box  maxx minx maxy miny maxz minz  
  lg.maxx = vp[0].X;
  lg.minx = lg.maxx;
  lg.maxy = vp[0].Y;
  lg.miny = lg.maxy;
  lg.maxz = vp[0].Z;
  lg.minz = lg.maxz;
  for (int i = 0; i < (int)vp.size(); i++) {
    LoadVector3(Vertices[i], vp[i].X, vp[i].Y, vp[i].Z);
    if (lg.maxx < vp[i].X)
      lg.maxx = vp[i].X;
    if (lg.minx > vp[i].X)
      lg.minx = vp[i].X;
    if (lg.maxy < vp[i].Y)
      lg.maxy = vp[i].Y;
    if (lg.miny > vp[i].Y)
      lg.miny = vp[i].Y;
    if (lg.maxz < vp[i].Z)
      lg.maxz = vp[i].Z;
    if (lg.minz > vp[i].Z)
      lg.minz = vp[i].Z;
  }
 // geometric center  
  lg.x = (lg.maxx - lg.minx) / 2;
  lg.y = (lg.maxy - lg.miny) / 2;
  lg.z = (lg.maxz - lg.minz) / 2;

  // printf("x=%f,y=%f,z=%f\n",legox,legoy,legoz);
  // printf("minx=%f,maxx=%f,miny=%f,maxy=%f,minz=%f,maxz=%f\n",minx,maxx,miny,maxy,minz,maxz);
  // printf("Indices:\n");
 //calculate indices an vertices
  for (int i = 0; i < (int)v_tri.size(); i++) {
    Indices[i * 3] = v_tri[i].p[0];
    Indices[i * 3 + 1] = v_tri[i].p[1];
    Indices[i * 3 + 2] = v_tri[i].p[2];
    // printf("%d\t%d\t%d\n", Indices[i * 3], Indices[i * 3+1], Indices[i * 3+2]);
  }
  indices_count = 3 * (int)v_tri.size();
  vertices_count = (int)vp.size();
 // 
  printf("%s", "Model loaded\n");
  return (lg);
}

//==================================================================
//function name：  AddShader
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function：  
 //            
 //            
 //            
 //            
 //input parameters：
 //           
 //return：  type（void)
 //          
 //modify：
 //
//==================================================================
static void AddShader(GLuint ShaderProgram, const char *pShaderText, GLenum ShaderType)
{
  GLuint ShaderObj = glCreateShader(ShaderType);
  // check if it is successful
  if (ShaderObj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", ShaderType);
    exit(0);
  }

  // define shader code source
  const GLchar *p[1];
  p[0] = pShaderText;
  GLint Lengths[1];
  Lengths[0] = strlen(pShaderText);
  glShaderSource(ShaderObj, 1, p, Lengths);
  // Compiler shader object
  glCompileShader(ShaderObj);

  // check the error about shader
  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
    exit(1);
  }
  // bound the shader object to shader program
  glAttachShader(ShaderProgram, ShaderObj);
}


//==================================================================
//function name：  CompilerShaders
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function：  
 //            
 //            
 //            
 //            
 //input parameters：
 //           
 //return：  type（void)
 //          
 //modify：
 //
//==================================================================
void CompilerShaders() 
{
  // We start the process of setting up our shaders by creating a program object.
  // We will link all the shaders together into this object.
  GLuint ShaderProgram = glCreateProgram();

  // Check yes or not success
  if (ShaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
    exit(1);
  }

  // the buffer of shader texts
  std::string vs, fs;
  // read the text of shader texts to buffer
<<<<<<< HEAD
  if (!ReadShader(pVSFileName.c_str(), vs)) {
    exit(1);
  }
  if (!ReadShader(pFSFileName.c_str(), fs)) {
=======
  if (!ReadShader(pVSFileName, vs)) {
    exit(1);
  }
  if (!ReadShader(pFSFileName, fs)) {
>>>>>>> 28e165c9eec863ea1387fb921c9ce7195f8f4339
    exit(1);
  }

  // add vertex shader and fragment shader
  AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
  AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

  // Link the shader program, and check the error
  GLint Success = 0;
  GLchar ErrorLog[1024] = {0};
  glLinkProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  // check if it can be execute
  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    exit(1);
  }

  // use program
  glUseProgram(ShaderProgram);

  // uniform locate
  PVMmatrix = glGetUniformLocation(ShaderProgram, "gWorld");
  assert(PVMmatrix != 0xFFFFFFFF);
}

//==================================================================
//function name：  CreateVertexBuffer
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function：  
 //            
 //            
 //            
 //            
 //input parameters：
 //           
 //return：  type（void)
 //          
 //modify：
 //
//==================================================================
void CreateVertexBuffer() 
{
  // Create an array of vertices with one vertex
  // Center point on screen
  // generate buffer
  glGenBuffers(1, &VBO);
  // bind GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // bind vertex buffer
  glBufferData(GL_ARRAY_BUFFER, vertices_count * 12, Vertices, GL_STATIC_DRAW);
  // printf("vertices=%d\n", sizeof(Vertices));
}


//==================================================================
//function name：  CreateIndexBuffer
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function：  
 //            
 //            
 //            
 //            
 //input parameters：
 //           
 //return：  type（void)
 //          
 //modify：
 //
//==================================================================
void CreateIndexBuffer() 
{
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * indices_count, Indices, GL_STATIC_DRAW);
}

//==================================================================
//function name：  grab
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function：  
 //            
 //            
 //            
 //            
 //input parameters：
 //           
 //return：  type（void)
 //          
 //modify：
 //
//==================================================================
void grab(std::string fn) 
{
  unsigned char *mpixels = new unsigned char[WINDOWWIDTH * WINDOWHEIGHT * 3];
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, WINDOWWIDTH, WINDOWHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, mpixels);
  glReadBuffer(GL_BACK);
  for (int i = 0; i < (int)WINDOWWIDTH * WINDOWHEIGHT * 3; i += 3) {
    mpixels[i] ^= mpixels[i + 2] ^= mpixels[i] ^= mpixels[i + 2];
  }
  FIBITMAP *bitmap = FreeImage_Allocate(WINDOWWIDTH, WINDOWHEIGHT, 24, 8, 8, 8);

  for (int y = 0; y < FreeImage_GetHeight(bitmap); y++) {
    BYTE *bits = FreeImage_GetScanLine(bitmap, y);
    for (int x = 0; x < FreeImage_GetWidth(bitmap); x++) {
      bits[0] = mpixels[(y * WINDOWWIDTH + x) * 3 + 0];
      bits[1] = mpixels[(y * WINDOWWIDTH + x) * 3 + 1];
      bits[2] = mpixels[(y * WINDOWWIDTH + x) * 3 + 2];
      bits += 3;
    }
  }

  FreeImage_Save(FIF_JPEG, bitmap, fn.c_str(), JPEG_DEFAULT);
  FreeImage_Unload(bitmap);
}


//==================================================================
//function name：  Render
 //author： Ning Chuanfang
 //date： 2020-04-22
 //function：  
 //            
 //            
 //            
 //            
 //input parameters：
 //           
 //return：  type（void)
 //          
 //modify：
 //
//==================================================================
void Render() 
{
  glClear(GL_COLOR_BUFFER_BIT);
  glUniformMatrix4fv(PVMmatrix, 1, GL_TRUE, &mvp[0]); // T: horizontal / F: vertical
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
  glDisableVertexAttribArray(0);
  // swap buffer
  glutSwapBuffers();
  grab(filename1s);
}
