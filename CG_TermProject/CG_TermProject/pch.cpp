#include "pch.h"
//
//void ReadObj(string file, vector<glm::vec3>* vertexData, vector<glm::vec3>* colorData, vector<glm::ivec3>* faceData)
//{
//	ifstream in(file);
//	if (!in) {
//		cout << "OBJ File NO Have" << endl;
//		return;
//	}
//
//	while (in) {
//		string temp;
//		getline(in, temp);
//
//		if (temp[0] == 'v' && temp[1] == ' ') {
//			istringstream slice(temp);
//
//			glm::vec3 vertemp;
//			for (int i = -1; i < 3; ++i) {
//				string word;
//				getline(slice, word, ' ');
//				if (i == -1) continue;						// v 읽을땐 건너뛴다
//
//				vertemp[i] = atof(word.c_str());
//			}
//			vertexData->emplace_back(vertemp);
//			colorData->emplace_back(glm::vec3(0.f, 0.f, 0.f));
//
//		}
//		else if (temp[0] == 'f' && temp[1] == ' ') {
//			istringstream slice(temp);
//
//			glm::ivec3 facetemp;
//			for (int i = -1; i < 3; ++i) {
//				string word;
//				getline(slice, word, ' ');
//				if (i == -1) continue;						// f 읽을땐 건너뛴다
//				if (word.find("/") != string::npos) {
//					istringstream slash(word);
//					string slashtmp;
//					getline(slash, slashtmp, '/');
//
//					facetemp[i] = atoi(slashtmp.c_str()) - 1;			//f 읽을땐 첫번째값만(v)	//배열인덱스 쓸거라 -1해줌
//				}
//				else {
//					facetemp[i] = atoi(word.c_str()) - 1;			//f 읽을땐 첫번째값만(v)	//배열인덱스 쓸거라 -1해줌
//				}
//			}
//			faceData->emplace_back(facetemp);
//		}
//	}
//}
//void initBuffer(GLuint* VAO, GLuint* VBO, GLuint* EBO, vector<glm::vec3> vertexData, vector<glm::vec3> colorData, vector<glm::ivec3> faceData)
//{
//	glGenVertexArrays(1, VAO);
//	glGenBuffers(1, EBO);
//	glGenBuffers(2, VBO);
//
//	glBindVertexArray(*VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
//	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
//	glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(glm::vec3), &colorData[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceData.size() * sizeof(glm::ivec3), &faceData[0], GL_STATIC_DRAW);
//
//
//}
void initBuffer(GLuint* VAO, GLuint* VBO, vector<glm::vec3> vertexInfo)
{
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexInfo.size() * sizeof(glm::ivec3), &vertexInfo[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  // 노말 속성
	glEnableVertexAttribArray(1);


}
void box_initBuffer(GLuint* VAO, GLuint* VBO, float vertices[])
{
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(1);

}
void ReadObj(std::string file, vector<glm::vec3>& vertexInfo)
{
	vector<glm::vec3> vertex;
	vector<glm::vec3> vNormal;

	vector<glm::ivec3> vFace;
	vector<glm::ivec3> vnFace;

	ifstream in(file);
	if (!in) {
		cout << "OBJ File NO Have" << endl;
		return;
	}

	while (in) {
		string temp;
		getline(in, temp);

		if (temp[0] == 'v' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::vec3 vertemp;
			char tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vertex.push_back(vertemp);
		}

		else if (temp[0] == 'v' && temp[1] == 'n' && temp[2] == ' ') {
			istringstream slice(temp);

			glm::vec3 vertemp;
			string tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vNormal.push_back(vertemp);
		}

		else if (temp[0] == 'f' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::ivec3 vfacetemp;
			glm::ivec3 vnfacetemp;
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;                  // f 읽을땐 건너뛴다
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					vfacetemp[i] = atoi(slashtmp.c_str()) - 1;         //f 읽을땐 첫번째값만(v)   //배열인덱스 쓸거라 -1해줌

					getline(slash, slashtmp, '/');
					getline(slash, slashtmp, '/');
					vnfacetemp[i] = atoi(slashtmp.c_str()) - 1;
				}
				else {
					vfacetemp[i] = atoi(word.c_str()) - 1;         //f 읽을땐 첫번째값만(v)   //배열인덱스 쓸거라 -1해줌
				}
			}
			vFace.push_back(vfacetemp);
			vnFace.push_back(vnfacetemp);
		}
	}

	for (int i = 0; i < vFace.size(); ++i) {
		vertexInfo.push_back(vertex[vFace[i].x]);
		vertexInfo.push_back(vNormal[vnFace[i].x]);

		vertexInfo.push_back(vertex[vFace[i].y]);
		vertexInfo.push_back(vNormal[vnFace[i].y]);

		vertexInfo.push_back(vertex[vFace[i].z]);
		vertexInfo.push_back(vNormal[vnFace[i].z]);
	}

}




bool MyintersectRect(RECT& a, RECT& b)
{
	if (a.top <= b.bottom &&
		a.right >= b.left &&
		a.left <= b.right &&
		a.bottom >= b.top)
		return TRUE;

	else return false;
}