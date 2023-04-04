#pragma once

#include "../Headers/ObjectModel.h"


class CubeModel : public ObjectModel
{
public:
	CubeModel();
	CubeModel(LoadFiles*);
	
	~CubeModel();
		
	bool Update(glm::mat4 view, glm::mat4 model) ; // we supply an update
	bool Draw(); // and a draw 
	
	
	

};
