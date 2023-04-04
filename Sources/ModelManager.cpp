#include "../Headers/ModelManager.h"
#include "../Headers/ObjectModel.h" 

ModelManager::ModelManager()
{
	// a default system
	mTranslationMatrix = glm::mat4(1.0f);
	mRotationMatrix = glm::mat4(1.0f);
	mScaleMatrix = glm::mat4(1.0f);
	Model = glm::mat4(1.0f);  // give the model I as an initialisation
	glm::vec3 Pos = glm::vec3(11.0f, 0.0f, 0.4f);
	this->SetPosition(Pos);

	Scales = glm::vec3(1.5f, 1.5f, 1.5f);
	SetModelMatrix();
	DegreeRotations = glm::vec3(0, 0, 0);
	Colour = glm::vec4(Rand(1.0f), Rand(1.0f), Rand(1.0f), 1.0f);

	
}

ModelManager::ModelManager(char* FileName, LoadFiles* FH) :ModelManager()
{
	bool ret = ObjectModel::LoadandConvert(&gdrawObjects, materials, textures, FileName, FH);

	if (ret == false)
	{
		printf("This load for %s failed, check file name and try again!\n", FileName);

	}
}

ModelManager::~ModelManager()
{
}

float roll2 = 1.0f;
bool ModelManager::Update(glm::mat4 view, glm::mat4 model)
{
	glUseProgram(this->programObject);

	DegreeRotations.x += 1.1f;
	Rotations.x = DEG2RAD(DegreeRotations.x);

	DegreeRotations.y -= 1.1f;
	Rotations.y = DEG2RAD(DegreeRotations.y);

	//	DegreeRotations.z += 1.1f;
	Rotations.z = DEG2RAD(DegreeRotations.z);
	mScaleMatrix = glm::mat4(1.0f);


	mScaleMatrix = glm::scale(mScaleMatrix, Scales);
	MakeRotationMatrix();
	MakeTranslationMatrix();
	MakeModelMatrix();

	// get these from the camera

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : near 0.1 unit <->  far 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 200.0f);

	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 20, 10),
		// Camera is at (17,23,3), in World Space
		glm::vec3(0, 0, 0),
		// look at the origin
		glm::vec3(0, roll2, 0)  // pointing up( 0,-1,0 will be upside-down)););}
	);



	glUseProgram(this->programObject);

	glEnable(GL_DEPTH_TEST);

	// make the MVP 
	glm::mat4 MVP = Projection * view * model;  // Remember order seems backwards

	GLuint MatrixID = glGetUniformLocation(programObject, "MVP");  //LOOK!!!!
	GLuint AmbID = glGetUniformLocation(programObject, "Ambient");
	GLuint SelectedID = glGetUniformLocation(programObject, "Selected");

	float Amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  // you can change these colour values to give your model a tint 
	glUniform4fv(AmbID, 1, &Amb[0]);
	glUniform1f(SelectedID, selected);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);



	return true;
}

bool ModelManager::Draw()
{
	//shader = new ShaderManager("../../../Resources/Shaders/modelVertShader.vert", "../../../Resources/Shaders/modelFragShad.frag");
	//this->shader->Activate();
	glUseProgram(this->programObject);
	for (int frames = 0; frames < gdrawObjects.size(); frames++)

	{
		DrawObject o = gdrawObjects[frames];  // get the DrawObject which has the vb
		glBindBuffer(GL_ARRAY_BUFFER, o.vb);  // this is now the buffer we work with


		GLsizei stride = (3 + 3 + 3 + 2) * sizeof(GLfloat);  // we have 3x vert, 3x normal,3xcolour,2x texcoord.
		//load the vertex data info
		glVertexAttribPointer(this->positionLoc,
			3,		// there are 3 values xyz
			GL_FLOAT, // they a float
			GL_FALSE,	 // don't need to be normalised
			stride,	  // how may floats to the next one
			(const void*)0  //this->Vertices // where do they start as an index); // use 3 values, but add 5 each time to get to the next
		);

		// Load the texture coordinate
		glVertexAttribPointer(this->texCoordLoc,
			2,	 // there are 2 values
			GL_FLOAT,	 //they are floats 
			GL_FALSE,	 // we don't need them normalise
			stride,		  // wats the stride to the next ones
			(const void*)(sizeof(float) * 9)  //(GLfloat*)this->Vertices + 9 // where is the 1st one);
		);

		glEnableVertexAttribArray(this->positionLoc);
		glEnableVertexAttribArray(this->texCoordLoc);

		// bind the correct texture for this shape	
		if ((o.material_id < materials.size())) {
			std::string diffuse_texname = materials[o.material_id].diffuse_texname;
			if (textures.find(diffuse_texname) != textures.end()) {
				glBindTexture(GL_TEXTURE_2D, textures[diffuse_texname]);
				
			}
			else
				printf("Model Draw texture error \n");
		}
		glUniform1i(this->samplerLoc, 0);
		glDrawArrays(GL_TRIANGLES, 0, o.numTriangles);
		if (glGetError() != GL_NO_ERROR)
		{
			//printf("Model Draw error with the triangles\n");
			return false;
		}

		// take these off the bind		
		glDisableVertexAttribArray(positionLoc);
		glDisableVertexAttribArray(texCoordLoc);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (glGetError() != GL_NO_ERROR)
		{
			printf("Model Draw array buffer error\n");
			return false;
		}

	}
	// always good to unbind	
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}
