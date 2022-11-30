#include "pch.h"
#include "PLAYER.h"
#include "MONSTER.h"
#include "GEM.h"
#include "PET.h"
#include "MAP.h"
#include "CAMERA.h"
#include "shader.h"

// �ݹ� �Լ���
GLvoid Reshape(int w, int h);
GLvoid Render();
GLvoid KeyboardDown(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

// ������ ���� ����
GLfloat winWidth, winHeight;

// ���̴� ���� �Լ�, ����

GLuint shaderID; //--- ���̴� ���α׷� �̸�

// lighting
glm::vec3 lightPos(10.0, 100.0, -3.0);

//�Է��Լ���
void mouse_passive(int x, int y); //1��Ī ������ ���
void sp_keybord(int key, int x, int y); //1��Ī ������ �̵�

// �÷��̾� ��ü (����, �Ʒ���, ��, ��)
PLAYER pacmanTop;
PLAYER pacmanBot;
PLAYER pacmanEyes;
PLAYER pacmanNose;

//��Ʈ ��ü 5���� ����
GHOST first_ghost_body;
GHOST first_ghost_eye;
GHOST second_ghost_body;
GHOST second_ghost_eye;
GHOST third_ghost_body;
GHOST third_ghost_eye;

// ��
MAP map[327]{};

// ī�޶�
CAMERA camera;
glm::vec3 cameraPos = glm::vec3(250., 10.f, -250.);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	winWidth = 1600.f;
	winHeight = 900.f;
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, -500);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("CG_TermProject");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	Shader shader("new_vertex.glsl", "new_fragment.glsl");
	shaderID = shader.ID;
	glUseProgram(shaderID);

	shader.setVec3("lightPos", lightPos);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	//make_vertexShader(); //--- ���ؽ� ���̴� �����
	//make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����
	//shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	//glUseProgram(shaderID);


	glEnable(GL_DEPTH_TEST);    // ���� ����
	glEnable(GL_DITHER);        // ǥ���� �Ų�����
	glEnable(GL_CULL_FACE);     // �ø�
	glEnable(GL_LINE_SMOOTH);   // ��Ƽ �ٸ����
	glEnable(GL_POLYGON_SMOOTH);// ��Ƽ �ٸ����
	glShadeModel(GL_SMOOTH);    // �ε巯�� ������ �����մϴ�.

	initPlayer();

	// ���ɰ�ü Init
	InitGhost(first_ghost_body, first_ghost_eye, 250., -270.);
	InitGhost(second_ghost_body, second_ghost_eye, -110., -70.);
	InitGhost(third_ghost_body, third_ghost_eye, -50, -270);
	//�� Init
	InitMap(map);

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutPassiveMotionFunc(mouse_passive);
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(sp_keybord);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}
bool firstMouse{ TRUE };
float yaw{};
float pitch{};
float lastX = 800, lastY = 400;

void mouse_passive(int x, int y)
{
	//if (firstMouse)
	//{
	//	lastX = x;
	//	lastY = y;
	//	firstMouse = false;
	//}

	//float xoffset = x - lastX;
	//float yoffset = lastY - y;
	//lastX = x;
	//lastY = y;

	//float sensitivity = 0.5;
	//xoffset *= sensitivity;
	//yoffset *= sensitivity;

	//yaw += xoffset;
	//pitch += yoffset;

	//if (pitch > 10.0f)
	//	pitch = 10.0f;
	//if (pitch < -10.0f)
	//	pitch = -10.0f;

	//glm::vec3 front;
	//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));
	//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//cameraFront = glm::normalize(front);
	float xpos = static_cast<float>(x);
	float ypos = static_cast<float>(y);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

<<<<<<< HEAD
	lastX = xpos;
	lastY = ypos;
=======
	float sensitivity = 0.5;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

<<<<<<< Updated upstream
	if (pitch > 10.0f)
		pitch = 10.0f;
	if (pitch < -10.0f)
		pitch = -10.0f;
=======
	if (pitch > 1.0f)
		pitch = 1.0f;
	if (pitch < -1.f)
		pitch = -1.0f;
>>>>>>> Stashed changes

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
>>>>>>> main

	camera.process_mouse_movement(xoffset, yoffset);
	glutPostRedisplay();
}

void setCamera() {

	// �׽�Ʈ�� ī�޶�, ����Ʈ�� ���߿� ��ü�� ���� ������ ����. �Ը���� �ٲ㼭 ������

<<<<<<< HEAD
	/*view = glm::mat4(1.0f);*/
=======
<<<<<<< Updated upstream
	view = glm::mat4(1.0f);
>>>>>>> main

	//3��Ī ���ͺ� ����
 //   cameraPos = glm::vec3(0.0, 900.f, 5.0);
	//cameraFront = glm::vec3(0.f, 0.f, 0.f);
	//cameraUp = glm::vec3(0.0, 1.0, 0.0);
	//view = glm::lookAt(cameraPos,  cameraFront, cameraUp);
	//unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
=======
	 //3��Ī ���ͺ� ����
	 //cameraPos = glm::vec3(0.0, 900.f, 5.0);
	 //cameraFront = glm::vec3(0.f, 0.f, 0.f);
	 //cameraUp = glm::vec3(0.0, 1.0, 0.0);
	 //view = glm::lookAt(cameraPos,  cameraFront, cameraUp);
	 //unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	 //glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	//1��Ī ī�޶� SET passivemouse ����ؼ� ������ 3��Ī �ּ� ��Ű�� �ؿ��� �ּ�Ǯ�� ��밡��
	view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
>>>>>>> Stashed changes

	//1��Ī ī�޶� SET passivemouse ����ؼ� ������ 3��Ī �ּ� ��Ű�� �ؿ��� �ּ�Ǯ�� ��밡��
	//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	//unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	if (camera.camera_change == 2)
		camera.set_top();
	else if (camera.camera_change == 1)
		camera.set_third_person();
	else if (camera.camera_change == 0)
		camera.set_frist_person();

	camera.get_view();
}
void setProjection()
{
	glm::mat4 projection = glm::mat4(1.0f);

	projection = glm::perspective(glm::radians(45.0f), winWidth / winHeight, 0.1f, 1000.0f);
	/*projection = glm::rotate(projection, glm::radians(0.f), glm::vec3(0.0, 1.0, 0.0));*/

	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}

void Render()
{
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setCamera();     // ī�޶� ����
	setProjection();
	//pacmanTop.draw();
	//pacmanBot.draw();
	for (int i{}; i < 327; ++i)
		map[i].draw(132,123,134);

	pacmanTop.draw(255,255,0);
	pacmanBot.draw(255,255,0);
	pacmanNose.draw(0,0,0);
	pacmanEyes.draw(0,0,0);

	first_ghost_body.draw(255,0,0);
	first_ghost_eye.draw(255,255,255);
	second_ghost_body.draw(0,255,0);
	second_ghost_eye.draw(255,255,255);
	third_ghost_body.draw(0,0,255);
	third_ghost_eye.draw(255,255,255);
	/// �׸��� (��ȯ�� Ÿ�̸ӿ��� �������ְ�, �������� update �־��ֱ�)

	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h); //--- ��ü �����츦 ����ؼ� ���
}

//// Ű���� ����
GLvoid KeyboardDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'1':
		camera.camera_change = 0;
		break;
	case'2':
		camera.camera_change = 1;
		glutPostRedisplay();
		break;
	case'3':
		camera.camera_change = 2;
		glutPostRedisplay();
		break;

	case VK_ESCAPE:
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

//// Ű���� �ø�
GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		break;
	}
}
//����� Ű����~
void sp_keybord(int key, int x, int y)
{
	float cameraSpeed = 1.5f;
	if (key == GLUT_KEY_UP)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (key == GLUT_KEY_RIGHT)
	{

		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	glutPostRedisplay();
}

void TimerFunction(int value) {
	//ù��° Ghost AI
	first_ghost_body.AI_FIRST();
	first_ghost_eye.AI_FIRST();
	//�ι�° Ghost AI
	second_ghost_body.AI_SECOND();
	second_ghost_eye.AI_SECOND();
	//����° Ghost AI
	third_ghost_body.AI_THIRD();
	third_ghost_eye.AI_THIRD();


//// Ÿ�̸� (��ü ��ȯ�� ���⼭, ��ȯ �� ������ �������� update �־��ֱ�)

	redrawPlayer();
	animatePlayer();
	glutTimerFunc(10, TimerFunction, 1);
	glutPostRedisplay();
}