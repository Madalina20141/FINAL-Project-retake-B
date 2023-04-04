#pragma once
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "Input.h"
#include "Camera.h"
#include "Graphics.h"
#include <X11/Xlib.h>
#include <cstdlib>

enum class Gamestates
{
	Start,
	Loading,
	Game,
	SecondScene,
	GameOver
};

enum MouseButtons {
	leftClick = 0,
	rightClick = 1,
	middleClick = 2
};

class ImGuiManager
{
public:
	ImGuiManager(Graphics* graphics);
	~ImGuiManager();
	bool Init(Input* TheInput);


	bool Update(Input* keys, float deltaTime, Camera* cam, Gamestates& states); //bool DoINeedTo, bool MouseOn, float DT
	bool ReturnMouseInput(MouseButtons mouseButton);
	//bool Render(bool DoINeedTo);
	bool Remove()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		return true;
	}
	glm::vec2 startpos = { 0.f,0.f };
	glm::vec2 currentmousepos = { 0.f,0.f };
	glm::vec2 endpos = { 0.f,0.f };

private:
	Graphics* graphics;
	//bool lock_camera;
	bool heldDown = false;
};

