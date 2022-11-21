#include "pch.h"

void ReadObj(string file, vector<glm::vec3>* vertexData, vector<glm::ivec3>* faceData)
{
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
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;						// v ÀÐÀ»¶© °Ç³Ê¶Ú´Ù

				vertemp[i] = atof(word.c_str());
			}
			vertexData->emplace_back(vertemp);
		}
		else if (temp[0] == 'f' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::ivec3 facetemp;
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;						// f ÀÐÀ»¶© °Ç³Ê¶Ú´Ù
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					facetemp[i] = atoi(slashtmp.c_str()) - 1;			//f ÀÐÀ»¶© Ã¹¹øÂ°°ª¸¸(v)	//¹è¿­ÀÎµ¦½º ¾µ°Å¶ó -1ÇØÁÜ
				}
				else {
					facetemp[i] = atoi(word.c_str()) - 1;			//f ÀÐÀ»¶© Ã¹¹øÂ°°ª¸¸(v)	//¹è¿­ÀÎµ¦½º ¾µ°Å¶ó -1ÇØÁÜ
				}
			}
			faceData->emplace_back(facetemp);
		}
	}

	//cout << "v information" << endl;
	//for (int i = 0; i < vertex.size(); ++i) {
	//	cout << vertex[i].x << ' ' << vertex[i].y << ' ' << vertex[i].z << endl;
	//}
	//cout << endl << "f information" << endl;;
	//for (int i = 0; i < face.size(); ++i) {
	//	cout << face[i].x << ' ' << face[i].y << ' ' << face[i].z << endl;
	//}
}
void initBuffer(GLuint* VAO, GLuint* VBO, GLuint* EBO, vector<glm::vec3> vertexData, vector<glm::ivec3> faceData)
{
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, EBO);
	glGenBuffers(1, VBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceData.size() * sizeof(glm::ivec3), &faceData[0], GL_STATIC_DRAW);
}