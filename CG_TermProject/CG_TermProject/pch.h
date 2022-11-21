#pragma once
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <random>
#include <math.h>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtx/rotate_vector.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

using namespace std;
extern GLuint shaderID;

void ReadObj(string file, vector<glm::vec3>* vertexData, vector<glm::vec3>* colorData, vector<glm::ivec3>* faceData); //--- 오브젝트 읽어오기
void initBuffer(GLuint* VAO, GLuint* VBO, GLuint* EBO, vector<glm::vec3> vertexData, vector<glm::vec3> colorData, vector<glm::ivec3> faceData); //--- 오브젝트 정보 저장하기
