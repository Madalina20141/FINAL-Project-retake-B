#include "../Headers/ImGuiManager.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "precomp.h"
#include <chrono>
#include <thread>


ImGuiManager::ImGuiManager(Graphics* graphics){	this->graphics = graphics;}

bool ImGuiManager::Init(Input* TheInput)
{
	// imgui setup
	const char* glsl_version = "#version 300 es";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init(glsl_version);
	return true;
}

bool ImGuiManager::Update(Input* TheInput, float deltaTime, Camera* cam, Gamestates& states)
{
	ImGuiIO& io = ImGui::GetIO();
	Window window_returned;

	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	bool result = XQueryPointer(
		graphics->x_display,
		graphics->win,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);


	io.AddMousePosEvent(static_cast<float>(root_x), static_cast<float>(root_y));
	io.DisplaySize = ImVec2(SCREENWIDTH, SCREENHEIGHT);
	io.DeltaTime = deltaTime;

	bool mousesHeldNow[2];
	mousesHeldNow[0] = mask_return & (1 << 8); // left
	mousesHeldNow[1] = mask_return & (1 << 10); // right
	bool OldRpi = false;
	io.AddMouseButtonEvent(0, mousesHeldNow[0]);
	io.AddMouseButtonEvent(1, mousesHeldNow[1]);
	io.MouseDrawCursor = OldRpi;

	io.MousePos = ImVec2(root_x, root_y); // we set up the mouse according to what x11 tells us
	currentmousepos = glm::vec2(root_x, root_y);
	//io.MouseDown[0] = mask_return & (1 << 8); // left
	//io.MouseDown[1] = mask_return & (1 << 10); // right
	io.MouseDown[2] = mask_return & (1 << 9); // centre
	io.MouseDrawCursor = OldRpi; // not really needed try true or false see what you get


	graphics->relmouseX = io.MousePos.x - graphics->mouseX; // I keep some relative movment
	graphics->relmouseY = io.MousePos.y - graphics->mouseY;

	graphics->mouseX = io.MousePos.x; // store the mouse somewhere every class gets acccess
	graphics->mouseY = io.MousePos.y;


	graphics->LastMouse = graphics->mouseButtons;
	graphics->LastmouseRight = graphics->mouseright;

	graphics->LastmouseLeft = graphics->mouseleft;


	graphics->mouseButtons = io.MouseDown[0] | io.MouseDown[1];
	graphics->mouseleft = io.MouseDown[0];
	graphics->mouseright = io.MouseDown[1];
	graphics->mousecent = io.MouseDown[2];



	graphics->debmouseleft = graphics->LastmouseLeft ^ graphics->mouseleft;
	graphics->debmouseleft &= graphics->mouseleft;

	graphics->debmouseright = graphics->LastmouseRight ^ graphics->mouseright;
	graphics->debmouseright &= graphics->mouseright;


	graphics->DebMouseButtons = graphics->LastMouse ^ graphics->mouseButtons;
	graphics->DebMouseButtons &= graphics->mouseButtons;



	if (io.MouseDown[1])
	{
		if (!heldDown)
		{
			startpos = { ((float)root_x / (float)SCREENWIDTH - 0.5f) * 2.0f, // [0,1920] -> [-1,1]
			-(((float)root_y / (float)SCREENHEIGHT - 0.5f) * 2.0f), // [0, 1080] -> [-1,1]}
			};
			heldDown = true;
		}

		endpos = { ((float)root_x / (float)SCREENWIDTH - 0.5f) * 2.0f, // [0,1920] -> [-1,1]
		-(((float)root_y / (float)SCREENHEIGHT - 0.5f) * 2.0f), // [0, 1080] -> [-1,1]}
		};


	}
	else
	{
		heldDown = false;
	}

	
	//io.MouseWheel = this->TheInput->TheMouse.wheel; // my mouse system is easier to grab wheel data

	//graphics->MouseWheel = TheInput->TheMouse.wheel;
	//TheInput->TheMouse.wheel = 0;

		// all this does is setup the x11 collections for mouse.


	//Window one
	static int counter;
	float test = 2.10f;
	float loadingProgress = 0.0f;



	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	io.WantCaptureMouse = true;

	ImGui::SetNextWindowPos(ImVec2{ 100,100 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 400,400 }, ImGuiCond_Once);
	ImGui::Begin("Configs", NULL, ImGuiWindowFlags_None);

	static int count = 0;
	char* names[] = { "Ana", "Ari", "Mimi" , "Alec" , "Tudi", /*"Dar" ,*/ "Cata", "Dia", "Madi", "Lio", "Pati" };
	ImGui::ListBox("ids", &count, names, 10);

	switch (states)
	{
		if (ImGui::Button("Start the Game"))
	case Gamestates::Start:
		ImGui::Text("RTS-B project world ");

		

		if (ImGui::Button("Start"))
		{
			counter++;
			if (ImGui::Button("Game"))
			{
				states = Gamestates::Game;
				ImGui::Text("Counter=%i , test=%f", counter, test);
				ImGui::Text("pitch=%f, yaw=%f", cam->pitch, cam->yaw);
				ImGui::Text("front:X=%f, front:Y=%f,front:Z=%f", cam->Front.x, cam->Front.y, cam->Front.z);

			}
			/*if (ImGui::Button("Second menu")) {
				states = Gamestates::SecondScene;
			}*/
		}


		if (ImGui::Button("Game Over"))
		{
			states = Gamestates::GameOver;
		}
		break;

	case Gamestates::Game:
		//ImGui::Begin("Frame Rate");
		if (ImGui::Button("Game Menu"))
		{

			// Display the frame rate
			float fps = ImGui::GetIO().Framerate;
			ImGui::Text("Frame rate: %.1f FPS", fps);

			//states = Gamestates::Game;
		}
		if (ImGui::Button("GameOver"))
		{
			states = Gamestates::GameOver;
		}
		break;
	/*case Gamestates::SecondScene:
		if (ImGui::Button("Second Menu"))
		{
			ImGui::Text("Camera X: %.3f", cam->Position.x);
			ImGui::Text("Camera Y: %.3f", cam->Position.y);
			ImGui::Text("Camera Z: %.3f", cam->Position.z);
			ImGui::Text("Camera yaw: %.3f", cam->GetYaw());
			ImGui::Text("Camera pitch: %.3f", cam->GetPitch());

		}
		if (ImGui::Button("GameOver"))
		{
			states = Gamestates::GameOver;
		}
		break;*/
	case Gamestates::GameOver:
		ImGui::Text("Try again!");
		ImGui::Text("You can do it!");
		if (ImGui::Button("Replay"))
		{
			states = Gamestates::Start;
		}
		break;
	default:
		break;
	}
	ImGui::End();

	//---2nd Window

	ImGui::SetNextWindowPos(ImVec2{ 1000,100 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 200,200 }, ImGuiCond_Once);
	ImGui::Begin("Details", NULL, ImGuiWindowFlags_None);
	switch (states)
	{
	case Gamestates::SecondScene:
		ImGui::Text("RTS-B project world ");
		if (ImGui::Button("load"))
		{
			if (ImGui::Button("Start load"))
			{
				counter++;
				if (ImGui::Button("Game"))
				{
					ImGui::Text("Counter=%i , test=%f", counter, test);
					ImGui::Text("pitch=%f, yaw=%f", cam->pitch, cam->yaw);
					ImGui::Text("front:X=%f, front:Y=%f,front:Z=%f", cam->Front.x, cam->Front.y, cam->Front.z);

					if (ImGui::Button("Game")) {
						states = Gamestates::Game;

					}

					//states = Gamestates::SecondeScene;
				}
			}
			//states = Gamestates::Loading;

			if (ImGui::Button("Game"))
			{
				ImGui::Text("Counter=%i , test=%f", counter, test);
				ImGui::Text("pitch=%f, yaw=%f", cam->pitch, cam->yaw);
				ImGui::Text("front:X=%f, front:Y=%f,front:Z=%f", cam->Front.x, cam->Front.y, cam->Front.z);
				states = Gamestates::Game;
				states = Gamestates::SecondScene;
			}
		}
		
	}
	ImGui::End();

	//---3rd Window
	ImGui::SetNextWindowPos(ImVec2{ 1700,100 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 200,200 }, ImGuiCond_Once);
	ImGui::Begin("Loading screen", NULL, ImGuiWindowFlags_None);
	switch (states)
	{
	case Gamestates::Loading:
		if (ImGui::Button("Loading Screen")) {
			// Simulate loading progress
			loadingProgress += deltaTime / 10.0f;
			if (loadingProgress >= 1.0f) {
				loadingProgress = 0.00f;
			}

			// Render the loading progress bar
			ImGui::ProgressBar(loadingProgress, ImVec2(-1.0f, 0.0f));

			// If the loading is complete, transition to the game state
			if (loadingProgress == 1.0f) {
				states = Gamestates::Game;
				break;
			}
		}
		break;
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

bool ImGuiManager::ReturnMouseInput(MouseButtons mouseButton)
{
	ImGuiIO& io = ImGui::GetIO();

	return io.MouseDown[mouseButton];
}

ImGuiManager::~ImGuiManager()
{
	Remove();
}