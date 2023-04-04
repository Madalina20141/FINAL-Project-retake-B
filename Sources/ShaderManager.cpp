#include "../Headers/ShaderManager.h" 

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in.is_open())
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

ShaderManager::ShaderManager(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode; //= get_file_contents(vertexFile); // FileHand->LoadText(vertexFile);
	std::string fragmentCode; //= get_file_contents(fragmentFile); // FileHand->LoadText(fragementFile);

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Vertex Shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// Fragment Shaders
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void ShaderManager::SetUniformMatrix(glm::mat4 objectModel, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, uniform), 1, GL_FALSE, glm::value_ptr(objectModel));
}

void ShaderManager::SetUniformV4(glm::vec4 lightColor, const char* uniform)
{
	glUniform4f(glGetUniformLocation(this->ID, uniform), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
}

void ShaderManager::SetUniformV3(glm::vec3 lightPos, const char* uniform)
{
	glUniform3f(glGetUniformLocation(this->ID, uniform), lightPos.x, lightPos.y, lightPos.z);
}



void ShaderManager::Activate()
{
	glUseProgram(ID);
}

void ShaderManager::Delete()
{
	glDeleteProgram(ID);
}

//
//#include <iostream> // we're going to be loading files so need these
//#include <fstream>
//#include <string>
//
//#include <sstream>
//
//#include <cerrno>
//#include <glm/gtc/type_ptr.hpp>
//
//using namespace std;
//
//ShaderManager::ShaderManager()
//{
//	//empty
//}
//ShaderManager::ShaderManager(const char* vertexFile, const char* fragmentFile)
//{
//}
//;
//
//ShaderManager::~ShaderManager()
//{
//	//empty (probably should unload the shaders?)
//};
//
////link a V and F shader together
//GLuint ShaderManager::MakeProgramObject(GLuint VShad, GLuint FShad)
//{
//
//
//	// Create the program object	
//	GLuint PO = glCreateProgram();
//
//	if (PO == 0) 	return false;
//
//	// now we have the V and F shaders  attach them to the progam object
//	glAttachShader(PO, VShad);
//	glAttachShader(PO, FShad);
//
//	// Link the program
//	glLinkProgram(PO);
//	// Check the link status
//	GLint LinkedOk;
//
//	glGetProgramiv(PO, GL_LINK_STATUS, &LinkedOk);
//	if (!LinkedOk)
//	{
//		GLint RetinfoLen = 0; // check and report any errors using standard system
//
//		glGetProgramiv(PO, GL_INFO_LOG_LENGTH, &RetinfoLen);
//		if (RetinfoLen > 1)
//		{
//			GLchar* infoLog = (GLchar*)malloc(sizeof(char) * RetinfoLen);
//			glGetProgramInfoLog(PO, RetinfoLen, NULL, infoLog);
//			fprintf(stderr, "Error linking program:\n%s\n", infoLog);
//			free(infoLog);
//		}
//		glDeleteProgram(PO);
//		return false;
//	}
//
//
//	return PO;
//}
//
//GLuint ShaderManager::LoadAndMakeShader(LoadFiles* FH, GLenum type, const char* Fname)
//{
//	const char* ShaderTxt = FH->LoadText(Fname);
//
//	// print out the shader		
//	printf("shader loaded from filename %s \n %s\n", Fname, ShaderTxt);
//
//
//	GLuint Shader = MakeShader(type, ShaderTxt);
//
//	if (Shader == 0) printf("Failed to compile Shader/n");
//
//	return Shader;
//
//}
//
//
///*
// Now we have be able to create a shader object, pass the shader source
// and them compile the shader.
//*/
//GLuint ShaderManager::MakeShader(GLenum type, const char* shaderSrc)
//{
//
//	GLuint TheShader = glCreateShader(type);	// 1st create the shader object
//
//	if (TheShader == 0) return false; // can't allocate so stop.
//
//
//	glShaderSource(TheShader, 1, &shaderSrc, NULL); // pass the shader source then compile it
//	glCompileShader(TheShader);
//
//	GLint  IsItCompiled;
//
//
//	glGetShaderiv(TheShader, GL_COMPILE_STATUS, &IsItCompiled); // After the compile we need to check the status and report any errors
//	if (!IsItCompiled)
//	{
//		GLint RetinfoLen = 0;
//		glGetShaderiv(TheShader, GL_INFO_LOG_LENGTH, &RetinfoLen);
//		if (RetinfoLen > 1)
//		{ // standard output for errors
//			char* infoLog = (char*)malloc(sizeof(char) * RetinfoLen);
//			glGetShaderInfoLog(TheShader, RetinfoLen, NULL, infoLog);
//			fprintf(stderr, "There was an error Error compiling shader:\n %s\n %s\n", shaderSrc, infoLog);
//			free(infoLog);
//		}
//		glDeleteShader(TheShader);
//		return false;
//	}
//	return TheShader;
//}
//
//void ShaderManager::SetUniformMatrix(glm::mat4 objectModel, const char* uniform)
//{
//	glUniformMatrix4fv(glGetUniformLocation(this->ID, uniform), 1, GL_FALSE, glm::value_ptr(objectModel));
//}
//
//void ShaderManager::Use()
//{
//	glUseProgram(ID);
//}