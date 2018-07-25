#include <iostream>
#include "display.h"
#include "inputManager.h"
#include <Windows.h>
#include "GUI/imgui.h"
#include "GUI/imgui_impl_glfw_gl3.h"


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
	bool inGame = true;
	bool endGame = true;
	bool eazy = true; bool normal = false; bool hard = false;
	bool toop = true;


	
	while (!glfwWindowShouldClose(display.m_window))
	{
		if (gui == false) { 
			glfwSetWindowShouldClose(display.m_window, GLFW_TRUE); 
		}
		
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::Begin("Snake!", &open, ImVec2(DISPLAY_WIDTH, DISPLAY_HEIGHT), 0.9f, ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_MenuBar);
			static float f = 0.0f;
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("dificulty"))
				{
					if (ImGui::MenuItem("Few objects", "eazy")) { scn.hardDif = false; }
					if (ImGui::MenuItem("Lots off objects", "hard")) { scn.hardDif = true; }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::Text("dificulty-level:");
			ImGui::Text(hard ? "	hard" : normal ? "	normal" : "	eazy");
			ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("view"))
				{
					if (ImGui::MenuItem("Top view", "View the snake from above ")) { toop = true; }
					if (ImGui::MenuItem("Snake view ", "View from the snake point of view ")) { toop = false; }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Text(""); ImGui::Text("");
			ImGui::Text("view:");
			ImGui::Text(toop ? "	Top view" : "	Snake view");
			ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
			ImGui::Text("Number of links:%d", numOfLinks);
			if (ImGui::Button("Increse number of links"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				numOfLinks++;
			if (ImGui::Button("Decrese number of links"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				numOfLinks--;
			//ImGui::SameLine();
			ImGui::Text(""); ImGui::Text("");
			ImGui::Text("Readme:");
			ImGui::Text("Use w to move up");
			ImGui::Text("Use d to move right");
			ImGui::Text("Use a to move left");
			ImGui::Text("Use s to move down");
			ImGui::Text("Avoid kiling objects");
			ImGui::Text("Dont eat yourself!");
			ImGui::Text("Dont hit the walls!");
			ImGui::Text("Collect incresing objects-shape of fruits");
			ImGui::Text("You can shot objects to kill using space, but it will decrese your snake if you dont have any free shots");
			ImGui::Text("Your goal is get to the end with the larger snake you can");
			ImGui::Text("Press P to stop");
			ImGui::Text("Press C to change camera mode");
			ImGui::Text("Press U to decrese your speed and I to increse your speed");
			ImGui::Text("");
			ImGui::Text("lets start!");
			ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); 
			if (ImGui::Button("																				start																					"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			{
				gui = false;
				scn.InitLevel();
			}

		}

		ImGui::End();
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
		int startLinkNum = numOfLinks;
		scn.setLinkNum(numOfLinks);
		scn.init(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]),toop);
		glfwSetWindowShouldClose(display.m_window, GLFW_FALSE);
		init();
		initCollisionDetection();
		int x = 0;
		std::string message ="";
		std::string guiMsg = "";
		while (!glfwWindowShouldClose(display.m_window))
		{
			//glfwPollEvents();
			if (scn.gameOver) {
				int endPoints = scn.Points + ((scn.GetLinkNum() - startLinkNum)>0? scn.GetLinkNum() - startLinkNum:0 * 100) + scn.GetFreeShots()*200;
				ImGui_ImplGlfwGL3_NewFrame();
				{
					ImGui::SetNextWindowPos(ImVec2(0, 0));
					ImGui::Begin("Snake!", &endGame, ImVec2(DISPLAY_WIDTH, DISPLAY_HEIGHT), 0.9f, ImGuiWindowFlags_NoResize |
						ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
						ImGuiWindowFlags_MenuBar);
					static float f = 0.0f;

					ImGui::SetWindowFontScale(3.5);
					ImGui::Text("					Game Over!");
					ImGui::SetWindowFontScale(1.7);
					ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
					ImGui::Text("Your dificulty-level:");
					ImGui::Text(hard ? "	hard" : normal ? "	normal" : "	eazy");
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); 
					ImGui::Text("Finised with :%d", scn.GetLinkNum(), "Number of Links");
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); 
					ImGui::Text("Number of points:%d", scn.Points);
					ImGui::Text(""); 
					ImGui::Text("Your total points is:%d", endPoints);
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); 
					ImGui::Text(guiMsg.c_str());
					ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
					if (ImGui::Button("												Start over												"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
					{
						scn.setLinkNum(startLinkNum);
						scn.levelStartOver();
						scn.init(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]), toop);
						initCollisionDetection();
						scn.gameOver = false;
						scn.setMessage("");
						guiMsg = "";
						message = "";
						scn.setFreeShots(2);
						scn.setSpeed();
					}

				}
				ImGui::End();
			}
			else {
				
				ImGui_ImplGlfwGL3_NewFrame();
				{
					ImGui::SetNextWindowPos(ImVec2(0, 0));
					ImGui::Begin("Snake!", &inGame, ImVec2(DISPLAY_WIDTH, DISPLAY_HEIGHT), 0.1f, ImGuiWindowFlags_NoResize |
						ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
						ImGuiWindowFlags_MenuBar);
					static float f = 0.0f;
					ImGui::Text("dificulty-level:");
					ImGui::Text(hard ? "	hard" : normal ? "	normal" : "	eazy");
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
					ImGui::Text("Number of Links:%d", scn.GetLinkNum());
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
					ImGui::Text("Number of points:%d", scn.Points);
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
					ImGui::Text("freeShotes:%d", scn.GetFreeShots());
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
					ImGui::Text(guiMsg.c_str());
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text("");
					ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); ImGui::Text(""); 
					if (ImGui::Button("												Start over												"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
					{
						scn.setLinkNum(startLinkNum);
						scn.levelStartOver();
						scn.init(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]), toop);
						 initCollisionDetection();
						 scn.setMessage("");
						 guiMsg = "";
						 message = "";
						 scn.setFreeShots(2);
						 scn.setSpeed();
					}
				}
				ImGui::End();
				int y = x;
			}
			if(!scn.gameOver)
			{
				toop = scn.getCameraMode();
				numOfLinks = scn.GetLinkNum();
				if (!scn.paused)
				{
					scn.move();
					scn.moveCamera();
					scn.UpdateLevel();
				}
				scn.checkCollisionFullLevel();
				message = scn.getMessage();
				if (message != "") {
					guiMsg = message;
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
				
			}
			
				//glfwPollEvents();
			
			
			display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
			if (display.IsFullscreen())
			{
				GLint viewport[4];
				glfwGetFramebufferSize(display.m_window, &viewport[2], &viewport[3]);
				window_size_callback(display.m_window, viewport[2], viewport[3]);
			}
			scn.draw(0, 0, false, toop?0:1); //change false to true for axis in every joint
			scn.levelDraw(0, 0, false, toop ? 0 : 1);
			
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
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
