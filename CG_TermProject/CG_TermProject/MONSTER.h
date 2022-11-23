#pragma once
#include "pch.h"

class GHOST
{
private:
	// ��ǥ, �ٿ���ڽ�
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

	// ������, ȸ��, �̵�
	glm::mat4 scale;  //--- ������ ���
	glm::mat4 rotate; //--- ȸ�� ��� 
	glm::mat4 move;   //--- �̵� ���
	glm::mat4 merge;  //--- �ռ� ��ȯ ���
	vector<glm::mat4> vec_transform;


public:
	void init(int R, int G, int B, string str,float pos_x, float pos_z); //--- �ʱ�ȭ(ReadObj, initBuffer)

	void scaleTransform(float x, float y, float z); //--- ������ ��ȯ
	void rotateTransform(float x, float y, float z, float degree); //--- ȸ�� ��ȯ
	void moveTranform(float x, float y, float z); //--- �̵� ��ȯ
	RECT get_BB();

	void update();
	void draw();

	void changeColor(int R, int G, int B); // �� ���� (0 ~ 255)

	//ù��° AI��ƾ
	bool first_ghost_down{ TRUE };
	bool first_ghost_left{ FALSE };
	bool first_ghost_up{ FALSE };
	bool first_ghost_right{ FALSE };
	void AI_FIRST();
	//�ι�° AI��ƾ
	bool second_ghost_down{ TRUE };
	bool second_ghost_left{ FALSE };
	bool second_ghost_up{ FALSE };
	bool second_ghost_right{ FALSE };
	void AI_SECOND();
	//����° AI��ƾ
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

