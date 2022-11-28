#include "MONSTER.h"

void GHOST::init(int R, int G, int B, string str,float pos_x,float pos_z)
{
	ReadObj(str, vertexInfo);
	initBuffer(&VAO, VBO, vertexInfo);
	//changeColor(R, G, B);
	x = pos_x;
	z = pos_z;
}

//--- 스케일 변환
void GHOST::scaleTransform(float x, float y, float z)
{
	scale = glm::mat4(1.0f);

	scale = glm::scale(glm::vec3(x, y, z));
	vec_transform.emplace_back(scale);
}

//--- 회전 변환
void GHOST::rotateTransform(float x, float y, float z, float degree)
{
	rotate = glm::mat4(1.0f);

	rotate = glm::rotate(glm::radians(degree), glm::vec3(x, y, z));
	vec_transform.emplace_back(rotate);
}

//--- 위치 변환
void GHOST::moveTranform(float x, float y, float z)
{
	move = glm::mat4(1.0f);

	move = glm::translate(glm::vec3(x, y, z));
	vec_transform.emplace_back(move);
}

RECT GHOST::get_BB()
{
	RECT temp;

	temp.left   = x + dx - 15.0;
	temp.top    = abs(z + dz - 15.0);
	temp.right  = x + dx + 15.0;
	temp.bottom = -abs(z + dz + 15.0);


	return temp;
}

void GHOST::update()
{
	merge = glm::mat4(1.0f);

	for (int i = 0; i < vec_transform.size(); ++i)
	{
		merge = vec_transform[i] * merge;
	}
	vec_transform.clear();
}

void GHOST::draw(int R,int G,int B)
{
	changeColor(R, G, B);
	glBindVertexArray(VAO);
	unsigned int modelTransform = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(merge));
	//glDrawElements(GL_TRIANGLES, faceData.size() * 3, GL_UNSIGNED_INT, 0); 
	glDrawArrays(GL_TRIANGLES, 0, vertexInfo.size());
}

void GHOST::changeColor(int R, int G, int B)
{
	float r = (float)R / 255.f;
	float g = (float)G / 255.f;
	float b = (float)B / 255.f;


	glUniform3f(glGetUniformLocation(shaderID, "objectColor"), r, g, b);
}

void GHOST::AI_FIRST()
{

	if (first_ghost_down)
	{
		if (dz <= 140) dz += 1.;	
		else
		{
			first_ghost_left = TRUE;
			first_ghost_down = FALSE;
			first_ghost_up = FALSE;
			first_ghost_right = FALSE;
		}
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (first_ghost_left)
	{
		if (dx >= -95) dx -= 1.;
		else {
			first_ghost_left = FALSE;
			first_ghost_down = FALSE;
			first_ghost_up = TRUE;
			first_ghost_right = FALSE;
		}
		rotateTransform(0.0, 1.0, 0.0, -90.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (first_ghost_up)
	{
		if (dz > 0) dz -= 1.;
		else
		{
			first_ghost_left = FALSE;
			first_ghost_down = FALSE;
			first_ghost_up = FALSE;
			first_ghost_right = TRUE;
		}
		rotateTransform(0.0, 1.0, 0.0, -180.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (first_ghost_right)
	{
		if (dx < 0) dx += 1.;
		else
		{
			first_ghost_left = FALSE;
			first_ghost_down = TRUE;
			first_ghost_up = FALSE;
			first_ghost_right = FALSE;
		}
		rotateTransform(0.0, 1.0, 0.0, -270.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
}

void GHOST::AI_SECOND()
{
	if (second_ghost_down)
	{
		if (dz <= 140) dz += 1.;
		else
		{
			second_ghost_left	= FALSE;
			second_ghost_down	= FALSE;
			second_ghost_up		= FALSE;
			second_ghost_right	= TRUE;
		}
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (second_ghost_left)
	{
		if (dx > -0) dx -= 1.;
		else {
			second_ghost_left = FALSE;
			second_ghost_down = TRUE;
			second_ghost_up = FALSE;
			second_ghost_right = FALSE;
		}
		rotateTransform(0.0, 1.0, 0.0, -90.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (second_ghost_up)
	{
		if (dz > 0) dz -= 1.;
		else
		{
			second_ghost_left = TRUE;
			second_ghost_down = FALSE;
			second_ghost_up = FALSE;
			second_ghost_right = FALSE;
		}
		rotateTransform(0.0, 1.0, 0.0, 180.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (second_ghost_right)
	{
		if (dx <= 200) dx += 1.;
		else
		{
			second_ghost_left = FALSE;
			second_ghost_down = FALSE;
			second_ghost_up = TRUE;
			second_ghost_right = FALSE;
		}
		rotateTransform(0.0, 1.0, 0.0, 90.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
}

void GHOST::AI_THIRD()
{
	if (third_ghost_down)
	{
		if (dz <= 80) dz += 1.;
		else
		{
			third_ghost_left = TRUE;
			third_ghost_down = FALSE;
			third_ghost_down2 = FALSE;
			third_ghost_up = FALSE;
			third_ghost_up2 = FALSE;
			third_ghost_right = FALSE;
		}
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (third_ghost_left)
	{
		if (dx > -120) dx -= 1.;
		else {
			third_ghost_left = FALSE;
			third_ghost_down = FALSE;
			third_ghost_down2 = TRUE;
			third_ghost_up = FALSE;
			third_ghost_up2 = FALSE;
			third_ghost_right = FALSE;
		}
		rotateTransform(0.0, 1.0, 0.0, -90.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (third_ghost_down2)
	{
		if (dz < 420) dz += 1.;
		else {
			third_ghost_left = FALSE;
			third_ghost_down = FALSE;
			third_ghost_down2 = FALSE;
			third_ghost_up = TRUE;
			third_ghost_up2 = FALSE;
			third_ghost_right = FALSE;
		}
		rotateTransform(0.0, 1.0, 0.0, 0.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (third_ghost_up)
	{
		if (dz >= 80) dz -= 1.;
		else
		{
			third_ghost_left = FALSE;
			third_ghost_down = FALSE;
			third_ghost_down2 = FALSE;
			third_ghost_up = FALSE;
			third_ghost_up2 = FALSE;
			third_ghost_right = TRUE;
		}
		rotateTransform(0.0, 1.0, 0.0, 180.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (third_ghost_up2)
	{
		if (dz >= 0) dz -= 1.;
		else
		{
			third_ghost_left = FALSE;
			third_ghost_down = TRUE;
			third_ghost_down2 = FALSE;
			third_ghost_up = FALSE;
			third_ghost_up2 = FALSE;
			third_ghost_right = TRUE;
		}
		rotateTransform(0.0, 1.0, 0.0, 180.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
	else if (third_ghost_right)
	{
		if (dx <= 0) dx += 1.;
		else
		{
			third_ghost_left = FALSE;
			third_ghost_down = FALSE;
			third_ghost_down2 = FALSE;
			third_ghost_up = FALSE;
			third_ghost_up2 = TRUE;
			third_ghost_right = FALSE;
		}
		rotateTransform(0.0, 1.0, 0.0, 90.);
		scaleTransform(20., 20., 20.);
		moveTranform(x + dx, 0.0, z + dz);
		update();
	}
}

GHOST::GHOST()
{
}

GHOST::~GHOST()
{
}

void InitGhost(GHOST& body, GHOST& eye,float pos_x,float pos_z)
{
	body.init(0, 255, 0, "asset/ghost_body.obj", pos_x,pos_z);
	eye.init(255, 0, 0, "asset/ghost_eyes.obj", pos_x,pos_z);

	body.scaleTransform(20., 20., 20.);
	eye.scaleTransform(20., 20., 20.);

	body.moveTranform(pos_x, 0.0, pos_z);
	eye.moveTranform(pos_x, 0.0, pos_z);


	body.update();
	eye.update();
}
