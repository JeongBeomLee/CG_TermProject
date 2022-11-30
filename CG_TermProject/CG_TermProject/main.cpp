#include "pch.h"
#include "PLAYER.h"
#include "MONSTER.h"
#include "GEM.h"
#include "PET.h"
#include "MAP.h"
#include "CAMERA.h"
#include "shader.h"

// 콜백 함수들
GLvoid Reshape(int w, int h);
GLvoid Render();
GLvoid KeyboardDown(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

// 윈도우 관련 변수
GLfloat winWidth, winHeight;

// 쉐이더 관련 함수, 변수

GLuint shaderID; //--- 세이더 프로그램 이름

// lighting
glm::vec3 lightPos(10.0, 100.0, -3.0);

//입력함수들
void mouse_passive(int x, int y); //1인칭 시점때 사용
void sp_keybord(int key, int x, int y); //1인칭 시점때 이동

// 플레이어 객체 (윗몸, 아랫몸, 눈, 코)
PLAYER pacmanTop;
PLAYER pacmanBot;
PLAYER pacmanEyes;
PLAYER pacmanNose;

//고스트 객체 5마리 예정
GHOST first_ghost_body;
GHOST first_ghost_eye;
GHOST second_ghost_body;
GHOST second_ghost_eye;
GHOST third_ghost_body;
GHOST third_ghost_eye;

// 맵
MAP map[327]{};

// 카메라
CAMERA camera;
glm::vec3 cameraPos = glm::vec3(250., 10.f, -250.);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	winWidth = 1600.f;
	winHeight = 900.f;
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, -500);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("CG_TermProject");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	Shader shader("new_vertex.glsl", "new_fragment.glsl");
	shaderID = shader.ID;
	glUseProgram(shaderID);

	shader.setVec3("lightPos", lightPos);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	//make_vertexShader(); //--- 버텍스 세이더 만들기
	//make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	//glUseProgram(shaderID);


	glEnable(GL_DEPTH_TEST);    // 은면 제거
	glEnable(GL_DITHER);        // 표면을 매끄럽게
	glEnable(GL_CULL_FACE);     // 컬링
	glEnable(GL_LINE_SMOOTH);   // 안티 앨리어싱
	glEnable(GL_POLYGON_SMOOTH);// 안티 앨리어싱
	glShadeModel(GL_SMOOTH);    // 부드러운 음영을 수행합니다.

	initPlayer();

	// 유령객체 Init
	InitGhost(first_ghost_body, first_ghost_eye, 250., -270.);
	InitGhost(second_ghost_body, second_ghost_eye, -110., -70.);
	InitGhost(third_ghost_body, third_ghost_eye, -50, -270);
	//맵 Init
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

	// 테스트용 카메라, 뷰포트임 나중에 객체로 만들어서 관리할 것임. 입맛대로 바꿔서 쓰세요

<<<<<<< HEAD
	/*view = glm::mat4(1.0f);*/
=======
<<<<<<< Updated upstream
	view = glm::mat4(1.0f);
>>>>>>> main

	//3인칭 쿼터뷰 시점
 //   cameraPos = glm::vec3(0.0, 900.f, 5.0);
	//cameraFront = glm::vec3(0.f, 0.f, 0.f);
	//cameraUp = glm::vec3(0.0, 1.0, 0.0);
	//view = glm::lookAt(cameraPos,  cameraFront, cameraUp);
	//unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
=======
	 //3인칭 쿼터뷰 시점
	 //cameraPos = glm::vec3(0.0, 900.f, 5.0);
	 //cameraFront = glm::vec3(0.f, 0.f, 0.f);
	 //cameraUp = glm::vec3(0.0, 1.0, 0.0);
	 //view = glm::lookAt(cameraPos,  cameraFront, cameraUp);
	 //unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	 //glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	//1인칭 카메라 SET passivemouse 사용해서 움직임 3인칭 주석 시키고 밑에꺼 주석풀면 사용가능
	view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
>>>>>>> Stashed changes

	//1인칭 카메라 SET passivemouse 사용해서 움직임 3인칭 주석 시키고 밑에꺼 주석풀면 사용가능
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

	setCamera();     // 카메라 설정
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
	/// 그리기 (변환은 타이머에서 적용해주고, 마지막에 update 넣어주기)

	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h); //--- 전체 윈도우를 사용해서 출력
}

//// 키보드 눌림
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

//// 키보드 올림
GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		break;
	}
}
//스페셜 키보드~
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
	//첫번째 Ghost AI
	first_ghost_body.AI_FIRST();
	first_ghost_eye.AI_FIRST();
	//두번째 Ghost AI
	second_ghost_body.AI_SECOND();
	second_ghost_eye.AI_SECOND();
	//세번째 Ghost AI
	third_ghost_body.AI_THIRD();
	third_ghost_eye.AI_THIRD();


//// 타이머 (객체 변환은 여기서, 변환 다 했으면 마지막에 update 넣어주기)

	redrawPlayer();
	animatePlayer();
	glutTimerFunc(10, TimerFunction, 1);
	glutPostRedisplay();
}