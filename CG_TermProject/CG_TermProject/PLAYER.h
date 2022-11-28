#pragma once
#include "pch.h"

class PLAYER
{
	// ��ǥ, �ٿ���ڽ�
	GLfloat x;
	GLfloat y;
	GLfloat z;
	RECT rect;

	// VAO, VBO, EBO
	//GLuint VAO, VBO[2], EBO;
	GLuint VAO, VBO;
	// vertex, color, face
	//vector<glm::vec3>  vertexData;
	//vector<glm::vec3>  colorData;
	//vector<glm::ivec3> faceData;
	vector<glm::vec3> vertexInfo;
	// ������, ȸ��, �̵�
	glm::mat4 scale;  //--- ������ ���
	glm::mat4 rotate; //--- ȸ�� ��� 
	glm::mat4 move;   //--- �̵� ���
	glm::mat4 merge;  //--- �ռ� ��ȯ ���
	vector<glm::mat4> vec_transform;


public:
	void init( string str); //--- �ʱ�ȭ(ReadObj, initBuffer)

	void scaleTransform(float x, float y, float z); //--- ������ ��ȯ
	void rotateTransform(float x, float y, float z, float degree); //--- ȸ�� ��ȯ
	void moveTranform(float x, float y, float z); //--- �̵� ��ȯ
	RECT get_BB(float x, float z);

	void update();
	void draw(int R, int G, int B);

	void changeColor(int R, int G, int B); // �� ���� (0 ~ 255)

public:
	PLAYER();
	~PLAYER();
};

 //�÷��̾� ��ü (����, �Ʒ���, ��, ��)
extern PLAYER pacmanTop;
extern PLAYER pacmanBot;
extern PLAYER pacmanEyes;
extern PLAYER pacmanNose;
extern GLboolean plusDeg;
extern GLfloat   deg;


GLvoid initPlayer();
GLvoid redrawPlayer();
GLvoid animatePlayer();