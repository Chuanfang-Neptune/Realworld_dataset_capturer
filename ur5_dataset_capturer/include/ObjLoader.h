#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

class ObjLoader{
public:
	ObjLoader(string filename);//Constructor
	void Draw();//function drow
private:
	vector<vector<GLfloat>>vSets;//Store vertex (x, y, z) coordinates
	vector<vector<GLint>>fSets;//Three vertex indexes of a face
};

ObjLoader::ObjLoader(string filename)
{
	string line;
	fstream f;
	f.open(filename, ios::in);
	if (!f.is_open()){
		cout << "Something Went Wrong When Opening Objfiles" << endl;
	}

	while (!f.eof()){
		getline(f, line);   //get one line of the files ,store into a string variable
		vector<string>parameters;
		string tailMark = " ";
		string ans = "";

		line = line.append(tailMark);
		for (int i = 0; i < line.length(); i++) {
			char ch = line[i];
			if (ch != ' ') {
				ans+=ch;
			}
			else {
				parameters.push_back(ans); //get the elements in the string and slice them with spaces
				ans = "";
			}
		}
		cout << parameters.size() << endl;
		if (parameters.size() != 4) {
			cout << "the size is not correct" << endl;
		}
		else {
			if (parameters[0] == "v") {   //vertex
				vector<GLfloat>Point;
				for (int i = 1; i < 4; i++) {   //Starting from 1, put the three XYZ coordinates of the vertex into the vertex vector
					GLfloat xyz = atof(parameters[i].c_str());
					Point.push_back(xyz);
				}
				vSets.push_back(Point);
			}

			else if (parameters[0] == "f") {   //face 果是面的话，存放三个顶点的索引
				vector<GLint>vIndexSets;
				for (int i = 1; i < 4; i++){
					string x = parameters[i];
					string ans = "";
					for (int j = 0; j < x.length(); j++) {   
						char ch = x[j];
						if (ch != '/') { 	// skip ‘/’
							ans += ch;
						}
						else {
							break;
						}
					}
					GLint index = atof(ans.c_str());
					index = index--;//index starts at 1, Variable array starts at 0,should decrease by 1
					vIndexSets.push_back(index);
				}
				fSets.push_back(vIndexSets);
			}
		}
	}
	f.close();
}

void ObjLoader::Draw(){

	glBegin(GL_TRIANGLES);//begin draw
	for (int i = 0; i < fSets.size(); i++) {
		GLfloat VN[3];
		//3 vertexs
		GLfloat SV1[3];
		GLfloat SV2[3];
		GLfloat SV3[3];

		if ((fSets[i]).size() != 3) {
			cout << "the fSetsets_Size is not correct" << endl;
		}
		else {
				GLint firstVertexIndex = (fSets[i])[0];//get index of vertex
				GLint secondVertexIndex = (fSets[i])[1];
				GLint thirdVertexIndex = (fSets[i])[2];

				SV1[0] = (vSets[firstVertexIndex])[0];//first vertex
				SV1[1] = (vSets[firstVertexIndex])[1];
				SV1[2] = (vSets[firstVertexIndex])[2];

				SV2[0] = (vSets[secondVertexIndex])[0]; //second vertex
				SV2[1] = (vSets[secondVertexIndex])[1];
				SV2[2] = (vSets[secondVertexIndex])[2];

				SV3[0] = (vSets[thirdVertexIndex])[0]; //third vertex
				SV3[1] = (vSets[thirdVertexIndex])[1];
				SV3[2] = (vSets[thirdVertexIndex])[2];

				//Calculate normal vector
				GLfloat vec1[3], vec2[3], vec3[3];
				//(x2-x1,y2-y1,z2-z1)
				vec1[0] = SV1[0] - SV2[0];
				vec1[1] = SV1[1] - SV2[1];
				vec1[2] = SV1[2] - SV2[2];

				//(x3-x2,y3-y2,z3-z2)
				vec2[0] = SV1[0] - SV3[0];
				vec2[1] = SV1[1] - SV3[1];
				vec2[2] = SV1[2] - SV3[2];

				//(x3-x1,y3-y1,z3-z1)
				vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
				vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
				vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

				GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

				VN[0] = vec3[0] / D;
				VN[1] = vec3[1] / D;
				VN[2] = vec3[2] / D;
           
				glNormal3f(VN[0], VN[1], VN[2]);//draw normal vector

				glVertex3f(SV1[0], SV1[1], SV1[2]);//draw triangle face
				glVertex3f(SV2[0], SV2[1], SV2[2]);
				glVertex3f(SV3[0], SV3[1], SV3[2]);	
		}
	}
	glEnd();
}

#endif