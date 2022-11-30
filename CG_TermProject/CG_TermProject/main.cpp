#include "pch.h"
#include "PLAYER.h"
#include "MONSTER.h"
#include "GEM.h"
#include "PET.h"
#include "MAP.h"
#include "CAMERA.h"

// 콜백 함수들
GLvoid Reshape(int w, int h);
GLvoid Render();
GLvoid KeyboardDown(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

// 윈도우 관련 변수
GLfloat winWidth, winHeight;

// 쉐이더 관련 함수, 변수
void make_vertexShader();
void make_fragmentShader();
GLuint ShaderProgramID;
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLchar* vertexSource;
GLuint fragmentShader;
GLchar* fragmentSource;

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


char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}
void make_vertexShader()
{
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShader()
{
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
GLuint make_shaderProgram()
{
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return ShaderProgramID;
}

//pacmanTop.update();
//pacmanBot.update();

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	winWidth = 1600.f;
	winHeight = 900.f;
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("CG_TermProject");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	glUseProgram(shaderID);

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
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(sp_keybord);
	glutPassiveMotionFunc(mouse_passive);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}
bool firstMouse{ TRUE };
float yaw{};
float pitch{};
float lastX = 800, lastY = 400;

glm::vec3 cameraPos = glm::vec3(250., 10.f, -250.);;
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, 0.0);;
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);;
void mouse_passive(int x, int y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y;
	lastX = x;
	lastY = y;

	float sensitivity = 0.5;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 10.0f)
		pitch = 10.0f;
	if (pitch < -10.0f)
		pitch = -10.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	glutPostRedisplay();
}
glm::mat4 view;
GLfloat cameraX = 103.f;
GLfloat cameraY = 38.f;
GLfloat cameraZ = 0.f;
GLfloat cameraRotateDegree = 0.f;
void setCamera() {

	// 테스트용 카메라, 뷰포트임 나중에 객체로 만들어서 관리할 것임. 입맛대로 바꿔서 쓰세요

	view = glm::mat4(1.0f);

	//3인칭 쿼터뷰 시점
    /*cameraPos = glm::vec3(0.0, 900.f, 5.0);
	cameraFront = glm::vec3(0.f, 0.f, 0.f);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);*/
	/*view = glm::lookAt(cameraPos,  cameraFront, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);*/

	//1인칭 카메라 SET passivemouse 사용해서 움직임 3인칭 주석 시키고 밑에꺼 주석풀면 사용가능
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

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
		map[i].draw();
	pacmanTop.draw();
	pacmanBot.draw();
	pacmanTop.draw();
	pacmanBot.draw();

	first_ghost_body.draw();
	first_ghost_eye.draw();
	second_ghost_body.draw();
	second_ghost_eye.draw();
	third_ghost_body.draw();
	third_ghost_eye.draw();
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
	case 'q':
		cameraX += 1.0;
		break;
	case 'w':
		cameraX += 1.0;
		break;

	case 'a':
		cameraY += 1.0;
		break;
	case 's':
		cameraY += 1.0;
		break;

	case 'z':
		cameraZ += 1.0;
		break;
	case 'x':
		cameraZ += 1.0;
		break;

	case ' ':
		cameraRotateDegree += 5.0;
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

	//====================================================================
	//충돌 검사 작동확인중 충돌 정상작동함	 pch에 my_intersectrect 만든거 사용요망
	//RECT m_temp[327];
	//RECT g_temp = second_ghost_body.get_BB();
	//for (int i{}; i < 327; ++i)
	//{
	//	m_temp[i] = map[i].get_bb();
	//}
	//for (int i{}; i < 327; ++i)
	//{
	//	if (MyintersectRect(g_temp, m_temp[i]))
	//		cout << "collide" << endl;
	//}
	//====================================================================

//// 타이머 (객체 변환은 여기서, 변환 다 했으면 마지막에 update 넣어주기)

	redrawPlayer();
	animatePlayer();
	glutTimerFunc(10, TimerFunction, 1);
	glutPostRedisplay();
}