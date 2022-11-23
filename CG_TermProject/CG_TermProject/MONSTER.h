#pragma once
#include "pch.h"

class GHOST
{
private:
	// 좌표, 바운딩박스
	GLfloat x;
	GLfloat z;

	GLfloat dx=0;
	GLfloat dz=0;

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
	void init(int R, int G, int B, string str,float pos_x, float pos_z); //--- 초기화(ReadObj, initBuffer)

	void scaleTransform(float x, float y, float z); //--- 스케일 변환
	void rotateTransform(float x, float y, float z, float degree); //--- 회전 변환
	void moveTranform(float x, float y, float z); //--- 이동 변환
	RECT get_BB();

	void update();
	void draw();

	void changeColor(int R, int G, int B); // 색 변경 (0 ~ 255)

	//첫번째 AI루틴
	bool first_ghost_down{ TRUE };
	bool first_ghost_left{ FALSE };
	bool first_ghost_up{ FALSE };
	bool first_ghost_right{ FALSE };
	void AI_FIRST();
	//두번째 AI루틴
	bool second_ghost_down{ TRUE };
	bool second_ghost_left{ FALSE };
	bool second_ghost_up{ FALSE };
	bool second_ghost_right{ FALSE };
	void AI_SECOND();
	//세번째 AI루틴
	bool third_ghost_down{ TRUE };
	bool third_ghost_down2{ FALSE };
	bool third_ghost_left{ FALSE };
	bool third_ghost_up{ FALSE };
	bool third_ghost_up2{ FALSE };
	bool third_ghost_right{ FALSE };
	void AI_THIRD();
public:
	GHOST();
	~GHOST();
};



void InitGhost(GHOST& body, GHOST& eye, float pos_x, float pos_z);

