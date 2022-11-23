#pragma once
#include "pch.h"

class MAP
{
private:
	// 좌표, 바운딩박스
	GLfloat pos_x;
	GLfloat pos_z;

	RECT rect;
	// VAO, VBO, EBO
	GLuint VAO, VBO[2], EBO;

	// vertex, color, face
	vector<glm::vec3>  vertexData;
	vector<glm::vec3>  colorData;
	vector<glm::ivec3> faceData;

	// 스케일, 회전, 이동
	glm::mat4 scale;  //--- 스케일 행렬
	glm::mat4 rotate; //--- 회전 행렬 
	glm::mat4 move;   //--- 이동 행렬
	glm::mat4 merge;  //--- 합성 변환 행렬
	vector<glm::mat4> vec_transform;


public:
	void init(int R, int G, int B); //--- 초기화(ReadObj, initBuffer)
	void scaleTransform(float x, float y, float z); //--- 스케일 변환
	void rotateTransform(float x, float y, float z, float degree); //--- 회전 변환
	void moveTranform(float x, float y, float z); //--- 이동 변환
	void update();
	void draw();
	RECT get_bb();

	void changeColor(int R, int G, int B); // 색 변경 (0 ~ 255)
public:
	MAP();
	~MAP();
};

void InitMap(MAP map[327]);