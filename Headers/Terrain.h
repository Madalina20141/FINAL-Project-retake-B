#pragma once
#include "precomp.h"
#include "stb_image.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include "ShaderManager.h"
#include "Perlin.h"

class ShaderManager;
class ObjectModel;
class Perlin;
class Camera;
class HeightMap
{
public:

	HeightMap() {}

	std::vector<float> vertices;
	std::vector<unsigned> indices;

	int height = 1080;
	int width = 1920;
	int nChannels;
	float yScale, yShift;
	int rez = 1;


	void Init();
	void Draw(/*ShaderManager* shader,*/ Camera* cam);

	unsigned int numStrips = height - 1;
	unsigned int numVertsPerStrip = width * 2;

	void SetView(glm::vec3 view) { this->view = view; }

	GLuint terrainVAO, terrainVBO, terrainIBO;
	unsigned char* data;
	string file;
	ObjectModel* objectMod;
	ShaderManager* shader;
private:

	glm::mat4 model;
	glm::vec3 view;

	Perlin perlin;
};

