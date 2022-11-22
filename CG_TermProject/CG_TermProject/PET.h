#pragma once
#include "pch.h"

class PET
{
	// ��ǥ, �ٿ���ڽ�
	GLfloat x;
	GLfloat y;
	GLfloat z;
	RECT rect;

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
	void init(int R, int G, int B, string str); //--- �ʱ�ȭ(ReadObj, initBuffer)

	void scaleTransform(float x, float y, float z); //--- ������ ��ȯ
	void rotateTransform(float x, float y, float z, float degree); //--- ȸ�� ��ȯ
	void moveTranform(float x, float y, float z); //--- �̵� ��ȯ
	RECT get_BB(float x, float z);

	void update();
	void draw();

	void changeColor(int R, int G, int B); // �� ���� (0 ~ 255)

public:
	PET();
	~PET();
};

