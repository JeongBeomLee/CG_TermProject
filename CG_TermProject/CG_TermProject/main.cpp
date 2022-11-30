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
void make_vertexShader();
void make_fragmentShader();
GLuint ShaderProgramID;
GLuint shaderID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLchar* vertexSource;
GLuint fragmentShader;
GLchar* fragmentSource;

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
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
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
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShader()
{
	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}
GLuint make_shaderProgram()
{
	ShaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� �����
	glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�
	glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(ShaderProgramID); //--- ������� ���̴� ���α׷� ����ϱ�
	//--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
	//--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
	//--- ����ϱ� ������ ȣ���� �� �ִ�.
	return ShaderProgramID;
}

//pacmanTop.update();
//pacmanBot.update();

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

	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	make_vertexShader(); //--- ���ؽ� ���̴� �����
	make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����
	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glUseProgram(shaderID);

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

	// �׽�Ʈ�� ī�޶�, ����Ʈ�� ���߿� ��ü�� ���� ������ ����. �Ը���� �ٲ㼭 ������

	view = glm::mat4(1.0f);

	//3��Ī ���ͺ� ����
    /*cameraPos = glm::vec3(0.0, 900.f, 5.0);
	cameraFront = glm::vec3(0.f, 0.f, 0.f);
	cameraUp = glm::vec3(0.0, 1.0, 0.0);*/
	/*view = glm::lookAt(cameraPos,  cameraFront, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);*/

	//1��Ī ī�޶� SET passivemouse ����ؼ� ������ 3��Ī �ּ� ��Ű�� �ؿ��� �ּ�Ǯ�� ��밡��
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

	setCamera();     // ī�޶� ����
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
		cameraRotateDegree += 5.0;
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

	//====================================================================
	//�浹 �˻� �۵�Ȯ���� �浹 �����۵���	 pch�� my_intersectrect ����� �����
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

//// Ÿ�̸� (��ü ��ȯ�� ���⼭, ��ȯ �� ������ �������� update �־��ֱ�)

	redrawPlayer();
	animatePlayer();
	glutTimerFunc(10, TimerFunction, 1);
	glutPostRedisplay();
}