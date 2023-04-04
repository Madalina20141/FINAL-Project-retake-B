#pragma once
#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "precomp.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream> 
#include <cerrno>

std::string get_file_contents(const char* filename);

class ShaderManager
{
public:
	GLuint ID;
	ShaderManager() {}
	ShaderManager(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

	void SetUniformMatrix(glm::mat4 objectModel, const char* uniform);
	void SetUniformV4(glm::vec4 lightColor, const char* uniform);
	void SetUniformV3(glm::vec3 lightPos, const char* uniform);

};


#endif


//#pragma once
//#include <GLES2/gl2.h>
//#include "LoadFiles.h"
//#include "ObjectModel.h"
//
//class ShaderManager
//{
//public:
//	GLuint ID;
//	ShaderManager();
//	ShaderManager(const char* vertexFile, const char* fragmentFile);
//	~ShaderManager();
//
//	GLuint MakeProgramObject(GLuint VShad, GLuint FShad);
//	GLuint LoadAndMakeShader(LoadFiles* FH, GLenum type, const char* Fname);
//	GLuint MakeShader(GLenum type, const char* shaderSrc);
//
//	void SetUniformMatrix(glm::mat4 objectModel, const char* uniform);
//
//	void Use();
//	
//private:
//
//};
//
