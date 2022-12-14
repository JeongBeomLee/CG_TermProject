#include "PET.h"

void PET::init(int R, int G, int B, string str)
{
	//ReadObj(str, &vertexData, &colorData, &faceData);
	//initBuffer(&VAO, VBO, &EBO, vertexData, colorData, faceData);
	changeColor(R, G, B);
}

//--- 스케일 변환
void PET::scaleTransform(float x, float y, float z)
{
	scale = glm::mat4(1.0f);

	scale = glm::scale(glm::vec3(x, y, z));
	vec_transform.emplace_back(scale);
}

//--- 회전 변환
void PET::rotateTransform(float x, float y, float z, float degree)
{
	rotate = glm::mat4(1.0f);

	rotate = glm::rotate(glm::radians(degree), glm::vec3(x, y, z));
	vec_transform.emplace_back(rotate);
}

//--- 위치 변환
void PET::moveTranform(float x, float y, float z)
{
	move = glm::mat4(1.0f);

	move = glm::translate(glm::vec3(x, y, z));
	vec_transform.emplace_back(move);
}

RECT PET::get_BB(float x, float z)
{
	RECT temp;

	temp.left = x - 1.0;
	temp.top = z - 1.0;
	temp.right = x + 1.0;
	temp.bottom = z + 1.0;

	return temp;
}

void PET::update()
{
	merge = glm::mat4(1.0f);

	for (int i = 0; i < vec_transform.size(); ++i)
	{
		merge = vec_transform[i] * merge;
	}
	vec_transform.clear();
}

void PET::draw(unsigned int shaderID)
{
	glBindVertexArray(VAO);
	unsigned int modelTransform = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(merge));
	glDrawElements(GL_TRIANGLES, faceData.size() * 3, GL_UNSIGNED_INT, 0);
}

void PET::changeColor(int R, int G, int B)
{
	float r = (float)R / 255.f;
	float g = (float)G / 255.f;
	float b = (float)B / 255.f;

	for (int i = 0; i < colorData.size(); ++i)
	{
		colorData[i] = glm::vec3(r, g, b);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(glm::vec3), &colorData[0], GL_STATIC_DRAW);
}

PET::PET()
{
}

PET::~PET()
{
}

