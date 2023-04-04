#include "../Headers/Terrain.h"

#include "../Headers/Camera.h"
#include "../Headers/ObjectModel.h"



void HeightMap::Init()
{
	yScale = 64.f / 256.f;
	yShift = 16.f;

	//Shader terrain(true, terrainVert, terrainFrag);
	shader = new ShaderManager("../../../Resources/Shaders/terrain.vert", "../../../Resources/Shaders/terrain.frag");
	file = string("../../../Resources/Textures/HeightMapThin.png");
	data = stbi_load(file.c_str(), &height, &width, &nChannels, 0);

	if (data)
	{
		std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	for (int i = 0; i < height; i++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char* texel = data + (x + width * i) * nChannels;
			unsigned char y = texel[0];
			int _y = perlin.InterpolatedNoise(i,x,y);
			//float height = ((float)y * yScale) - yShift;
			vertices.push_back(-height / 1.5f + height * i / (float)height);   // vx
			vertices.push_back(((int)_y * yScale) - yShift);   // vy
			vertices.push_back(-width / 2.0f + width * x / (float)width);   // vz
		}
	}
	std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
	stbi_image_free(data);

	for (unsigned int i = 0; i < height - 1; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			for (unsigned int k = 0; k < 2; k++)
			{
				indices.push_back(j + width * (i + k));
			}
		}
	}

	std::cout << "Loaded " << indices.size() << " indices" << std::endl;

	numStrips = (height - 1) / rez;
	numVertsPerStrip = (width / rez) * 2 - 2;

	std::cout << "Created lattice of " << numStrips << " strips with " << numVertsPerStrip << " triangles each" << std::endl;
	std::cout << "Created " << numStrips * numVertsPerStrip << " triangles total" << std::endl;

	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &terrainIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

	//Draw(shader);
}

void HeightMap::Draw(/*ShaderManager* shader,*/ Camera* cam)
{

	//glBindVertexArray(terrainVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);

	//// draw mesh
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

	this->shader->Activate();
	this->shader->SetUniformMatrix(cam->Projection, "projection");
	this->shader->SetUniformMatrix(cam->View, "view");
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	this->shader->SetUniformMatrix(model, "model");

	//glGenVertexArrays(1, &terrainVAO);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(terrainVAO);

	////glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glGenBuffers(1, &terrainIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

	// render the mesh triangle strip by triangle strip - each row at a time
	for (unsigned int strip = 0; strip < numStrips; strip++)
	{
		unsigned int start_index = strip * width * 2;
		glDrawElements(GL_TRIANGLE_STRIP, numVertsPerStrip, GL_UNSIGNED_INT, (void*)(start_index * sizeof(unsigned int)));
	}
}