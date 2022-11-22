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

// 플레이어 객체 (윗몸, 아랫몸, 눈, 코)
PLAYER pacmanTop;
PLAYER pacmanBot;
PLAYER pacmanEyes;
PLAYER pacmanNose;

GLvoid initPlayer();

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
	glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraRotateDegree), glm::vec3(0.f, 1.f, 0.f));
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
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setCamera();     // 카메라 설정
	setProjection(); // 투영 공간 설정


	/// 그리기 (그리기 전에 main에서 init, update 해주기)
	pacmanTop.draw();
	pacmanBot.draw();
	pacmanEyes.draw();
	pacmanNose.draw();

	/// 그리기 (변환은 타이머에서 적용해주고, 마지막에 update 넣어주기)

	glutSwapBuffers();
	glutPostRedisplay();
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

//// 타이머 (객체 변환은 여기서, 변환 다 했으면 마지막에 update 넣어주기)
GLvoid TimerFunction(int value)
{
	animatePlayer();
	redrawPlayer();
	glutTimerFunc(10, TimerFunction, 1);
	glutPostRedisplay();
}