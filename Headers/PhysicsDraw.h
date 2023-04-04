#pragma once

// it is sometimes useful to draw the bounding containment around physics objects.
// Bullet was designed to do this with OpenGL 1.xx which had line draw systems
// we are using shader based draw systems so some issues occur when trying to draw lines
// we must store them in a buffer and use a flush sysem to draw and release them
// this will be slow but this is a debug system


#include <bullet/LinearMath/btIDebugDraw.h>
#include "GLES2/gl2.h"
#include <vector>
// Include GLM
//#undef countof
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//#undef countof
// there are already debug draw systems in Bullet, all we are doing is overriding them
/*
enum	DebugDrawModes
{
	DBG_NoDebug              = 0,
	DBG_DrawWireframe        = 1,
	DBG_DrawAabb             = 2,
	DBG_DrawFeaturesText     = 4,
	DBG_DrawContactPoints    = 8,
	DBG_NoDeactivation       = 16,
	DBG_NoHelpText           = 32,
	DBG_DrawText             = 64,
	DBG_ProfileTimings       = 128,
	DBG_EnableSatComparison  = 256,
	DBG_DisableBulletLCP     = 512,
	DBG_EnableCCD            = 1024,
	DBG_DrawConstraints      = (1 << 11),
	DBG_DrawConstraintLimits = (1 << 12),
	DBG_FastWireframe        = (1 << 13),
	DBG_DrawNormals          = (1 << 14),
	DBG_MAX_DEBUG_DRAW_MODE
};
*/

class PhysicsDraw : public btIDebugDraw // << notice we are inheriting from an inbuilt draw class
{
public:
	struct
	{
		btVector3 p1; // 4*float
		btVector3 Colour1;
		btVector3 p2;
		btVector3 Colour2;

	} typedef LineValues;

	// overide the original model funcitons

	PhysicsDraw();
	~PhysicsDraw();

	void SetView(glm::mat4 View);


	void Init();

	void setDebugMode(int debugMode) override;
	int getDebugMode() const override;
	

	// and provide a new drawline function
	void drawLine(const btVector3& from,
		const btVector3& to,
		const btVector3& color) override; // the line, is actually stored


	void drawLine(glm::vec3 from, glm::vec3 to); // same as above but use glm vec

	//void drawRayLine(const btVector3& from, const btVector3& to, const btVector3& color);

	void drawRayLine(glm::vec3 from, glm::vec3 to);

	void drawRayLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);

	void DoDebugDraw(); // this will flush out the buffer

	void DoDebugDrawForSelectionBox();

	static glm::vec3 btToGlm(const btVector3& v)
	{
		return glm::vec3(v.x(), v.y(), v.z());
	}

	void drawRayLine(const btVector3& from, const btVector3& to, const btVector3& color) 
	{
		// Convert btVector3 to glm::vec3
		glm::vec3 glmFrom = btToGlm(from);
		glm::vec3 glmTo = btToGlm(to);
		glm::vec3 glmColor = btToGlm(color);

		// Define the vertex data for the line
		glm::vec3 vertices[] = { glmFrom, glmTo };

		// Create and bind a vertex buffer
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Upload the vertex data to the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Enable the vertex attribute array for position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Set the color
		glVertexAttrib3f(1,glmColor.x, glmColor.y, glmColor.z);

		// Draw the line
		glDrawArrays(GL_LINES, 0, 2);

		// Disable the vertex attribute array
		glDisableVertexAttribArray(0);

		// Unbind the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo);
	}


	// we might use this	

	void reportErrorWarning(const char* warningString) override;
	

	// though unused we must define these	
	void draw3dText(const btVector3& location, const char* textString) override;
	

	void drawContactPoint(const btVector3& pointOnB,
		const btVector3& normalOnB,
		btScalar distance,
		int lifeTime,
		const btVector3& color) override;

	void clearLines();
	void ToggleDebugFlag(int flag);
	int m_debugMode;



	// shader info	
	static GLuint ProgramObject; // this is a very simple line draw shader, no frills

	std::vector<LineValues> TheLines; // we store this when using bullet, draw it after our renders, then clear it

	GLuint LinesVAO = -1; //VAO's are not too effective with dynamic memory, this has depricated
	GLuint Program;
	glm::mat4 Projection, View;
};
