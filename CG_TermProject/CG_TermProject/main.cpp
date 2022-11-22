#include "pch.h"
#include "PLAYER.h"
#include "MONSTER.h"
#include "GEM.h"
#include "PET.h"
#include "MAP.h"
#include "CAMERA.h"

// �ݹ� �Լ���
GLvoid Reshape(int w, int h);
GLvoid Render();
GLvoid KeyboardDown(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

// ������ ���� ����
GLfloat winWidth, winHeight;

// ���̴� ���� �Լ�, ����
void make_vertexShaders();
void make_fragmentShaders();
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
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
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
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
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return;
	}
}
void make_fragmentShaders()
{
	GLchar* fragmentsource;
	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
	fragmentsource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentsource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	winWidth = 1600.f;
	winHeight = 900.f;
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("CG_TermProject");
	
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();

	glEnable(GL_DEPTH_TEST);    // ���� ����
	glEnable(GL_DITHER);        // ǥ���� �Ų�����
	glEnable(GL_CULL_FACE);     // �ø�
	glEnable(GL_LINE_SMOOTH);   // ��Ƽ �ٸ����
	glEnable(GL_POLYGON_SMOOTH);// ��Ƽ �ٸ����
	glShadeModel(GL_SMOOTH);    // �ε巯�� ������ �����մϴ�.

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}

// �׽�Ʈ�� ī�޶�, ����Ʈ�� ���߿� ��ü�� ���� ������ ����. �Ը���� �ٲ㼭 ������
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

	setCamera();     // ī�޶� ����
	setProjection(); // ���� ���� ����


	/// �׸��� (�׸��� ���� main���� init, update ���ֱ�)
	

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

//// Ű���� �ø�
GLvoid KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
		break;
	}
}

//// Ÿ�̸� (��ü ��ȯ�� ���⼭, ��ȯ �� ������ �������� update �־��ֱ�)
GLvoid TimerFunction(int value)
{

	glutTimerFunc(10, TimerFunction, 1);
	glutPostRedisplay();
}