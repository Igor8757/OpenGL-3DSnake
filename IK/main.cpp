#include <iostream>
#include "display.h"
#include "inputManager.h"
#include <Windows.h>
#include "../engine3D/GUI/imgui.h"
#include "../engine3D/GUI/imgui_impl_glfw_gl3.h"
void init()
{
	glfwSetKeyCallback(display.m_window, key_callback);
	glfwSetWindowSizeCallback(display.m_window, window_size_callback);
}

void initCollisionDetection()
{
	scn.createKDTreesForShapes();
	scn.createKDTreesForLevelShapes();
}



int main(int argc, char** argv)
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};
	unsigned int indices[] = { 0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};


	scn.addShader("./res/shaders/basicShader");
	scn.addShader("./res/shaders/pickingShader");
	scn.addShader("./res/shaders/phongShader");
	scn.addShader("./res/shaders/basicShader2");


	// Setup window

	if (!glfwInit())
		return 1;

	ImGui::CreateContext();

	ImGui_ImplGlfwGL3_Init(display.m_window, true);

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool gui = true;
	// Main loop
	static int numOfLinks = 4;
	bool open = true;
	bool eazy = true; bool normal = false; bool hard = false;
	bool toop = true;


	//GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui GLFW+OpenGL3 example", NULL, NULL);
	while (!glfwWindowShouldClose(display.m_window))
	{
		if (gui == false) { glfwSetWindowShouldClose(display.m_window, GLFW_TRUE); }
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::Begin("Snake!", &open, ImVec2(DISPLAY_WIDTH, DISPLAY_HEIGHT), 0.9f, ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_MenuBar);
			static float f = 0.0f;
			//ImGui::Begin("Test Window", DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 0, 4, 4);
			//ImGui::Text("Hello, world!");   
			// Display some text (you can use a format string too)
			//ImGui::Text("dificulty");
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("dificulty"))
				{
					if (ImGui::MenuItem("Ffew objects", "eazy")) { eazy = true; hard = false; normal = false; }
					if (ImGui::MenuItem("Lot off objects", "normal")) { normal = true; hard = false; }
					if (ImGui::MenuItem("Lot off shuting objects", "hard")) { hard = true; }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::Text("dificulty-level:");
			ImGui::Text(hard ? "	hard" : normal ? "	normal" : "	eazy");
			ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
			//ImGui::Checkbox("Another Window", &show_another_window);
			//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
			//ImGui::Checkbox("Another Window", &show_another_window);
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("view"))
				{
					if (ImGui::MenuItem("Top view", "View the snake from above ")) { toop = false; }
					if (ImGui::MenuItem("Snake view ", "View from the snake point of view ")) { toop = true; }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Text(""); ImGui::Text("");
			ImGui::Text("view:");
			ImGui::Text(toop ? "	Top view" : "	Snake view");
			ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
			ImGui::Text("Number of chains:%d", numOfLinks);
			if (ImGui::Button("Increse number of chains"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				numOfLinks++;
			if (ImGui::Button("Decrese number of chains"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				numOfLinks--;
			//ImGui::SameLine();
			ImGui::Text(""); ImGui::Text("");
			ImGui::Text("readme:");
			ImGui::Text("Use w to move up");
			ImGui::Text("Use d to move right");
			ImGui::Text("Use a to move left");
			ImGui::Text("Use s to move down");
			ImGui::Text("Avoid kiling objects");
			ImGui::Text("Avoid decresind objects");
			if (hard) {
				ImGui::Text("Avoid Shoting objects- shoot will decres your snake ");
			}
			ImGui::Text("Collect incresing objects-shape of fruits");
			ImGui::Text("You can shoot at objects using space - but it will decrese your snake");
			ImGui::Text("Your goal is get to the end with the larger snake you can");
			ImGui::Text("Press P to stop");
			ImGui::Text("");
			ImGui::Text("lets start!");
			ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
			if (ImGui::Button("																				start																					"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				gui = false;
			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		}

		ImGui::End();
		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(display.m_window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(display.m_window);

	}
	if (!gui) {
		scn.setLinkNum(numOfLinks);
		scn.init(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]),toop);
		glfwSetWindowShouldClose(display.m_window, GLFW_FALSE);
		init();
		//glfwSetInputMode(display.m_window,GLFW_STICKY_MOUSE_BUTTONS,1);
		initCollisionDetection();

		while (!glfwWindowShouldClose(display.m_window))
		{
			toop = scn.getCameraMode();
			numOfLinks = scn.GetLinkNum();
			if (scn.checkCollisionFullLevel()) {
			//if (scn.checkCollision(3, 4)) {
			}
			else {
				
			}
			if (scn.isActive())
			{
				int j = 3;
				Sleep(30);
				float dif = glm::distance(scn.getTipPosition(numOfLinks - 1), scn.destPosition);
				//std::cout << dif << std::endl;
				if (dif > scn.delta || j == 0) {
					scn.makeChange();
				}
				
				j--;
			}
			display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
			if (display.IsFullscreen())
			{
				GLint viewport[4];
				glfwGetFramebufferSize(display.m_window, &viewport[2], &viewport[3]);
				window_size_callback(display.m_window, viewport[2], viewport[3]);
			}
			if(!scn.paused)
			{
				scn.move();
				//scn.moveCamera();
				scn.UpdateLevel();
			}		
			scn.draw(0, 0, false, toop?0:1); //change false to true for axis in every joint
			scn.levelDraw(0, 0, false);
			display.SwapBuffers();
			glfwPollEvents();

		}
	}
	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;

}
