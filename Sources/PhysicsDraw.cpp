#include "../Headers/PhysicsDraw.h"


PhysicsDraw::PhysicsDraw() 
{
	setDebugMode(1);
	Projection =  glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 200.0f);
	Init();
}
PhysicsDraw::~PhysicsDraw() 
{


}
void PhysicsDraw::SetView(glm::mat4 View)
{
	this->View = View;
}

GLuint LoadShader(GLenum type, const char* shaderSrc)
{
	// 1st create the shader object
	GLuint TheShader = glCreateShader(type);

	if (TheShader == 0) return false; // can't allocate so stop.

	// pass the shader source then compile it
	glShaderSource(TheShader, 1, &shaderSrc, NULL);
	glCompileShader(TheShader);

	GLint  IsItCompiled;

	// After the compile we need to check the status and report any errors
	glGetShaderiv(TheShader, GL_COMPILE_STATUS, &IsItCompiled);
	if (!IsItCompiled)
	{
		GLint RetinfoLen = 0;
		glGetShaderiv(TheShader, GL_INFO_LOG_LENGTH, &RetinfoLen);
		if (RetinfoLen > 1)
		{
			// standard output for errors
			char* infoLog = (char*)malloc(sizeof(char) * RetinfoLen);
			glGetShaderInfoLog(TheShader, RetinfoLen, NULL, infoLog);
			fprintf(stderr, "Error compiling this shader:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteShader(TheShader);
		return false;
	}
	return TheShader;
}

void PhysicsDraw::Init()
{

	// hard code for now student can optimise :D

	GLbyte vShaderStr[] =
		"#version 310 es                        \n"
		"precision mediump float;     \n"
		"layout(location = 0) in vec3 a_position;   \n"
		"layout(location = 1) in vec3 a_color;   \n"
		"uniform mat4 MVP;            \n"
		"out vec4 color;     \n"
		"void main()                  \n"
		"{ 							  \n"
		" gl_Position =  MVP * vec4(a_position,1);\n"
		" color = vec4(a_color,1);  \n"
		"}                            \n";

	GLbyte fShaderStr[] =
		"#version 310 es                        \n"
		"precision mediump float;                            \n"
		"in vec4 color;                            \n"
		"out vec4 ColorFrag;                            \n"
		"void main()                                         \n"
		"{                                                   \n"
		" ColorFrag = color;\n"
		"}                                                   \n";

	GLuint FragmementShader;
	GLuint VertexShader;
	// Load and compile the vertex/fragment shaders
	VertexShader = LoadShader(GL_VERTEX_SHADER, (const char*)vShaderStr);
	FragmementShader = LoadShader(GL_FRAGMENT_SHADER, (const char*)fShaderStr);

	// Create the program object	
	Program = glCreateProgram();
	if (Program == 0) 	return ;

	// now we have the V and F shaders  attach them to the progam object
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmementShader);

	// Link the program
	glLinkProgram(Program);
	// Check the link status
	GLint AreTheylinked;
	glGetProgramiv(Program, GL_LINK_STATUS, &AreTheylinked);
	if (!AreTheylinked)
	{
		GLint RetinfoLen = 0;
		// check and report any errors
		glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &RetinfoLen);
		if (RetinfoLen > 1)
		{
			GLchar* infoLog = (GLchar*)malloc(sizeof(char) * RetinfoLen);
			glGetProgramInfoLog(Program, RetinfoLen, NULL, infoLog);
			fprintf(stderr, "Error linking program:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteProgram(Program);
		return ;
	}

}
void PhysicsDraw::setDebugMode(int debugMode)  // what content do we want to see??
{
	m_debugMode = debugMode; // +DBG_DrawConstraints + DBG_DrawConstraintLimits; //DBG_DrawWireframe
}

int PhysicsDraw::getDebugMode() const
{
	return m_debugMode;
}
void PhysicsDraw::drawLine(const btVector3& from,
	const btVector3& to,
	const btVector3& color) 
{
	LineValues lines = { from ,color,to,color };
	TheLines.push_back(lines);
}
void PhysicsDraw::drawLine(glm::vec3 from, glm::vec3 to)
{
	LineValues lines = {btVector3(from.x,from.y,from.z) ,btVector3(0.f,0.f,0.f),btVector3(to.x,to.y,to.z),btVector3(0.f,0.f,0.f) };
	TheLines.push_back(lines);
}

//------- GLM to BT conversion


//void PhysicsDraw::drawRayLine(const btVector3& from,
//	const btVector3& to,
//	const btVector3& color)
//{
//	LineValues lines = { from ,color,to,color };
//	TheLines.push_back(lines);
//}
void PhysicsDraw::drawRayLine(glm::vec3 from, glm::vec3 to)
{
	LineValues lines = { btVector3(from.x,from.y,from.z) ,btVector3(0.f,0.f,0.f),btVector3(to.x,to.y,to.z),btVector3(0.f,0.f,0.f) };
	TheLines.push_back(lines);
}

void PhysicsDraw::drawRayLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	// Convert glm::vec3 to btVector3
	btVector3 btFrom(from.x, from.y, from.z);
	btVector3 btTo(to.x, to.y, to.z);
	btVector3 btColor(color.x, color.y, color.z);

	// Call the original drawLine function
	drawRayLine(btFrom, btTo, btColor);
}
void PhysicsDraw::DoDebugDraw()
{
	glLineWidth(2.0f);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(Program);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(btVector3), (btVector3*)&TheLines[0].p1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 2 * sizeof(btVector3), (btVector3*)&TheLines[0].Colour1);
	glEnableVertexAttribArray(1);

	glm::mat4 VP = Projection * View;
	GLuint MatrixID = glGetUniformLocation(Program, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &VP[0][0]);

	glDrawArrays(GL_LINES, 0, TheLines.size() * 2);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	TheLines.clear();
}

void PhysicsDraw::DoDebugDrawForSelectionBox()
{
	glLineWidth(2.0f);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(Program);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(btVector3), (btVector3*)&TheLines[0].p1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(btVector3), (btVector3*)&TheLines[0].Colour1);
	glEnableVertexAttribArray(1);

	glm::mat4 MVP = glm::mat4(1.0f);
	GLuint MatrixID = glGetUniformLocation(Program, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glDrawArrays(GL_LINES, 0, TheLines.size() * 2);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	TheLines.clear();
}

void PhysicsDraw::reportErrorWarning(const char* warningString)
{
	printf("Physics reports an error:- %s /n", warningString);
}


// though unused we must define these	
void PhysicsDraw::draw3dText(const btVector3& location, const char* textString)
{
	printf("Attempting to draw:- %s /n", textString);
}

void PhysicsDraw::drawContactPoint(const btVector3& pointOnB,
	const btVector3& normalOnB,
	btScalar distance,
	int lifeTime,
	const btVector3& color) 
{

}

void PhysicsDraw::clearLines()
{

}
void PhysicsDraw::ToggleDebugFlag(int flag)
{

}

