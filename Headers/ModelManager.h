#pragma once
#include "LoadFiles.h"
#include "ObjectModel.h"
#include "ShaderManager.h"

class ModelManager : public ObjectModel
{
public:
	ModelManager();
	ModelManager(char* FileName, LoadFiles*);
	~ModelManager();

	bool Update(glm::mat4 view, glm::mat4 model);  // we supply an update
	bool Draw();  // and a draw 

	ShaderManager* shader;
	
private:

};

