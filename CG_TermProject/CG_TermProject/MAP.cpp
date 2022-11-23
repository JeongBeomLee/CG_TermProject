#include "MAP.h"

void MAP::init(int R, int G, int B)
{
	ReadObj("asset/BOX.obj", &vertexData, &colorData, &faceData);
	initBuffer(&VAO, VBO, &EBO, vertexData, colorData, faceData);
	changeColor(R, G, B);
}

//--- 스케일 변환
void MAP::scaleTransform(float x, float y, float z)
{
	scale = glm::mat4(1.0f);

	scale = glm::scale(glm::vec3(x, y, z));
	vec_transform.emplace_back(scale);
}


//--- 회전 변환
void MAP::rotateTransform(float x, float y, float z, float degree)
{
	rotate = glm::mat4(1.0f);

	rotate = glm::rotate(glm::radians(degree), glm::vec3(x, y, z));
	vec_transform.emplace_back(rotate);
}

//--- 위치 변환
void MAP::moveTranform(float x, float y, float z)
{
	pos_x = x;
	pos_z = z;

	move = glm::mat4(1.0f);

	move = glm::translate(glm::vec3(x, y, z));
	vec_transform.emplace_back(move);
}

void MAP::update()
{
	merge = glm::mat4(1.0f);

	for (int i = 0; i < vec_transform.size(); ++i)
	{
		merge = vec_transform[i] * merge;
	}
	vec_transform.clear();
}

void MAP::draw()
{
	glBindVertexArray(VAO);
	unsigned int modelTransform = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(merge));
	glDrawElements(GL_TRIANGLES, faceData.size() * 3, GL_UNSIGNED_INT, 0);
}

 RECT MAP::get_bb()
{
	RECT temp;
	temp.bottom = abs(pos_z + 10.);
	temp.left = pos_x - 10.;
	temp.top = abs(pos_z - 10.);
	temp.right = pos_x + 10.;

	return temp;
}

void MAP::changeColor(int R, int G, int B)
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

MAP::MAP()
{
}

MAP::~MAP()
{
}

void InitMap(MAP map[327])
{
	//맵 전체 크기 600*600
	//타일 하나당 크기 20

	float row = -300.0;
	float col = -300.0;
	for (int i{}; i < 30; ++i)
	{
		int rand_num = rand() % 255;
		//윗면
		map[i].init(rand_num, 0, 0);
		map[i].moveTranform(row + (20.0 * i), 0.0, col);
		map[i].update();
		//아랫면
		map[30 + i].init(rand_num, 0, 0);
		map[30 + i].moveTranform(row + (20.0 * i), 0.0, col + 600.0);
		map[30 + i].update();
		//윗면 중앙
		if (i < 5)
		{
			map[60 + i].init(rand_num, 0, 0);
			map[60 + i].moveTranform(0.0, 0.0, (col + 20.0) + (i * 20.0));
			map[60 + i].update();

			map[113 + i].init(rand_num, 0, 0);
			map[113 + i].moveTranform(-20.0, 0.0, (col + 20.0) + (i * 20.0));
			map[113 + i].update();

			map[128 + i].init(rand_num, 0, 0);
			map[128 + i].moveTranform(row+20+20.*i, 0.0, (col + 200.0));
			map[128 + i].update();

			map[133 + i].init(rand_num, 0, 0);
			map[133 + i].moveTranform(-row - 40 - 20. * i, 0.0, (col + 200.0));
			map[133 + i].update();
		}
		//윗면 양쪽 사이드
		if (i < 10)
		{
			map[65 + i].init(rand_num, 0, 0);
			map[65 + i].moveTranform(row, 0.0, (col + 20.0) + (i * 20.0));
			map[65 + i].update();
			map[75 + i].init(rand_num, 0, 0);
			map[75 + i].moveTranform(-row - 20, 0.0, (col + 20.0) + (i * 20.0));
			map[75 + i].update();
		}
		// 맨위 사각형 4개
		if (i < 3)
		{
			for (int j{}; j < 3; ++j)
			{
				map[85 + j].init(rand_num, 0, 0);
				map[85 + j].moveTranform((row + 60.) + (20. * j), 0.0, (col + 60.));
				map[85 + j].update();

				int rand_num = rand() % 255;
				map[88 + j].init(rand_num, 0, 0);
				map[88 + j].moveTranform((-row - 120.) + (20. * j), 0.0, (col + 60.));
				map[88 + j].update();
			}
			for (int j{}; j < 3; ++j)
			{
				map[91 + j].init(rand_num, 0, 0);
				map[91 + j].moveTranform((row + 60.) + (20. * j), 0.0, (col + 80.));
				map[91 + j].update();

				map[94 + j].init(rand_num, 0, 0);
				map[94 + j].moveTranform((-row - 120.) + (20. * j), 0.0, (col + 80.));
				map[94 + j].update();
			}
			map[118 + i].init(rand_num, 0, 0);
			map[118 + i].moveTranform((row + 60.) + (20. * i), 0.0, (col + 140));
			map[118 + i].update();

			map[123 + i].init(rand_num, 0, 0);
			map[123 + i].moveTranform((-row - 120.) + (20. * i), 0.0, (col + 140));
			map[123 + i].update();
		}
		if (i < 4)
		{
			map[97 + i].init(rand_num, 0, 0);
			map[97 + i].moveTranform((row + 160.) + (20. * i), 0.0, (col + 60.));
			map[97 + i].update();

			map[101 + i].init(rand_num, 0, 0);
			map[101 + i].moveTranform((-row - 240.) + (20. * i), 0.0, (col + 60.));
			map[101 + i].update();
		
			map[105 + i].init(rand_num, 0, 0);
			map[105 + i].moveTranform((row + 160.) + (20. * i), 0.0, (col + 80.));
			map[105 + i].update();

			map[109 + i].init(rand_num, 0, 0);
			map[109 + i].moveTranform((-row - 240.) + (20. * i), 0.0, (col + 80.));
			map[109 + i].update();
		}
		// 위쪽 ㅜ 모양
		if (i < 8) {
			map[138 + i].init(rand_num, 0, 0);
			map[138 + i].moveTranform(row + 220 + (20. * i), 0.0, (col + 140));
			map[138 + i].update();
		}
		if (i < 3) {
			map[146 + i].init(rand_num, 0, 0);
			map[146 + i].moveTranform(0.0, 0.0, (col + 160)+(20. * i));
			map[146 + i].update();
			map[149 + i].init(rand_num, 0, 0);
			map[149 + i].moveTranform(-20.0, 0.0, (col + 160) + (20. * i));
			map[149 + i].update();
		}
		// 위쪽 ㅏ 모양
		if (i < 8){
			map[152 + i].init(rand_num, 0, 0);
			map[152 + i].moveTranform(row + 160 , 0.0, (col + 140)+(20. * i));
			map[152 + i].update();

			map[160 + i].init(rand_num, 0, 0);
			map[160 + i].moveTranform(-row - 180, 0.0, (col + 140) + (20. * i));
			map[160 + i].update();
		}
		if (i < 3){
			map[168 + i].init(rand_num, 0, 0);
			map[168 + i].moveTranform(row + 180 +(20. * i), 0.0, (col + 200));
			map[168 + i].update();
			map[171 + i].init(rand_num, 0, 0);
			map[171 + i].moveTranform(-row - 200 - (20. * i), 0.0, (col + 200));
			map[171 + i].update();

			map[174 + i].init(rand_num, 0, 0);
			map[174 + i].moveTranform(row + 100, 0.0, (col + 220) + (20. * i));
			map[174 + i].update();
			map[177 + i].init(rand_num, 0, 0);
			map[177 + i].moveTranform(-row - 120, 0.0, (col + 220) + (20. * i));
			map[177 + i].update();
		}
		if (i < 6) {
			map[180 + i].init(rand_num, 0, 0);
			map[180 + i].moveTranform(row + 20. * i,0.0,col+280);
			map[180 + i].update();
			map[186 + i].init(rand_num, 0, 0);
			map[186 + i].moveTranform(-row-20 - 20.* i, 0.0, col + 280);
			map[186 + i].update();

			map[192 + i].init(rand_num, 0, 0);
			map[192 + i].moveTranform(row + 20. * i, 0.0, col + 340);
			map[192 + i].update();
			map[198 + i].init(rand_num, 0, 0);
			map[198 + i].moveTranform(-row -20- 20. * i, 0.0, col + 340);
			map[198 + i].update();
		}
		//아래쪽 ㅜ 모양
		if (i < 8){
			map[204 + i].init(rand_num, 0, 0);
			map[204 + i].moveTranform(row + 220 + (20. * i), 0.0, (col + 400));
			map[204 + i].update();
		}
		if (i < 3){
			map[212 + i].init(rand_num, 0, 0);
			map[212 + i].moveTranform(0.0, 0.0, (col + 420) + (20. * i));
			map[212 + i].update();
			map[215 + i].init(rand_num, 0, 0);
			map[215 + i].moveTranform(-20.0, 0.0, (col + 420) + (20. * i));
			map[215 + i].update();
		}
		//-----------------------------------
		if (i < 9){
			map[218 + i].init(rand_num, 0, 0);
			map[218 + i].moveTranform(row, 0.0, (col + 420) + (20. * i));
			map[218 + i].update();

			map[226 + i].init(rand_num, 0, 0);
			map[226 + i].moveTranform(-row-20, 0.0, (col + 420) + (20. * i));
			map[226 + i].update();
		}
		if (i < 6) {
			map[235 + i].init(rand_num, 0, 0);
			map[235 + i].moveTranform(row+ 20. * i, 0.0, col + 420);
			map[235 + i].update();
			map[241 + i].init(rand_num, 0, 0);
			map[241 + i].moveTranform(-row - 20. - 20. * i, 0.0, col + 420);
			map[241 + i].update();
		}
		if (i < 3) {
			map[247 + i].init(rand_num, 0, 0);
			map[247 + i].moveTranform(row + 100, 0.0, (col + 360) + (20. * i));
			map[247 + i].update();
			map[250 + i].init(rand_num, 0, 0);
			map[250 + i].moveTranform(-row - 120, 0.0, (col + 360) + (20. * i));
			map[250 + i].update();
		}
		if (i < 5){
			map[253 + i].init(rand_num, 0, 0);
			map[253 + i].moveTranform(row + 160, 0.0, (col + 340) + (20. * i));
			map[253 + i].update();
			map[258 + i].init(rand_num, 0, 0);
			map[258 + i].moveTranform(-row - 180, 0.0, (col + 340) + (20. * i));
			map[258 + i].update();
		}
		//중앙 사각형
		if (i < 8) {
			map[263 + i].init(rand_num, 0, 0);
			map[263 + i].moveTranform(row + 220 + (20. * i), 0.0, (col + 260));
			map[263 + i].update();
			map[271 + i].init(rand_num, 0, 0);
			map[271 + i].moveTranform(row + 220 + (20. * i), 0.0, (col + 340));
			map[271 + i].update();
		}
		if (i < 3) {
			map[279 + i].init(rand_num, 0, 0);
			map[279 + i].moveTranform(row + 220 , 0.0, (col + 280) + (20. * i));
			map[279 + i].update();
			map[282 + i].init(rand_num, 0, 0);
			map[282 + i].moveTranform(-row - 240 , 0.0, (col + 280) + (20. * i));
			map[282 + i].update();
		}
		if (i < 9) {
			map[285 + i].init(rand_num, 0, 0);
			map[285 + i].moveTranform(row + 60+ (20. * i), 0.0, (col + 540) );
			map[285 + i].update();
			map[294 + i].init(rand_num, 0, 0);
			map[294 + i].moveTranform(-row - 80 - (20. * i), 0.0, (col + 540));
			map[294 + i].update();
		}
		if (i < 4){
			map[303 + i].init(rand_num, 0, 0);
			map[303 + i].moveTranform(row +220, 0.0, (col + 520)-(20. * i));
			map[303 + i].update();
			map[307 + i].init(rand_num, 0, 0);
			map[307 + i].moveTranform(-row - 240, 0.0, (col + 520) - (20. * i));
			map[307 + i].update();
		}
		if (i < 6) {
			map[311 + i].init(rand_num, 0, 0);
			map[311 + i].moveTranform(row + 60 + (20. * i), 0.0, (col + 480));
			map[311 + i].update();
			map[317 + i].init(rand_num, 0, 0);
			map[317 + i].moveTranform(-row - 80 - (20. * i), 0.0, (col + 480));
			map[317 + i].update();
		}
		if (i < 2) {
			map[323 + i].init(rand_num, 0, 0);
			map[323 + i].moveTranform(row + 280 + (20. * i), 0.0, (col + 540));
			map[323 + i].update();
			map[325 + i].init(rand_num, 0, 0);
			map[325 + i].moveTranform(row + 280 + (20. * i), 0.0, (col + 520));
			map[325 + i].update();
		}
	}

}
