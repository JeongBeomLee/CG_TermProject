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
void make_vertexShaders();
void make_fragmentShaders();
GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader;
GLuint coreShader{};

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
void make_vertexShaders()
{
	GLchar* vertexsource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexsource = filetobuf("vertex.glsl");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexsource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void make_fragmentShaders()
{
	GLchar* fragmentsource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentsource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentsource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();

	coreShader = glCreateProgram();
	glAttachShader(coreShader, vertexShader);
	glAttachShader(coreShader, fragmentShader);
	glLinkProgram(coreShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(coreShader);
}

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
	InitShader();

	glEnable(GL_DEPTH_TEST);    // 은면 제거
	glEnable(GL_DITHER);        // 표면을 매끄럽게
	glEnable(GL_CULL_FACE);     // 컬링
	glEnable(GL_LINE_SMOOTH);   // 안티 앨리어싱
	glEnable(GL_POLYGON_SMOOTH);// 안티 앨리어싱
	glShadeModel(GL_SMOOTH);    // 부드러운 음영을 수행합니다.

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}

// 테스트용 카메라, 뷰포트임 나중에 객체로 만들어서 관리할 것임. 입맛대로 바꿔서 쓰세요
GLfloat cameraX = 103.f;
GLfloat cameraY = 38.f;
GLfloat cameraZ = 0.f;
GLfloat cameraRotateDegree = 0.f;

void setCamera()
{
	glm::mat4 view = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(cameraX, 45.f, cameraZ);
	cameraPos = glm::rotate(cameraPos, glm::radians(cameraRotateDegree), glm::vec3(0.0, 1.0, 0.0));
	glm::vec3 cameraTarget = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, Right);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

void setProjection()
{
	glm::mat4 projection = glm::mat4(1.0f);

	projection = glm::perspective(glm::radians(45.0f), winWidth / winHeight, 10.f, 1000.0f);
	projection = glm::rotate(projection, glm::radians(0.f), glm::vec3(0.0, 1.0, 0.0));
	//projection = glm::translate(projection, glm::vec3(camera_x, camera_y, camera_z));

	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}

void Render()
{
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setCamera();     // 카메라 설정
	setProjection(); // 투영 공간 설정


	/// 그리기 (그리기 전에 main에서 init, update 해주기)
	

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
		cameraRotateDegree += 1.0;
		break;

	case VK_ESCAPE:
		glutLeaveMainLoop();
		break;
	}
}

//// 키보드 올림
GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		break;
	}
}

//// 타이머 (객체 변환은 여기서, 변환 다 했으면 마지막에 update 넣어주기)
GLvoid TimerFunction(int value)
{

	glutTimerFunc(10, TimerFunction, 1);
	glutPostRedisplay();
}